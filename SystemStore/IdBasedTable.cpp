/*****************************************************************************
 * IdBasedTable.cpp -- $Id$
 *
 * Purpose
 *   Implements the IdBasedTable class.
 *
 * Indentation
 *   Four characters. No tabs!
 *
 * Modifications
 *   2012-03-26 (MM) Added a copy method.
 *   2012-03-02 (MM) Changed to a new storage organization.
 *   2010-12-11 (MM) Changed to new bind scheme.
 *   2010-12-10 (MM) Added generic Select/Update methods.
 *   2010-12-06 (MM) Created.
 *
 * Copyright (c) 2010-2012 Xiao Shengguang.  All rights reserved.
 ****************************************************************************/

#include "Common/BaseDefs.h"
#include "IdBasedTable.h"

namespace AOI
{
namespace SystemStore
{
    /******************************
    * IdBasedTable implementation *
    ******************************/
    StatementPtr IdBasedTable::BuildSelectCommandWithId(int valueFieldIndex) const
    {
        return BuildSelectCommand(valueFieldIndex, GetFieldIndexOfId());
    }

    StatementPtr IdBasedTable::BuildSelectQueryWithId(int const *valueFieldIndexBegin, int const *valueFieldIndexEnd) const
    {
        return BuildSelectQuery(valueFieldIndexBegin, valueFieldIndexEnd, GetFieldIndexOfId());
    }

    StatementPtr IdBasedTable::BuildUpdateCommandWithId(int valueFieldIndex) const
    {
        return BuildUpdateCommand(valueFieldIndex, GetFieldIndexOfId());
    }

    StatementPtr IdBasedTable::BuildUpdateCommandWithId(int const *valueFieldIndexBegin, int const *valueFieldIndexEnd) const
    {
        return BuildUpdateCommand(valueFieldIndexBegin, valueFieldIndexEnd, GetFieldIndexOfId());
    }

    Int64 IdBasedTable::Copy(Int64 id)
    {
        if (!this->copcmd)
        {
            String const tn   = GetTableName();
            String       sql1 = SL("insert into ") + tn + SL(" (");
            String       sql2 = SL(") select ");
            int    const idfi = GetFieldIndexOfId();

            for (int i = 0, k = 0, n = GetFieldCount(); i != n; ++i)
                if (i != idfi)
                {
                    String const field = ((++k == 1) ? SL("") : SL(", ")) + GetFieldName(i);
                    sql1 += field;
                    sql2 += field;
                }

            String const sql = sql1 + sql2 + SL(" from ") + tn + SL(" where ") + GetFieldName(idfi) + SL(" = ?;");
            this->copcmd = std::make_shared<SQLite::Statement>( *GetDatabase().get(), sql );
        }

        Bind(this->copcmd, 1, id, ID_SAFE);
        Exec(this->copcmd);

        return GetLastInsertedRowId();
    }

    void IdBasedTable::Delete(Int64 id)
    {
        if (!this->delcmd)
            this->delcmd = BuildDeleteCommand(GetFieldIndexOfId());

        Bind(this->delcmd, 1, id, ID_SAFE);
        Exec(this->delcmd);
    }

    void IdBasedTable::Select(Int64 id, StatementPtr &select, int fieldIndex, Int32 &value) const
    {
        if (!select)
            select = BuildSelectCommand(fieldIndex, GetFieldIndexOfId());

        Bind(select, 1, id, ID_SAFE);
        Exec(select, value);
    }

    void IdBasedTable::Select(Int64 id, StatementPtr &select, int fieldIndex, Int64 &value) const
    {
        if (!select)
            select = BuildSelectCommand(fieldIndex, GetFieldIndexOfId());

        Bind(select, 1, id, ID_SAFE);
        Exec(select, value);
    }

    void IdBasedTable::Select(Int64 id, StatementPtr &select, int fieldIndex, double &value) const
    {
        if (!select)
            select = BuildSelectCommand(fieldIndex, GetFieldIndexOfId());

        Bind(select, 1, id, ID_SAFE);
        Exec(select, value);
    }

    void IdBasedTable::Select(Int64 id, StatementPtr &select, int fieldIndex, String &value) const
    {
        if (!select)
            select = BuildSelectCommand(fieldIndex, GetFieldIndexOfId());

        Bind(select, 1, id, ID_SAFE);
        Exec(select, value);
    }

    void IdBasedTable::Select(Int64 id, StatementPtr &select, int fieldIndex, Binary &value) const
    {
        if (!select)
            select = BuildSelectCommand(fieldIndex, GetFieldIndexOfId());

        Bind(select, 1, id, ID_SAFE);
        Exec(select, value);
    }

    void IdBasedTable::Select(Int64 id, StatementPtr &select, int fieldIndex, Enum::YesNo &value) const
    {
        Int32 v;
        Select(id, select, fieldIndex, v);
        value = Enum::YesNo(v);
    }

    void IdBasedTable::Update(Int64 id, StatementPtr &update, int fieldIndex, Int32 value)
    {
        if (!update)
            update = BuildUpdateCommand(fieldIndex, GetFieldIndexOfId());

        Bind(update, 1, value);
        Bind(update, 2, id, ID_SAFE);
        Exec(update);
    }

    void IdBasedTable::Update(Int64 id, StatementPtr &update, int fieldIndex, Int64 value)
    {
        if (!update)
            update = BuildUpdateCommand(fieldIndex, GetFieldIndexOfId());

        Bind(update, 1, value, fieldIndex); // Use the special binder for Int64 updates!
        Bind(update, 2, id, ID_SAFE);
        Exec(update);
    }

    void IdBasedTable::Update(Int64 id, StatementPtr &update, int fieldIndex, double value)
    {
        if (!update)
            update = BuildUpdateCommand(fieldIndex, GetFieldIndexOfId());

        Bind(update, 1, value);
        Bind(update, 2, id, ID_SAFE);
        Exec(update);
    }

    void IdBasedTable::Update(Int64 id, StatementPtr &update, int fieldIndex, String const &value)
    {
        if (!update)
            update = BuildUpdateCommand(fieldIndex, GetFieldIndexOfId());

        Bind(update, 1, value);
        Bind(update, 2, id, ID_SAFE);
        Exec(update);
    }

    void IdBasedTable::Update(Int64 id, StatementPtr &update, int fieldIndex, Binary const &value)
    {
        if (!update)
            update = BuildUpdateCommand(fieldIndex, GetFieldIndexOfId());

        Bind(update, 1, value);
        Bind(update, 2, id, ID_SAFE);
        Exec(update);
    }
}
}
