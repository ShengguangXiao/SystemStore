/*****************************************************************************
 * Table.cpp -- $Id$
 *
 * Purpose
 *   Implements the Table class which is the base class for classes that
 *   represent database tables.
 *
 * Indentation
 *   Four characters. No tabs!
 *
 * Modifications
 *   2015-06-14 (MM) Added GetMaxFor methods.
 *   2012-05-29 (MM) Added a BuildUpdateCommand2 method.
 *   2012-05-14 (MM) Added more shorthand "For" methods (id with string).
 *   2012-04-10 (MM) Added shorthand methods for comparing a field against
 *                   a list of values.
 *   2012-03-03 (MM) Added more shorthand "For" methods and bit-unique.
 *   2012-02-23 (MM) Added some shorthand "For" methods.
 *   2011-11-30 (MM) Added two more if-null methods.
 *   2011-10-11 (MM) Started using foreign key constraints again.
 *   2011-08-08 (YB) Added a BuildDelete method that takes a list of fields
 *                   to parameterize in the where clause (similar to what
 *                   BuildSelectQuery does).
 *   2011-07-27 (MM) Started ignoring foreign key constraints to improve
 *                   insertion performance.
 *   2011-03-16 (MM) Added the GetLastInsertedRowId method.
 *   2011-03-11 (MM) Added an indexer method (Index).
 *   2010-12-16 (MM) Added SQL builders, binders, and another Join.
 *   2010-12-05 (MM) Created.
 *
 * Copyright (c) 2010-2012, 2015 Keysight Technologies, Inc.  All rights reserved.
 ****************************************************************************/

#include "Common/BaseDefs.h"
#include "Table.h"

namespace AOI
{
namespace SystemStore
{
    Table::Table(DatabasePtr const &db)
      : _db(db)
    {
        if (!this->_db)
            throw std::exception(SL("Null connection argument to SQLiteImpl1::Table::Table."));
    }

    StatementPtr Table::BuildDeleteCommand(int keyFieldIndex) const
    {
        String const fmt = SL("delete from %s where %s = ?;");
        String const sql = (boost::format(fmt) % GetTableName() % GetFieldName(keyFieldIndex)).str();
        return std::make_shared<SQLite::Statement>( *_db.get(), sql );
    }

    StatementPtr Table::BuildDeleteCommand2(int keyFieldIndex1, int keyFieldIndex2) const
    {
        String const fmt = SL("delete from %s where %s = ? and %s = ?;");
        String const sql = (boost::format(fmt) % GetTableName() % GetFieldName(keyFieldIndex1) % GetFieldName(keyFieldIndex2)).str();
        return std::make_shared<SQLite::Statement>( *_db.get(), sql );
    }

    StatementPtr Table::BuildDeleteCommand3(int keyFieldIndex1, int keyFieldIndex2, int keyFieldIndex3) const
    {
        String const fmt = SL("delete from %s where %s = ? and %s = ? and %s = ?;");
        String const sql = (boost::format(fmt) % GetTableName() % GetFieldName(keyFieldIndex1) % GetFieldName(keyFieldIndex2) % GetFieldName(keyFieldIndex3)).str();
        return std::make_shared<SQLite::Statement>( *_db.get(), sql );
    }

    StatementPtr Table::BuildDeleteCommand(int const *fieldIndexBegin, int const *fieldIndexEnd) const
    {
        assert(fieldIndexBegin != fieldIndexEnd && "Invalid field index iterators in Table::BuildDeleteCommand.");

        String const fmt = SL("delete from %s where ");
        String       sql = (boost::format(fmt) % GetTableName()).str();

        for (int const *i = fieldIndexBegin, *n = fieldIndexEnd; i != n; ++i)
        {
            sql += ((i == fieldIndexBegin) ? GetFieldName(*i) : SL(" and ") + GetFieldName(*i));
            sql += SL(" = ?");
        }
        sql += SL(";");

        return std::make_shared<SQLite::Statement>( *_db.get(), sql );
    }

    StatementPtr Table::BuildInsertCommand(int const *fieldIndexBegin, int const *fieldIndexEnd) const
    {
        assert(fieldIndexBegin != fieldIndexEnd && "Invalid field index iterators in Table::BuildInsertCommand.");

        String sql1 = SL("insert into ") + GetTableName() + SL(" (");
        String sql2 = SL(") values (");

        for (int const *i = fieldIndexBegin, *n = fieldIndexEnd; i != n; ++i)
        {
            sql1 += ((i == fieldIndexBegin) ? SL("")  : SL(", ")) + GetFieldName(*i);
            sql2 += ((i == fieldIndexBegin) ? SL("?") : SL(", ?"));
        }

        String sql = sql1 + sql2 + SL(");");
        return std::make_shared<SQLite::Statement>( *_db.get(), sql );
    }    

    StatementPtr Table::BuildSelectCommand(int fieldIndex, int keyFieldIndex) const
    {
        // Used when one single-column row will be selected.
        String const fmt = SL("select %s from %s where %s = ?;");
        String const sql = (boost::format(fmt) % GetFieldName(fieldIndex) % GetTableName() % GetFieldName(keyFieldIndex)).str();
        return std::make_shared<SQLite::Statement>( *_db.get(), sql );
    }

    StatementPtr Table::BuildSelectCommand(int fieldIndex, int keyFieldIndex1, int keyFieldIndex2) const
    {
        // Used when one single-column row will be selected.
        String const fmt = SL("select %s from %s where %s = ? and %s = ?;");
        String const sql = (boost::format(fmt) % GetFieldName(fieldIndex) % GetTableName() % GetFieldName(keyFieldIndex1) % GetFieldName(keyFieldIndex1) ).str();
        return std::make_shared<SQLite::Statement>( *_db.get(), sql );
    }

    StatementPtr Table::BuildSelectQuery(int fieldIndex) const
    {
         // Used when one single-column row will be selected.
        String const fmt = SL("select %s from %s;");
        String const sql = (boost::format(fmt) % GetFieldName(fieldIndex) % GetTableName()).str();
        return std::make_shared<SQLite::Statement>( *_db.get(), sql );
    }

    StatementPtr Table::BuildSelectQuery(int fieldIndex, int keyFieldIndex, int sortFieldIndex, bool distinct) const
    {
        // Used when zero or more single-column rows will be selected.
        bool order = sortFieldIndex != UNSORTED;
        String sel = (distinct) ? SL("select distinct ") : SL("select ");
        String sql = sel           + GetFieldName(fieldIndex)
                   + SL(" from ")  + GetTableName()
                   + SL(" where ") + GetFieldName(keyFieldIndex) + SL(" = ?");
        if (order) sql += SL(" order by ") + GetFieldName(sortFieldIndex);
        sql += ";";
        return std::make_shared<SQLite::Statement>( *_db.get(), sql );
    }

    StatementPtr Table::BuildSelectNotNullQuery(int fieldIndex, int keyFieldIndex, int sortFieldIndex, bool distinct) const
    {
        // Used when zero or more single-column rows will be selected.
        bool order = sortFieldIndex != UNSORTED;
        String sel = (distinct) ? SL("select distinct ") : SL("select ");
        String sql = sel           + GetFieldName(fieldIndex)
                   + SL(" from ")  + GetTableName()
                   + SL(" where ") + GetFieldName(keyFieldIndex) + SL(" = ?");
                   + SL(" and ")   + GetFieldName(fieldIndex)    + SL(" is not null");
        if (order) sql += SL(" order by ") + GetFieldName(sortFieldIndex);
        sql += ";";
        return std::make_shared<SQLite::Statement>( *_db.get(), sql );
    }

    StatementPtr Table::BuildSelectQuery2(int fieldIndex, int keyFieldIndex1, int keyFieldIndex2, int sortFieldIndex, bool distinct) const
    {
        // Used when zero or more single-column rows will be selected.
        bool order = sortFieldIndex != UNSORTED;
        String sel = (distinct) ? SL("select distinct ") : SL("select ");
        String sql = sel           + GetFieldName(fieldIndex)
                   + SL(" from ")  + GetTableName()
                   + SL(" where ") + GetFieldName(keyFieldIndex1) + SL(" = ?")
                   + SL(" and ")   + GetFieldName(keyFieldIndex2) + SL(" = ?");
        if (order) sql += SL(" order by ") + GetFieldName(sortFieldIndex);
        sql += ";";
        return std::make_shared<SQLite::Statement>( *_db.get(), sql );
    }

    StatementPtr Table::BuildSelectQuery3(int fieldIndex, int keyFieldIndex1, int keyFieldIndex2, int keyFieldIndex3, int sortFieldIndex, bool distinct) const
    {
        // Used when zero or more single-column rows will be selected.
        bool order = sortFieldIndex != UNSORTED;
        String sel = (distinct) ? SL("select distinct ") : SL("select ");
        String sql = sel           + GetFieldName(fieldIndex)
                   + SL(" from ")  + GetTableName()
                   + SL(" where ") + GetFieldName(keyFieldIndex1) + SL(" = ?")
                   + SL(" and ")   + GetFieldName(keyFieldIndex2) + SL(" = ?")
                   + SL(" and ")   + GetFieldName(keyFieldIndex3) + SL(" = ?");
        if (order) sql += SL(" order by ") + GetFieldName(sortFieldIndex);
        sql += ";";
        return std::make_shared<SQLite::Statement>( *_db.get(), sql );
    }

    StatementPtr Table::BuildSelectQuery(int const *fieldIndexBegin, int const *fieldIndexEnd, int keyFieldIndex, int sortFieldIndex, bool distinct) const
    {
        assert(fieldIndexBegin != fieldIndexEnd && "Invalid field index iterators in Table::BuildSelectQuery.");

        bool order = sortFieldIndex != UNSORTED;
        String sql = (distinct) ? SL("select distinct") : SL("select");

        for (int const *i = fieldIndexBegin, *n = fieldIndexEnd; i != n; ++i)
            sql += ((i == fieldIndexBegin) ? SL(" ") : SL(", ")) + GetFieldName(*i);

        sql += SL(" from ") + GetTableName() + SL(" where ") + GetFieldName(keyFieldIndex) + SL(" = ?");
        if (order) sql += SL(" order by ") + GetFieldName(sortFieldIndex);
        sql += ";";
        return std::make_shared<SQLite::Statement>( *_db.get(), sql );
    }

    StatementPtr Table::BuildSelectQuery(int const *fieldIndexBegin, int const *fieldIndexEnd, int const *keyFieldIndexBegin, int const *keyFieldIndexEnd, int sortFieldIndex, bool distinct) const
    {
        assert(fieldIndexBegin != fieldIndexEnd && "Invalid field index iterators in Table::BuildSelectQuery.");

        bool order = sortFieldIndex != UNSORTED;
        String sql = (distinct) ? SL("select distinct") : SL("select");

        for (int const *i = fieldIndexBegin, *n = fieldIndexEnd; i != n; ++i)
            sql += ((i == fieldIndexBegin) ? SL(" ") : SL(", ")) + GetFieldName(*i);

        sql += SL(" from ") + GetTableName();
        
        for (int const *i = keyFieldIndexBegin, *n = keyFieldIndexEnd; i != n; ++i)
            sql += ((i == keyFieldIndexBegin) ? SL(" where ") : SL(" and ")) + GetFieldName(*i) + SL(" = ?");

        if (order) sql += SL(" order by ") + GetFieldName(sortFieldIndex);
        sql += ";";
        return std::make_shared<SQLite::Statement>( *_db.get(), sql );
    }

    StatementPtr Table::BuildSelectCountCommand() const
    {
        String const fmt = SL("select count(*) from %s;");
        String const sql = (boost::format(fmt) % GetTableName()).str();
        return std::make_shared<SQLite::Statement>( *_db.get(), sql );
    }

    StatementPtr Table::BuildSelectCountCommand(int keyFieldIndex) const
    {
        String const fmt = SL("select count(*) from %s where %s = ?;");
        String const sql = (boost::format(fmt) % GetTableName() % GetFieldName(keyFieldIndex)).str();
        return std::make_shared<SQLite::Statement>( *_db.get(), sql );
    }

    StatementPtr Table::BuildSelectCountCommand2(int keyFieldIndex1, int keyFieldIndex2) const
    {
        String const fmt = SL("select count(*) from %s where %s = ? and %s = ?;");
        String const sql = (boost::format(fmt) % GetTableName() % GetFieldName(keyFieldIndex1) % GetFieldName(keyFieldIndex2)).str();
        return std::make_shared<SQLite::Statement>( *_db.get(), sql );
    }

    StatementPtr Table::BuildSelectCountCommand3(int keyFieldIndex1, int keyFieldIndex2, int keyFieldIndex3) const
    {
        String const fmt = SL("select count(*) from %s where %s = ? and %s = ? and %s = ?;");
        String const sql = (boost::format(fmt) % GetTableName() % GetFieldName(keyFieldIndex1) % GetFieldName(keyFieldIndex2) % GetFieldName(keyFieldIndex3)).str();
        return std::make_shared<SQLite::Statement>( *_db.get(), sql );
    }

    StatementPtr Table::BuildSelectExistsCommand(int keyFieldIndex) const
    {
        String const fmt = SL("select exists(select * from %s where %s = ?);");
        String const sql = (boost::format(fmt) % GetTableName() % GetFieldName(keyFieldIndex)).str();
        return std::make_shared<SQLite::Statement>( *_db.get(), sql );
    }

    StatementPtr Table::BuildSelectExistsCommand2(int keyFieldIndex1, int keyFieldIndex2) const
    {
        String const fmt = SL("select exists(select * from %s where %s = ? and %s = ?);");
        String const sql = (boost::format(fmt) % GetTableName() % GetFieldName(keyFieldIndex1) % GetFieldName(keyFieldIndex2)).str();
        return std::make_shared<SQLite::Statement>( *_db.get(), sql );
    }

    StatementPtr Table::BuildSelectExistsCommand3(int keyFieldIndex1, int keyFieldIndex2, int keyFieldIndex3) const
    {
        String const fmt = SL("select exists(select * from %s where %s = ? and %s = ? and %s = ?);");
        String const sql = (boost::format(fmt) % GetTableName() % GetFieldName(keyFieldIndex1) % GetFieldName(keyFieldIndex2) % GetFieldName(keyFieldIndex3)).str();
        return std::make_shared<SQLite::Statement>( *_db.get(), sql );
    }

    StatementPtr Table::BuildSelectMaxCommand(int fieldIndex) const
    {
        String const fmt = SL("select max(%s) from %s;");
        String const sql = (boost::format(fmt) % GetFieldName(fieldIndex) % GetTableName()).str();
        return std::make_shared<SQLite::Statement>( *_db.get(), sql );
    }

    StatementPtr Table::BuildSelectMaxCommand(int keyFieldIndex, int fieldIndex) const
    {
        String const fmt = SL("select max(%s) from %s where %s = ?;");
        String const sql = (boost::format(fmt) % GetFieldName(fieldIndex) % GetTableName() % GetFieldName(keyFieldIndex)).str();
        return std::make_shared<SQLite::Statement>( *_db.get(), sql );
    }

    StatementPtr Table::BuildSelectMaxCommand(int keyFieldIndex1, int keyFieldIndex2, int fieldIndex) const
    {
        String const fmt = SL("select max(%s) from %s where %s = ? and %s = ?;");
        String const sql = (boost::format(fmt) % GetFieldName(fieldIndex) % GetTableName() % GetFieldName(keyFieldIndex1) % GetFieldName(keyFieldIndex2)).str();
        return std::make_shared<SQLite::Statement>( *_db.get(), sql );
    }

    StatementPtr Table::BuildSelectMaxCommand(int keyFieldIndex1, int keyFieldIndex2, int keyFieldIndex3, int fieldIndex) const
    {
        String const fmt = SL("select max(%s) from %s where %s = ? and %s = ? and %s = ?;");
        String const sql = (boost::format(fmt) % GetFieldName(fieldIndex) % GetTableName() % GetFieldName(keyFieldIndex1) % GetFieldName(keyFieldIndex2) % GetFieldName(keyFieldIndex3)).str();
        return std::make_shared<SQLite::Statement>( *_db.get(), sql );
    }

    StatementPtr Table::BuildUpdateCommand(int fieldIndex, int keyFieldIndex) const
    {
        String const fmt = SL("update %s set %s = ? where %s = ?;");
        String const sql = (boost::format(fmt) % GetTableName() % GetFieldName(fieldIndex) % GetFieldName(keyFieldIndex)).str();
        return std::make_shared<SQLite::Statement>( *_db.get(), sql );
    }

    StatementPtr Table::BuildUpdateCommand(int const *fieldIndexBegin, int const *fieldIndexEnd, int keyFieldIndex) const
    {
        assert(fieldIndexBegin != fieldIndexEnd && "Invalid field index iterators in Table::BuildUpdateCommand.");

        String sql = SL("update ") + GetTableName();

        for (int const *i = fieldIndexBegin, *n = fieldIndexEnd; i != n; ++i)
            sql += ((i == fieldIndexBegin) ? SL(" set ") : SL(", ")) + GetFieldName(*i) + SL(" = ?");

        sql += SL(" where ") + GetFieldName(keyFieldIndex) + SL(" = ?;");
        return std::make_shared<SQLite::Statement>( *_db.get(), sql );
    }

    StatementPtr Table::BuildUpdateCommand2(int fieldIndex, int keyFieldIndex1, int keyFieldIndex2) const
    {
        String const fmt = SL("update %s set %s = ? where %s = ? and %s = ?;");
        String const sql = (boost::format(fmt) % GetTableName() % GetFieldName(fieldIndex) % GetFieldName(keyFieldIndex1) % GetFieldName(keyFieldIndex2)).str();
        return std::make_shared<SQLite::Statement>( *_db.get(), sql );
    }

    void Table::GetMaxFor(StatementPtr &command, int selectFieldIndex, Int32 &value) const
    {
        if (!command)
            command = BuildSelectMaxCommand(selectFieldIndex);

        value = command->getColumn(0).getInt();
    }

    void Table::GetMaxFor(StatementPtr &command, int selectFieldIndex, Int64 &value) const
    {
        if (!command)
            command = BuildSelectMaxCommand(selectFieldIndex);

        value = command->getColumn(0).getInt64();
    }

    void Table::GetMaxFor(StatementPtr &command, int selectFieldIndex, String &value) const
    {
        if (!command)
            command = BuildSelectMaxCommand(selectFieldIndex);

        value = command->getColumn(0).getString();
    }

    /* Key-based "for" operations (one key) */
    void Table::DeleteAllFor(StatementPtr &command, int keyFieldIndex, Int64 keyValue)
    {
        if (!command)
            command = BuildDeleteCommand(keyFieldIndex);

        bind(command, 1, keyValue, Table::ID_SAFE);
        command->exec();
    }

    void Table::SelectAll(StatementPtr &query, int selectFieldIndex, Int64Vector  &values) const
    {
        values.clear();

        if (!query)
            query = BuildSelectQuery(selectFieldIndex);

        try
        {
            // Column indexes are zero-based.
            while (query->executeStep())
                values.push_back(query->getColumn(0).getInt64());

            query->reset();
        }
        catch (...)
        {
            query->reset();
            throw;
        }
    }

    void Table::SelectAllFor(StatementPtr &query, int keyFieldIndex, Int64 keyValue, int fieldIndex, Int64Vector &values, int sortFieldIndex, bool distinct) const
    {
        values.clear();

        if (!query)
            query = BuildSelectQuery(fieldIndex, keyFieldIndex, sortFieldIndex, distinct);

        // Parameter indexes are one-based.
        bind(query, 1, keyValue);

        try
        {
            // Column indexes are zero-based.
            while (query->executeStep())
                values.push_back(query->getColumn(0).getInt64());

            query->reset();
        }
        catch (...)
        {
            query->reset();
            throw;
        }
    }

    void Table::SelectAllNotNullFor(StatementPtr &query, int keyFieldIndex, Int64 keyValue, int fieldIndex, Int64Vector &values, int sortFieldIndex, bool distinct) const
    {
        values.clear();

        if (!query)
            query = BuildSelectNotNullQuery(fieldIndex, keyFieldIndex, sortFieldIndex, distinct);

        // Parameter indexes are one-based.
        bind(query, 1, keyValue);

        try
        {
            // Column indexes are zero-based.
            while (query->executeStep())
                values.push_back(query->getColumn(0).getInt64());

            query->reset();
        }
        catch (...)
        {
            query->reset();
            throw;
        }
    }

    void Table::SelectAllFor(StatementPtr &query, int keyFieldIndex, Int64 keyValue, int fieldIndex, Int32Vector &values, int sortFieldIndex, bool distinct) const
    {
        values.clear();

        if (!query)
            query = BuildSelectQuery(fieldIndex, keyFieldIndex, sortFieldIndex, distinct);

        // Parameter indexes are one-based.
        bind(query, 1, keyValue);

        try
        {
            // Column indexes are zero-based.
            while (query->executeStep())
                values.push_back(query->getColumn(0).getInt());

            query->reset();
        }
        catch (...)
        {
            query->reset();
            throw;
        }
    }

    void Table::SelectAllFor(StatementPtr &query, int keyFieldIndex, Int64 keyValue, int fieldIndex, StringVector &values, int sortFieldIndex, bool distinct) const
    {
        values.clear();

        if (!query)
            query = BuildSelectQuery(fieldIndex, keyFieldIndex, sortFieldIndex, distinct);

        // Parameter indexes are one-based.
        bind(query, 1, keyValue);

        try
        {
            // Column indexes are zero-based. The query's "Get" method
            // is used here to avoid a string copy per iteration.
            String value;

            while (query->executeStep())
            {
                value = query->getColumn(0).getString();
                values.push_back(value);
            }

            query->reset();
        }
        catch (...)
        {
            query->reset();
            throw;
        }
    }

    Int32 Table::GetCountFor(StatementPtr &command, int keyFieldIndex, Int64 keyValue) const
    {
        if (!command)
            command = BuildSelectCountCommand(keyFieldIndex);

        bind(command, 1, keyValue, Table::ID_SAFE);
        command->exec();
        return command->getColumn(0).getInt();
    }

    bool Table::GetExistsFor(StatementPtr &command, int keyFieldIndex, Int64 keyValue) const
    {
        if (!command)
            command = BuildSelectExistsCommand(keyFieldIndex);

        bind(command, 1, keyValue, Table::ID_SAFE);
        command->exec();
        return command->getColumn(0).getInt() != 0;
    }

    void Table::GetMaxFor(StatementPtr &command, int keyFieldIndex, Int64 keyValue, int selectFieldIndex, Int32 &value) const
    {
        if (!command)
            command = BuildSelectMaxCommand(keyFieldIndex, selectFieldIndex);

        bind(command, 1, keyValue, Table::ID_SAFE);
        command->exec();
        value = command->getColumn(0).getInt();
    }

    void Table::GetMaxFor(StatementPtr &command, int keyFieldIndex, Int64 keyValue, int selectFieldIndex, Int64 &value) const
    {
        if (!command)
            command = BuildSelectMaxCommand(keyFieldIndex, selectFieldIndex);

        bind(command, 1, keyValue, Table::ID_SAFE);
        command->exec();
        value = command->getColumn(0).getInt64();
    }

    void Table::GetMaxFor(StatementPtr &command, int keyFieldIndex, Int64 keyValue, int selectFieldIndex, String &value) const
    {
        if (!command)
            command = BuildSelectMaxCommand(keyFieldIndex, selectFieldIndex);

        bind(command, 1, keyValue, Table::ID_SAFE);
        command->exec();
        value = command->getColumn(0).getString();
    }

    void Table::SelectAllFor(StatementPtr &query, int keyFieldIndex, Int64 keyValue, int fieldIndex1, int fieldIndex2, Int32PairVector &values, int sortFieldIndex, bool distinct) const
    {
        values.clear();

        if (!query)
        {
            int const N = 2; int sels[N]; sels[0] = fieldIndex1; sels[1] = fieldIndex2;
            query = BuildSelectQuery(sels, sels + N, keyFieldIndex, sortFieldIndex, distinct);
        }

        // Parameter indexes are one-based.
        bind(query, 1, keyValue);

        try
        {
            // Column indexes are zero-based.
            while (query->executeStep())
                values.push_back(Int32Pair(query->getColumn(0).getInt(), query->getColumn(1).getInt()));

            query->reset();
        }
        catch (...)
        {
            query->reset();
            throw;
        }
    }

    void Table::SelectAllFor(StatementPtr &query, int keyFieldIndex, Int64 keyValue, int fieldIndex1, int fieldIndex2, Int64PairVector &values, int sortFieldIndex, bool distinct) const
    {
        values.clear();

        if (!query)
        {
            int const N = 2; int sels[N]; sels[0] = fieldIndex1; sels[1] = fieldIndex2;
            query = BuildSelectQuery(sels, sels + N, keyFieldIndex, sortFieldIndex, distinct);
        }

        // Parameter indexes are one-based.
        bind(query, 1, keyValue);

        try
        {
            // Column indexes are zero-based.
            while (query->executeStep())
                values.push_back(Int64Pair(query->getColumn(0).getInt64(), query->getColumn(1).getInt64()));

            query->reset();
        }
        catch (...)
        {
            query->reset();
            throw;
        }
    }

    void Table::SelectAllFor(StatementPtr &query, int keyFieldIndex, Int64 keyValue, int fieldIndex1, int fieldIndex2, Int64StringVector &values, int sortFieldIndex, bool distinct) const
    {
        values.clear();

        if (!query)
        {
            int const N = 2; int sels[N]; sels[0] = fieldIndex1; sels[1] = fieldIndex2;
            query = BuildSelectQuery(sels, sels + N, keyFieldIndex, sortFieldIndex, distinct);
        }

        // Parameter indexes are one-based.
        bind(query, 1, keyValue);

        try
        {
            // Column indexes are zero-based.
            while (query->executeStep())
                values.push_back(Int64String(query->getColumn(0).getInt64(), query->getColumn(1).getString()));

            query->reset();
        }
        catch (...)
        {
            query->reset();
            throw;
        }
    }

    /* Key-based "for" operations (two keys) */
    void Table::DeleteAllFor(StatementPtr &command, int keyFieldIndex1, Int64 keyValue1, int keyFieldIndex2, Int64 keyValue2)
    {
        if (!command)
            command = BuildDeleteCommand2(keyFieldIndex1, keyFieldIndex2);

        bind(command, 1, keyValue1, Table::ID_SAFE);
        bind(command, 2, keyValue2, Table::ID_SAFE);
        command->exec();
    }

    void Table::SelectAllFor(StatementPtr &query, int keyFieldIndex1, Int64 keyValue1, int keyFieldIndex2, Int64 keyValue2, int fieldIndex, Int64Vector &values, int sortFieldIndex, bool distinct) const
    {
        values.clear();

        if (!query)
            query = BuildSelectQuery2(fieldIndex, keyFieldIndex1, keyFieldIndex2, sortFieldIndex, distinct);

        // Parameter indexes are one-based.
        bind(query, 1, keyValue1);
        bind(query, 2, keyValue2);

        try
        {
            // Column indexes are zero-based.
            while (query->executeStep())
                values.push_back(query->getColumn(0).getInt64());

            query->reset();
        }
        catch (...)
        {
            query->reset();
            throw;
        }
    }

    void Table::SelectAllFor(StatementPtr &query, int keyFieldIndex1, Int64 keyValue1, int keyFieldIndex2, Int64 keyValue2, int fieldIndex, StringVector &values, int sortFieldIndex, bool distinct) const
    {
        values.clear();

        if (!query)
            query = BuildSelectQuery2(fieldIndex, keyFieldIndex1, keyFieldIndex2, sortFieldIndex, distinct);

        // Parameter indexes are one-based.
        bind(query, 1, keyValue1);
        bind(query, 2, keyValue2);

        try
        {
            // Column indexes are zero-based. The query's "Get" method
            // is used here to avoid a string copy per iteration.
            String value;

            while (query->executeStep())
            {
                value = query->getColumn(0).getString();
                values.push_back(value);
            }

            query->reset();
        }
        catch (...)
        {
            query->reset();
            throw;
        }
    }

    Int32 Table::GetCountFor(StatementPtr &command, int keyFieldIndex1, Int64 keyValue1, int keyFieldIndex2, Int64 keyValue2) const
    {
        if (!command)
            command = BuildSelectCountCommand2(keyFieldIndex1, keyFieldIndex2);

        bind(command, 1, keyValue1, Table::ID_SAFE);
        bind(command, 2, keyValue2, Table::ID_SAFE);
        command->exec();
        return command->getColumn(0).getInt();
    }

    bool Table::GetExistsFor(StatementPtr &command, int keyFieldIndex1, Int64 keyValue1, int keyFieldIndex2, Int64 keyValue2) const
    {
        if (!command)
            command = BuildSelectExistsCommand2(keyFieldIndex1, keyFieldIndex2);

        bind(command, 1, keyValue1, Table::ID_SAFE);
        bind(command, 2, keyValue2, Table::ID_SAFE);
        command->exec();
        return command->getColumn(0).getInt() != 0;
    }

    void Table::GetMaxFor(StatementPtr &command, int keyFieldIndex1, Int64 keyValue1, int keyFieldIndex2, Int64 keyValue2, int selectFieldIndex, Int32 &value) const
    {
        if (!command)
            command = BuildSelectMaxCommand(keyFieldIndex1, keyFieldIndex2, selectFieldIndex);

        bind(command, 1, keyValue1, Table::ID_SAFE);
        bind(command, 2, keyValue2, Table::ID_SAFE);
        command->exec();
        value = command->getColumn(0).getInt();
    }

    void Table::GetMaxFor(StatementPtr &command, int keyFieldIndex1, Int64 keyValue1, int keyFieldIndex2, Int64 keyValue2, int selectFieldIndex, Int64 &value) const
    {
        if (!command)
            command = BuildSelectMaxCommand(keyFieldIndex1, keyFieldIndex2, selectFieldIndex);

        bind(command, 1, keyValue1, Table::ID_SAFE);
        bind(command, 2, keyValue2, Table::ID_SAFE);
        command->exec();
        value = command->getColumn(0).getInt64();
    }

    void Table::GetMaxFor(StatementPtr &command, int keyFieldIndex1, Int64 keyValue1, int keyFieldIndex2, Int64 keyValue2, int selectFieldIndex, String &value) const
    {
        if (!command)
            command = BuildSelectMaxCommand(keyFieldIndex1, keyFieldIndex2, selectFieldIndex);

        bind(command, 1, keyValue1, Table::ID_SAFE);
        bind(command, 2, keyValue2, Table::ID_SAFE);
        value = command->getColumn(0).getString();
    }

    void Table::DeleteAllFor(StatementPtr &command, int keyFieldIndex1, Int64 keyValue1, int keyFieldIndex2, String const &keyValue2)
    {
        if (!command)
            command = BuildDeleteCommand2(keyFieldIndex1, keyFieldIndex2);

        bind(command, 1, keyValue1, Table::ID_SAFE);
        bind(command, 2, keyValue2);
        command->exec();
    }

    void Table::SelectAllFor(StatementPtr &query, int keyFieldIndex1, Int64 keyValue1, int keyFieldIndex2, String const &keyValue2, int fieldIndex, Int64Vector &values, int sortFieldIndex, bool distinct) const
    {
        values.clear();

        if (!query)
            query = BuildSelectQuery2(fieldIndex, keyFieldIndex1, keyFieldIndex2, sortFieldIndex, distinct);

        // Parameter indexes are one-based.
        bind(query, 1, keyValue1);
        bind(query, 2, keyValue2);

        try
        {
            // Column indexes are zero-based.
            while (query->executeStep())
                values.push_back(query->getColumn(0).getInt64());

            query->reset();
        }
        catch (...)
        {
            query->reset();
            throw;
        }
    }

    void Table::SelectAllFor(StatementPtr &query, int keyFieldIndex1, Int64 keyValue1, int keyFieldIndex2, String const &keyValue2, int fieldIndex, StringVector &values, int sortFieldIndex, bool distinct) const
    {
        values.clear();

        if (!query)
            query = BuildSelectQuery2(fieldIndex, keyFieldIndex1, keyFieldIndex2, sortFieldIndex, distinct);

        // Parameter indexes are one-based.
        bind(query, 1, keyValue1);
        bind(query, 2, keyValue2);

        try
        {
            // Column indexes are zero-based. The query's "Get" method
            // is used here to avoid a string copy per iteration.
            String value;

            while (query->executeStep())
            {
                value = query->getColumn(0).getString();
                values.push_back(value);
            }

            query->reset();
        }
        catch (...)
        {
            query->reset();
            throw;
        }
    }

    Int32 Table::GetCountFor(StatementPtr &command, int keyFieldIndex1, Int64 keyValue1, int keyFieldIndex2, String const &keyValue2) const
    {
        if (!command)
            command = BuildSelectCountCommand2(keyFieldIndex1, keyFieldIndex2);

        bind(command, 1, keyValue1, Table::ID_SAFE);
        bind(command, 2, keyValue2);
        command->exec();
        return command->getColumn(0).getInt();
    }

    bool Table::GetExistsFor(StatementPtr &command, int keyFieldIndex1, Int64 keyValue1, int keyFieldIndex2, String const &keyValue2) const
    {
        if (!command)
            command = BuildSelectExistsCommand2(keyFieldIndex1, keyFieldIndex2);

        bind(command, 1, keyValue1, Table::ID_SAFE);
        bind(command, 2, keyValue2);
        command->exec();
        return command->getColumn(0).getInt() != 0;
    }

    void Table::GetMaxFor(StatementPtr &command, int keyFieldIndex1, Int64 keyValue1, int keyFieldIndex2, String const &keyValue2, int selectFieldIndex, Int32 &value) const
    {
        if (!command)
            command = BuildSelectMaxCommand(keyFieldIndex1, keyFieldIndex2, selectFieldIndex);

        bind(command, 1, keyValue1, Table::ID_SAFE);
        bind(command, 2, keyValue2);
        command->exec();
        value = command->getColumn(0).getInt();
    }

    void Table::GetMaxFor(StatementPtr &command, int keyFieldIndex1, Int64 keyValue1, int keyFieldIndex2, String const &keyValue2, int selectFieldIndex, Int64 &value) const
    {
        if (!command)
            command = BuildSelectMaxCommand(keyFieldIndex1, keyFieldIndex2, selectFieldIndex);

        bind(command, 1, keyValue1, Table::ID_SAFE);
        bind(command, 2, keyValue2);
        command->exec();
        value = command->getColumn(0).getInt64();
    }

    void Table::GetMaxFor(StatementPtr &command, int keyFieldIndex1, Int64 keyValue1, int keyFieldIndex2, String const &keyValue2, int selectFieldIndex, String &value) const
    {
        if (!command)
            command = BuildSelectMaxCommand(keyFieldIndex1, keyFieldIndex2, selectFieldIndex);

        bind(command, 1, keyValue1, Table::ID_SAFE);
        bind(command, 2, keyValue2);
        value = command->getColumn(0).getString();
    }

    void Table::SelectAllFor(StatementPtr &query, int keyFieldIndex1, Int64 keyValue1, int keyFieldIndex2, Int64 keyValue2, int fieldIndex1, int fieldIndex2, Int64StringVector &values, int sortFieldIndex, bool distinct) const
    {
        values.clear();

        if (!query)
        {
            int const N = 2; int sels[N]; sels[0] = fieldIndex1;    sels[1] = fieldIndex2;
            int const M = 2; int keys[M]; keys[0] = keyFieldIndex1; keys[1] = keyFieldIndex2;
            query = BuildSelectQuery(sels, sels + N, keys, keys + M, sortFieldIndex, distinct);
        }

        // Parameter indexes are one-based.
        bind(query, 1, keyValue1);
        bind(query, 2, keyValue2);

        try
        {
            // Column indexes are zero-based.
            while (query->executeStep())
                values.push_back(Int64String(query->getColumn(0).getInt64(), query->getColumn(1).getString()));

            query->reset();
        }
        catch (...)
        {
            query->reset();
            throw;
        }
    }

    /* Key-based "for" operations (three keys) */
    void Table::DeleteAllFor(StatementPtr &command, int keyFieldIndex1, Int64 keyValue1, int keyFieldIndex2, Int64 keyValue2, int keyFieldIndex3, Int64 keyValue3)
    {
        if (!command)
            command = BuildDeleteCommand3(keyFieldIndex1, keyFieldIndex2, keyFieldIndex3);

        bind(command, 1, keyValue1, Table::ID_SAFE);
        bind(command, 2, keyValue2, Table::ID_SAFE);
        bind(command, 3, keyValue3, Table::ID_SAFE);
        command->exec();
    }

    void Table::SelectAllFor(StatementPtr &query, int keyFieldIndex1, Int64 keyValue1, int keyFieldIndex2, Int64 keyValue2, int keyFieldIndex3, Int64 keyValue3, int fieldIndex, Int64Vector &values, int sortFieldIndex, bool distinct) const
    {
        values.clear();

        if (!query)
            query = BuildSelectQuery3(fieldIndex, keyFieldIndex1, keyFieldIndex2, keyFieldIndex3, sortFieldIndex, distinct);

        // Parameter indexes are one-based.
        bind(query, 1, keyValue1);
        bind(query, 2, keyValue2);
        bind(query, 3, keyValue3);

        try
        {
            // Column indexes are zero-based.
            while (query->executeStep())
                values.push_back(query->getColumn(0).getInt64());

            query->reset();
        }
        catch (...)
        {
            query->reset();
            throw;
        }
    }

    void Table::SelectAllFor(StatementPtr &query, int keyFieldIndex1, Int64 keyValue1, int keyFieldIndex2, String const &keyValue2, int keyFieldIndex3, Int64 keyValue3, int fieldIndex, Int64Vector &values, int sortFieldIndex, bool distinct) const
    {
        values.clear();

        if (!query)
            query = BuildSelectQuery3(fieldIndex, keyFieldIndex1, keyFieldIndex2, keyFieldIndex3, sortFieldIndex, distinct);

        // Parameter indexes are one-based.
        bind(query, 1, keyValue1);
        bind(query, 2, keyValue2);
        bind(query, 3, keyValue3);

        try
        {
            // Column indexes are zero-based.
            while (query->executeStep())
                values.push_back(query->getColumn(0).getInt64());

            query->reset();
        }
        catch (...)
        {
            query->reset();
            throw;
        }
    }

    void Table::SelectAllFor(StatementPtr &query, int keyFieldIndex1, Int64 keyValue1, int keyFieldIndex2, Int64 keyValue2, int keyFieldIndex3, Int64 keyValue3, int fieldIndex, StringVector &values, int sortFieldIndex, bool distinct) const
    {
        values.clear();

        if (!query)
            query = BuildSelectQuery3(fieldIndex, keyFieldIndex1, keyFieldIndex2, keyFieldIndex3, sortFieldIndex, distinct);

        // Parameter indexes are one-based.
        bind(query, 1, keyValue1);
        bind(query, 2, keyValue2);
        bind(query, 3, keyValue3);

        try
        {
            // Column indexes are zero-based. The query's "Get" method
            // is used here to avoid a string copy per iteration.
            String value;

            while (query->executeStep())
            {
                value = query->getColumn(0).getString();
                values.push_back(value);
            }

            query->reset();
        }
        catch (...)
        {
            query->reset();
            throw;
        }
    }

    Int32 Table::GetCountFor(StatementPtr &command, int keyFieldIndex1, Int64 keyValue1, int keyFieldIndex2, Int64 keyValue2, int keyFieldIndex3, Int64 keyValue3) const
    {
        if (!command)
            command = BuildSelectCountCommand3(keyFieldIndex1, keyFieldIndex2, keyFieldIndex3);

        bind(command, 1, keyValue1, Table::ID_SAFE);
        bind(command, 2, keyValue2, Table::ID_SAFE);
        bind(command, 3, keyValue3, Table::ID_SAFE);
        command->exec();
        return command->getColumn(0).getInt();
    }

    bool Table::GetExistsFor(StatementPtr &command, int keyFieldIndex1, Int64 keyValue1, int keyFieldIndex2, Int64 keyValue2, int keyFieldIndex3, Int64 keyValue3) const
    {
        if (!command)
            command = BuildSelectExistsCommand3(keyFieldIndex1, keyFieldIndex2, keyFieldIndex3);

        bind(command, 1, keyValue1, Table::ID_SAFE);
        bind(command, 2, keyValue2, Table::ID_SAFE);
        bind(command, 3, keyValue3, Table::ID_SAFE);
        command->exec();
        return command->getColumn(0).getInt() != 0;
    }

    void Table::GetMaxFor(StatementPtr &command, int keyFieldIndex1, Int64 keyValue1, int keyFieldIndex2, Int64 keyValue2, int keyFieldIndex3, Int64 keyValue3, int selectFieldIndex, Int32 &value) const
    {
        if (!command)
            command = BuildSelectMaxCommand(keyFieldIndex1, keyFieldIndex2, keyFieldIndex3, selectFieldIndex);

        bind(command, 1, keyValue1, Table::ID_SAFE);
        bind(command, 2, keyValue2, Table::ID_SAFE);
        bind(command, 3, keyValue3, Table::ID_SAFE);
        command->exec();
        value = command->getColumn(0).getInt();
    }

    void Table::GetMaxFor(StatementPtr &command, int keyFieldIndex1, Int64 keyValue1, int keyFieldIndex2, Int64 keyValue2, int keyFieldIndex3, Int64 keyValue3, int selectFieldIndex, Int64 &value) const
    {
        if (!command)
            command = BuildSelectMaxCommand(keyFieldIndex1, keyFieldIndex2, keyFieldIndex3, selectFieldIndex);

        bind(command, 1, keyValue1, Table::ID_SAFE);
        bind(command, 2, keyValue2, Table::ID_SAFE);
        bind(command, 3, keyValue3, Table::ID_SAFE);
        command->exec();
        value = command->getColumn(0).getInt64();
    }

    void Table::GetMaxFor(StatementPtr &command, int keyFieldIndex1, Int64 keyValue1, int keyFieldIndex2, Int64 keyValue2, int keyFieldIndex3, Int64 keyValue3, int selectFieldIndex, String &value) const
    {
        if (!command)
            command = BuildSelectMaxCommand(keyFieldIndex1, keyFieldIndex2, keyFieldIndex3, selectFieldIndex);

        bind(command, 1, keyValue1, Table::ID_SAFE);
        bind(command, 2, keyValue2, Table::ID_SAFE);
        bind(command, 3, keyValue3, Table::ID_SAFE);
        command->exec();
        value = command->getColumn(0).getString();
    }

    void Table::SelectAllFor(StatementPtr &query, int keyFieldIndex1, Int64 keyValue1, int keyFieldIndex2, Int64 keyValue2, int keyFieldIndex3, Int64 keyValue3, int fieldIndex1, int fieldIndex2, Int64StringVector &values, int sortFieldIndex, bool distinct) const
    {
        values.clear();

        if (!query)
        {
            int const N = 2; int sels[N]; sels[0] = fieldIndex1;    sels[1] = fieldIndex2;
            int const M = 3; int keys[M]; keys[0] = keyFieldIndex1; keys[1] = keyFieldIndex2; keys[2] = keyFieldIndex3;
            query = BuildSelectQuery(sels, sels + N, keys, keys + M, sortFieldIndex, distinct);
        }

        // Parameter indexes are one-based.
        bind(query, 1, keyValue1);
        bind(query, 2, keyValue2);
        bind(query, 3, keyValue3);

        try
        {
            // Column indexes are zero-based.
            while (query->executeStep())
                values.push_back(Int64String(query->getColumn(0).getInt64(), query->getColumn(1).getString()));

            query->reset();
        }
        catch (...)
        {
            query->reset();
            throw;
        }
    }

    /* Binders */
    void Table::Bind(StatementPtr const &query, Int32 index, Int64 value) const
    {
        query->bind(index, value);
    }

    void Table::Bind(StatementPtr const &query, Int32 index, Int32 value) const
    {
        query->bind(index, value);
    }

    void Table::Bind(StatementPtr const &query, Int32 index, double value) const
    {
        query->bind(index, value);
    }

    void Table::Bind(StatementPtr const &query, Int32 index, String const &value) const
    {
        query->bind(index, value);
    }

    void Table::Bind(StatementPtr const &query, Int32 index, Binary const &value) const
    {
        query->bind(index, &value[0], value.size() );
    }

    void Table::Bind(StatementPtr const &command, Int32 index, Int64 value, int fieldIndex) const
    {
        if (value == ID_NULL && IsIntId(fieldIndex))
        {
            if (!IsNullOk(fieldIndex))
            {
                String fmt = SL("bind value cannot be zero for NOT NULL field \"%s\" in table \"%s\".");
                throw std::exception((boost::format(fmt) % GetFieldName(fieldIndex) % GetTableName()).str().c_str());
            }

            command->bind(index);
        }
        else
            command->bind(index, value);
    }

    void Table::Bind(StatementPtr const &command, Int32 index, Int64 value, bool) const
    {
        command->bind(index, value);
    }

    void Table::Exec(StatementPtr const &command) const
    {
        command->exec();
    }

    void Table::Exec(StatementPtr const &command, Int32 &value) const
    {
        command->executeStep();
        value = command->getColumn(0).getInt();
    }

    void Table::Exec(StatementPtr const &command, Int64 &value) const
    {
        command->executeStep();
        value = command->getColumn(0).getInt64();
    }

    void Table::Exec(StatementPtr const &command, double &value) const
    {
        command->executeStep();
        value = command->getColumn(0).getDouble();
    }

    void Table::Exec(StatementPtr const &command, String &value) const
    {
        command->executeStep();
        value = command->getColumn(0).getString();
    }

    void Table::Exec(StatementPtr const &command, Binary &value) const
    {
        command->executeStep();

        typedef Binary::value_type const Target;
        const void *source = command->getColumn(0).getBlob();
        int n = command->getColumn(0).size();
        Target *t = reinterpret_cast<Target *>(source);
        Binary(t, t + n).swap(value);
    }

    Int64 Table::GetLastInsertedRowId() const
    {
        return _db->getLastInsertRowid();
    }

    int Table::GetConstraintCount() const
    {
        return 0;
    }

    String Table::GetConstraintName(int index) const
    {
        // The default action is to return an empty string for
        // the name (assuming that the index is in range).
        if (index >= GetConstraintCount())
            throw std::exception(SL("Constraint index out of range in SQLiteImpl1::Table::GetConstraintName."));

        return String();
    }

    String Table::GetConstraintSql(int index) const
    {
        // The default action is to throw an exception for one
        // reason or another. If a table class has constraints,
        // it must implement this method.
        if (index >= GetConstraintCount())
            throw std::exception(SL("Constraint index out of range in SQLiteImpl1::Table::GetConstraintSql."));

        throw std::exception(SL("Constraint cannot be returned by SQLiteImpl1::Table::GetConstraintSql."));
    }

    void Table::Create()
    {
        String sql = SL("create table ") + GetTableName() + SL("(");

        for (int i = 0, n = GetFieldCount(); i != n; ++i)
        {
            int    fieldBits = GetFieldBits(i);
            String fieldSql  = GetFieldSql(i);

            sql += ((i == 0) ? SL("") : SL(", ")) + GetFieldName(i);

            if (IsBinary(i))
                sql += SL(" blob");
            else if (IsString(i))
                sql += SL(" text");
            else if (IsFloating(i))
                sql += SL(" real");
            else
                sql += SL(" integer");

            if (IsPKeyInc(i))
                sql += SL(" primary key asc autoincrement");
            else if (IsPKeyAsc(i))
                sql += SL(" primary key asc");
            else if (IsUnique(i))
                sql += SL(" unique");

            if (!IsPKey(i))
                sql += (IsNullOk(i)) ? SL(" null") : SL(" not null");

            if (IsString(i))
                if (IsNoCase(i))
                    sql += SL(" collate nocase");

            if (!fieldSql.empty())
                sql += SL(" ") + fieldSql;
        }

        for (int i = 0, n = GetConstraintCount(); i != n; ++i)
        {
            String name = GetConstraintName(i);
            sql += SL(",") + ((name.empty()) ? String() : SL(" constraint ") + name);
            sql += SL(" ") + GetConstraintSql(i);
        }

        sql += SL(");");

        _db->exec(sql);
        Fill();
        Index();
    }

    void Table::Fill()
    {
        // The default action is to not populate the table at
        // creation.
    }

    void Table::Index()
    {
        // The default action is to not index the table at
        // creation.
    }

    void Table::Verify() const
    {
        // The default action is to not validate the table.
    }

    String Table::From() const
    {
        return SL(" from ") + GetTableName();
    }

    String Table::From(String const &asTable) const
    {
        assert(!asTable.empty() && "Empty as-table in SQLiteImpl1::Table::From.");
        return (boost::format(SL(" from %s as %s")) % GetTableName() % asTable).str();
    }

    String Table::Join(String const &asTable) const
    {
        assert(!asTable.empty() && "Empty as-table in SQLiteImpl1::Table::Join.");
        return (boost::format(SL(" join %s as %s")) % GetTableName() % asTable).str();
    }

    String Table::Join(String const &asTable, int fieldIndex) const
    {
        assert(!asTable.empty() && "Empty as-table in SQLiteImpl1::Table::Join.");
        return (boost::format(SL(" join %s as %s on %s.%s")) % GetTableName() % asTable % asTable % GetFieldName(fieldIndex)).str();
    }

    String Table::Name(int fieldIndex, bool comma) const
    {
        return (boost::format(SL("%s %s")) % ((comma) ? SL(",") : SL("")) % GetFieldName(fieldIndex)).str();
    }

    String Table::Name(String const &asTable, int fieldIndex, bool comma) const
    {
        assert(!asTable.empty() && "Empty as-table in SQLiteImpl1::Table::Name.");
        return (boost::format(SL("%s %s.%s")) % ((comma) ? SL(",") : SL("")) % asTable % GetFieldName(fieldIndex)).str();
    }

    String Table::IfNull(String const &asTable, int fieldIndex, Int32 value, bool comma) const
    {
        assert(!asTable.empty() && "Empty as-table in SQLiteImpl1::Table::IfNull.");
        return (boost::format(SL("%s ifnull(%s.%s, %d)")) % ((comma) ? SL(",") : SL("")) % asTable % GetFieldName(fieldIndex) % value).str();
    }

    String Table::IfNull(String const &asTable, int fieldIndex, Int64 value, bool comma) const
    {
        assert(!asTable.empty() && "Empty as-table in SQLiteImpl1::Table::IfNull.");
        return (boost::format(SL("%s ifnull(%s.%s, %d)")) % ((comma) ? SL(",") : SL("")) % asTable % GetFieldName(fieldIndex) % value).str();
    }
#pragma message("TODO: Investigate conversion from double to string.")
    String Table::IfNull(String const &asTable, int fieldIndex, double value, bool comma) const
    {
        assert(!asTable.empty() && "Empty as-table in SQLiteImpl1::Table::IfNull.");
        return (boost::format(SL("%s ifnull(%s.%s, %.0g)")) % ((comma) ? SL(",") : SL("")) % asTable % GetFieldName(fieldIndex) % value).str();
    }

    String Table::IfNull(String const &asTable, int fieldIndex, String const &value, bool comma) const
    {
        assert(!asTable.empty() && "Empty as-table in SQLiteImpl1::Table::IfNull.");
        return (boost::format(SL("%s ifnull(%s.%s, %s)")) % ((comma) ? SL(",") : SL("")) % asTable % GetFieldName(fieldIndex) % Quote(value)).str();
    }

    String Table::IfNullId(String const &asTable, int fieldIndex, bool comma) const
    {
        assert(!asTable.empty() && "Empty as-table in SQLiteImpl1::Table::IfNullId.");
        return IfNull(asTable, fieldIndex, Table::ID_NULL, comma);
    }

    String Table::IfNullBlob(String const &asTable, int fieldIndex, bool comma) const
    {
        assert(!asTable.empty() && "Empty as-table in SQLiteImpl1::Table::IfNullBlob.");
        return (boost::format(SL("%s ifnull(%s.%s, zeroblob(0))")) % ((comma) ? SL(",") : SL("")) % asTable % GetFieldName(fieldIndex)).str();
    }

    String Table::IfNullText(String const &asTable, int fieldIndex, bool comma) const
    {
        assert(!asTable.empty() && "Empty as-table in SQLiteImpl1::Table::IfNullText.");
        return IfNull(asTable, fieldIndex, String(), comma);
    }

    namespace
    {
        template <class Vector>
            String InValuesExpression(String const &fieldName, Vector const &values)
            {
                // This function returns an equality expression for a field
                // and an integral value list in one of two forms. For small
                // lists, the form is "(x = 7 or x = 4 or x = 9 or x = 2").
                // For big lists, the form is "(x in (7,4,9,2,3,1,8,...))".
                if (values.empty())
                    throw std::exception(SL("Empty in-values list."));

                size_t const BIG = 32; // Better than 8, 20, 50, and 100.
                String s;

                if (values.size() >= BIG)
                {
                    s += fieldName + SL(" in (") + boost::lexical_cast<String>(values.front());
                    for (Vector::const_iterator i = values.begin() + 1, n = values.end(); i != n; ++i)
                        s += SL(",") + boost::lexical_cast<String>(*i);
                    s += SL(")");
                }
                else
                {
                    s += fieldName + SL(" = ") + boost::lexical_cast<String>(values.front());
                    for (Vector::const_iterator i = values.begin() + 1, n = values.end(); i != n; ++i)
                        s += SL(" or ") + fieldName + SL(" = ") + boost::lexical_cast<String>(*i);
                }

                return SL("(") + s + SL(")");
            }
    }

    String Table::InValues(int fieldIndex, Int32Vector  const &values) const
    {
        return InValuesExpression(GetFieldName(fieldIndex), values);
    }

    String Table::InValues(int fieldIndex, Int64Vector  const &values) const
    {
        return InValuesExpression(GetFieldName(fieldIndex), values);
    }

    String Table::InValues(String const &asTable, int fieldIndex, Int32Vector  const &values) const
    {
        return InValuesExpression(asTable + SL(".") + GetFieldName(fieldIndex), values);
    }

    String Table::InValues(String const &asTable, int fieldIndex, Int64Vector  const &values) const
    {
        return InValuesExpression(asTable + SL(".") + GetFieldName(fieldIndex), values);
    }

    /*static*/String Table::Quote(String const &s)
    {
        return SL("'") + boost::replace_all_copy(s, SL("'"), SL("''")) + SL("'");
    }
}
}
