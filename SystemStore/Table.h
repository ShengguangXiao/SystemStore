#ifndef KEYSIGHT_BBOX_ENGINE_CORE_SQLITEIMPL1_TABLE_H
#define KEYSIGHT_BBOX_ENGINE_CORE_SQLITEIMPL1_TABLE_H
/*****************************************************************************
 * Table.h -- $Id$
 *
 * Purpose
 *   Declares the Table class which is the base class for classes that
 *   contain information about database tables.
 *
 * Indentation
 *   Four characters. No tabs!
 *
 * Modifications
 *   2015-06-14 (MM) Added GetMaxFor methods.
 *   2012-05-29 (MM) Added a BuildUpdateCommand2 method.
 *   2012-05-14 (MM) Added more shorthand "For" methods.
 *   2012-04-10 (MM) Added shorthand methods for comparing a field against
 *                   a list of values.
 *   2012-03-05 (MM) Added more shorthand "For" methods and bit-unique.
 *   2012-02-23 (MM) Added some shorthand "For" methods.
 *   2011-11-30 (MM) Added two more if-null methods.
 *   2011-08-08 (YB) Added a BuildDeleteCommand method.
 *   2011-05-10 (MM) Changed the additional-includes scheme.
 *   2011-03-16 (MM) Added the GetLastInsertedRowId method.
 *   2011-03-11 (MM) Added an indexer method (Index).
 *   2010-12-16 (MM) Added SQL builders, binders, and another Join.
 *   2010-12-05 (MM) Created.
 *
 * Copyright (c) 2010-2012, 2015 Keysight Technologies, Inc.  All rights reserved.
 ****************************************************************************/

#include "Common/BaseDefs.h"
#include <SQLiteCpp/SQLiteCpp.h>

namespace AOI
{
namespace SystemStore
{
    class Table;

    using StatementPtr  = std::shared_ptr<SQLite::Statement> ;
    using TablePtr      = std::shared_ptr<Table>;
    using TableConstPtr = std::shared_ptr<Table const>;
    using DatabasePtr   = std::shared_ptr<SQLite::Database>;    
    #define ToInt32(param)      (static_cast<Int32>(param))

    class Table: private Uncopyable
    {
        DatabasePtr mutable _db;
    protected:
        explicit Table(DatabasePtr const &db);

        static int  const UNSORTED = -1;
        static bool const DISTINCT = true;

        // Sql: "delete from <table> where <key> = ?;".
        // Bind(1, <keyFieldValue>);
        StatementPtr BuildDeleteCommand(int keyFieldIndex) const;

        // Sql: "delete from <table> where <key1> = ? and <key2> = ?;".
        // Bind(1, <keyFieldValue1>);
        // Bind(2, <keyFieldValue2>);
        StatementPtr BuildDeleteCommand2(int keyFieldIndex1, int keyFieldIndex2) const;

        // Sql: "delete from <table> where <key1> = ? and <key2> = ? and <key3> = ?;".
        // Bind(1, <keyFieldValue1>);
        // Bind(2, <keyFieldValue2>);
        // Bind(3, <keyFieldValue3>);
        StatementPtr BuildDeleteCommand3(int keyFieldIndex1, int keyFieldIndex2, int keyFieldIndex3) const;

        // Sql: "delete from <table> where <f1> = <v1>, ..., <f2> = <vn>;".
        // Bind(1,   <fieldValue>);
        // Bind(..., <fieldValue>);
        // Bind(n,   <fieldValue>);   /* n = fieldIndexEnd - fieldIndexBegin; */
        StatementPtr BuildDeleteCommand(int const *fieldIndexBegin, int const *fieldIndexEnd) const;

        // Sql: "insert into <table> (<f1>, ..., <fn>) values (<v1>, ..., <vn>);".
        // Bind(1,   <fieldValue>);
        // Bind(..., <fieldValue>);
        // Bind(n,   <fieldValue>);   /* n = fieldIndexEnd - fieldIndexBegin; */
        StatementPtr BuildInsertCommand(int const *fieldIndexBegin, int const *fieldIndexEnd) const;        

        // Sql: "select <f1> from <table> where <key> = ?;".
        // Bind(1, <keyFieldValue>);
        StatementPtr BuildSelectCommand(int fieldIndex, int keyFieldIndex) const;

        // Sql: "select <f1> from <table> where <key1> = ? and <key2> = ?;".
        // Bind(1, <keyFieldValue>);
        StatementPtr BuildSelectCommand(int fieldIndex, int keyFieldIndex1, int keyFieldIndex2) const;

        // Sql: "select <f1> from <table>;".
        StatementPtr   BuildSelectQuery(int fieldIndex) const;

        // Sql: "select <f1> from <table> where <key> = ?;".
        // Sql: "select <f1> from <table> where <key> = ? order by <sf1>;".
        // Bind(1, <keyFieldValue>);
        StatementPtr   BuildSelectQuery(int fieldIndex, int keyFieldIndex, int sortFieldIndex=UNSORTED, bool distinct=!DISTINCT) const;

        // Sql: "select <f1> from <table> where <key> = ? and <f1> is not null;".
        // Sql: "select <f1> from <table> where <key> = ? and <f1> is not null order by <sf1>;".
        // Bind(1, <keyFieldValue>);
        StatementPtr   BuildSelectNotNullQuery(int fieldIndex, int keyFieldIndex, int sortFieldIndex=UNSORTED, bool distinct=!DISTINCT) const;

        // Sql: "select <f1> from <table> where <key1> = ? and <key2> = ?;".
        // Sql: "select <f1> from <table> where <key1> = ? and <key2> = ? order by <sf1>;".
        // Bind(1, <keyFieldValue1>);
        // Bind(2, <keyFieldValue2>);
        StatementPtr   BuildSelectQuery2(int fieldIndex, int keyFieldIndex1, int keyFieldIndex2, int sortFieldIndex=UNSORTED, bool distinct=!DISTINCT) const;

        // Sql: "select <f1> from <table> where <key1> = ? and <key2> = ? and <key3> = ?;".
        // Sql: "select <f1> from <table> where <key1> = ? and <key2> = ? and <key3> = ? order by <sf1>;".
        // Bind(1, <keyFieldValue1>);
        // Bind(2, <keyFieldValue2>);
        StatementPtr   BuildSelectQuery3(int fieldIndex, int keyFieldIndex1, int keyFieldIndex2, int keyFieldIndex3, int sortFieldIndex=UNSORTED, bool distinct=!DISTINCT) const;

        // Sql: "select <f1>, ..., <fn> from <table> where <key> = ?;".
        // Sql: "select <f1>, ..., <fn> from <table> where <key> = ? order by <sf1>;".
        // Bind(1, <keyFieldValue>);
        StatementPtr   BuildSelectQuery(int const *fieldIndexBegin, int const *fieldIndexEnd, int keyFieldIndex, int sortFieldIndex=UNSORTED, bool distinct=!DISTINCT) const;

        // Sql: "select <f1>, ..., <fn> from <table> where <k1> = ? ... and <kn> = ?;".
        // Sql: "select <f1>, ..., <fn> from <table> where <k1> = ? ... and <kn> = ? order by <sf1>;".
        // Bind(1,   <keyFieldValue>);
        // Bind(..., <keyFieldValue>);
        // Bind(n,   <keyFieldValue>); /* n = keyFieldIndexEnd - keyFieldIndexBegin */
        StatementPtr   BuildSelectQuery(int const *fieldIndexBegin, int const *fieldIndexEnd, int const *keyFieldIndexBegin, int const *keyFieldIndexEnd, int sortFieldIndex=UNSORTED, bool distinct=!DISTINCT) const;

        // Sql: "select count(*) from <table>;".
        StatementPtr BuildSelectCountCommand() const;

        // Sql: "select count(*) from <table> where <key> = ?;".
        // Bind(1, <keyFieldValue>);
        StatementPtr BuildSelectCountCommand(int keyFieldIndex) const;

        // Sql: "select count(*) from <table> where <key1> = ? and <key2> = ?;".
        // Bind(1, <keyFieldValue1>);
        // Bind(2, <keyFieldValue2>);
        StatementPtr BuildSelectCountCommand2(int keyFieldIndex1, int keyFieldIndex2) const;

        // Sql: "select count(*) from <table> where <key1> = ? and <key2> = ? and <key3> = ?;".
        // Bind(1, <keyFieldValue1>);
        // Bind(2, <keyFieldValue2>);
        // Bind(3, <keyFieldValue3>);
        StatementPtr BuildSelectCountCommand3(int keyFieldIndex1, int keyFieldIndex2, int keyFieldIndex3) const;

        // Sql: "select exists(select * from <table> where <key> = ?);".
        // Bind(1, <keyFieldValue>);
        StatementPtr BuildSelectExistsCommand(int keyFieldIndex) const;

        // Sql: "select exists(select * from <table> where <key1> = ? and <key2> = ?);".
        // Bind(1, <keyFieldValue1>);
        // Bind(2, <keyFieldValue2>);
        StatementPtr BuildSelectExistsCommand2(int keyFieldIndex1, int keyFieldIndex2) const;

        // Sql: "select exists(select * from <table> where <key1> = ? and <key2> = ? and <key3> = ?);".
        // Bind(1, <keyFieldValue1>);
        // Bind(2, <keyFieldValue2>);
        // Bind(3, <keyFieldValue3>);
        StatementPtr BuildSelectExistsCommand3(int keyFieldIndex1, int keyFieldIndex2, int keyFieldIndex3) const;

        // Sql: "select max(f1) from <table>;".
        StatementPtr BuildSelectMaxCommand(int fieldIndex) const;

        // Sql: "select max(f1) from <table> where <key> = ?;".
        // Bind(1, <keyFieldValue>);
        StatementPtr BuildSelectMaxCommand(int keyFieldIndex, int fieldIndex) const;

        // Sql: "select max(f1) from <table> where <key1> = ? and <key2> = ?;".
        // Bind(1, <keyFieldValue1>);
        // Bind(2, <keyFieldValue2>);
        StatementPtr BuildSelectMaxCommand(int keyFieldIndex1, int keyFieldIndex2, int fieldIndex) const;

        // Sql: "select max(f1) from <table> where <key1> = ? and <key2> = ? and <key3> = ?;".
        // Bind(1, <keyFieldValue1>);
        // Bind(2, <keyFieldValue2>);
        // Bind(3, <keyFieldValue3>);
        StatementPtr BuildSelectMaxCommand(int keyFieldIndex1, int keyFieldIndex2, int keyFieldIndex3, int fieldIndex) const;

        // Sql: "update <table> set <f1> = ? where <key> = ?;".
        // Bind(1, <fieldValue>);
        // Bind(2, <keyFieldValue>);
        StatementPtr BuildUpdateCommand(int fieldIndex, int keyFieldIndex) const;

        // Sql: "update <table> set <f1> = ?, ..., <fn> = ? where <key> = ?;".
        // Bind(1,   <fieldValue>);
        // Bind(..., <fieldValue>);
        // Bind(n,   <fieldValue>);   /* n = fieldIndexEnd - fieldIndexBegin; */
        // Bind(n+1, <keyFieldValue>);
        StatementPtr BuildUpdateCommand(int const *fieldIndexBegin, int const *fieldIndexEnd, int keyFieldIndex) const;

        // Sql: "update <table> set <f1> = ? where <key1> = ? and <key2> = ?;".
        // Bind(1, <fieldValue>);
        // Bind(2, <keyFieldValue1>);
        // Bind(3, <keyFieldValue2>);
        StatementPtr BuildUpdateCommand2(int fieldIndex, int keyFieldIndex1, int keyFieldIndex2) const;

        // Shorthand methods on entire tables. The only recommended use for
        // GetForMax is to retrieve the Int64 max of a table's primary id.
        void  GetMaxFor   (StatementPtr &, int selectFieldIndex, Int32  &) const;
        void  GetMaxFor   (StatementPtr &, int selectFieldIndex, Int64  &) const;
        void  GetMaxFor   (StatementPtr &, int selectFieldIndex, String &) const;

        // Shorthand methods intended primarily for use on id fields.
        void  DeleteAllFor(StatementPtr &, int keyFieldIndex, Int64 keyValue);
        void  SelectAllFor(StatementPtr   &, int keyFieldIndex, Int64 keyValue, int selectFieldIndex, Int32Vector  &values, int sortFieldIndex=UNSORTED, bool distinct=!DISTINCT) const;
        void  SelectAll   (StatementPtr   &, int selectFieldIndex, Int64Vector  &values) const;
        void  SelectAllFor(StatementPtr   &, int keyFieldIndex, Int64 keyValue, int selectFieldIndex, Int64Vector  &values, int sortFieldIndex=UNSORTED, bool distinct=!DISTINCT) const;
        void  SelectAllFor(StatementPtr   &, int keyFieldIndex, Int64 keyValue, int selectFieldIndex, StringVector &values, int sortFieldIndex=UNSORTED, bool distinct=!DISTINCT) const;
        Int32 GetCountFor (StatementPtr &, int keyFieldIndex, Int64 keyValue) const;
        bool  GetExistsFor(StatementPtr &, int keyFieldIndex, Int64 keyValue) const;
        void  GetMaxFor   (StatementPtr &, int keyFieldIndex, Int64 keyValue, int selectFieldIndex, Int32  &) const;
        void  GetMaxFor   (StatementPtr &, int keyFieldIndex, Int64 keyValue, int selectFieldIndex, Int64  &) const;
        void  GetMaxFor   (StatementPtr &, int keyFieldIndex, Int64 keyValue, int selectFieldIndex, String &) const;

        void  SelectAllNotNullFor(StatementPtr &, int keyFieldIndex, Int64 keyValue, int selectFieldIndex, Int64Vector  &values, int sortFieldIndex=UNSORTED, bool distinct=!DISTINCT) const;

        // Shorthand methods intended primarily for use on id fields. These
        // versions produce results made up of pairs.
        void  SelectAllFor(StatementPtr   &, int keyFieldIndex, Int64 keyValue, int selectFieldIndex1, int selectFieldIndex2, Int32PairVector   &values, int sortFieldIndex=UNSORTED, bool distinct=!DISTINCT) const;
        void  SelectAllFor(StatementPtr   &, int keyFieldIndex, Int64 keyValue, int selectFieldIndex1, int selectFieldIndex2, Int64PairVector   &values, int sortFieldIndex=UNSORTED, bool distinct=!DISTINCT) const;
        void  SelectAllFor(StatementPtr   &, int keyFieldIndex, Int64 keyValue, int selectFieldIndex1, int selectFieldIndex2, Int64StringVector &values, int sortFieldIndex=UNSORTED, bool distinct=!DISTINCT) const;

        // Shorthand methods intended primarily for use on id fields with
        // an associated qualifier field (e.g., a type or name).
        void  DeleteAllFor(StatementPtr &, int keyFieldIndex1, Int64 keyValue1, int keyFieldIndex2, Int64 keyValue2);
        void  SelectAllFor(StatementPtr   &, int keyFieldIndex1, Int64 keyValue1, int keyFieldIndex2, Int64 keyValue2, int selectFieldIndex, Int64Vector  &values, int sortFieldIndex=UNSORTED, bool distinct=!DISTINCT) const;
        void  SelectAllFor(StatementPtr   &, int keyFieldIndex1, Int64 keyValue1, int keyFieldIndex2, Int64 keyValue2, int selectFieldIndex, StringVector &values, int sortFieldIndex=UNSORTED, bool distinct=!DISTINCT) const;
        Int32 GetCountFor (StatementPtr &, int keyFieldIndex1, Int64 keyValue1, int keyFieldIndex2, Int64 keyValue2) const;
        bool  GetExistsFor(StatementPtr &, int keyFieldIndex1, Int64 keyValue1, int keyFieldIndex2, Int64 keyValue2) const;
        void  GetMaxFor   (StatementPtr &, int keyFieldIndex1, Int64 keyValue1, int keyFieldIndex2, Int64 keyValue2, int selectFieldIndex, Int32  &) const;
        void  GetMaxFor   (StatementPtr &, int keyFieldIndex1, Int64 keyValue1, int keyFieldIndex2, Int64 keyValue2, int selectFieldIndex, Int64  &) const;
        void  GetMaxFor   (StatementPtr &, int keyFieldIndex1, Int64 keyValue1, int keyFieldIndex2, Int64 keyValue2, int selectFieldIndex, String &) const;

        void  DeleteAllFor(StatementPtr &, int keyFieldIndex1, Int64 keyValue1, int keyFieldIndex2, String const &keyValue2);
        void  SelectAllFor(StatementPtr   &, int keyFieldIndex1, Int64 keyValue1, int keyFieldIndex2, String const &keyValue2, int selectFieldIndex, Int64Vector  &values, int sortFieldIndex=UNSORTED, bool distinct=!DISTINCT) const;
        void  SelectAllFor(StatementPtr   &, int keyFieldIndex1, Int64 keyValue1, int keyFieldIndex2, String const &keyValue2, int selectFieldIndex, StringVector &values, int sortFieldIndex=UNSORTED, bool distinct=!DISTINCT) const;
        Int32 GetCountFor (StatementPtr &, int keyFieldIndex1, Int64 keyValue1, int keyFieldIndex2, String const &keyValue2) const;
        bool  GetExistsFor(StatementPtr &, int keyFieldIndex1, Int64 keyValue1, int keyFieldIndex2, String const &keyValue2) const;
        void  GetMaxFor   (StatementPtr &, int keyFieldIndex1, Int64 keyValue1, int keyFieldIndex2, String const &keyValue2, int selectFieldIndex, Int32  &) const;
        void  GetMaxFor   (StatementPtr &, int keyFieldIndex1, Int64 keyValue1, int keyFieldIndex2, String const &keyValue2, int selectFieldIndex, Int64  &) const;
        void  GetMaxFor   (StatementPtr &, int keyFieldIndex1, Int64 keyValue1, int keyFieldIndex2, String const &keyValue2, int selectFieldIndex, String &) const;

        // Shorthand methods intended primarily for use on id fields with
        // an associated qualifier field (e.g., a type or name). These
        // versions produce results made up of pairs.
        void  SelectAllFor(StatementPtr   &, int keyFieldIndex1, Int64 keyValue1, int keyFieldIndex2, Int64 keyValue2, int selectFieldIndex1, int selectFieldIndex2, Int64StringVector &values, int sortFieldIndex=UNSORTED, bool distinct=!DISTINCT) const;

        // Shorthand methods intended primarily for use on id fields with
        // two associated qualifier fields (e.g., a type and number).
        void  DeleteAllFor(StatementPtr &, int keyFieldIndex1, Int64 keyValue1, int keyFieldIndex2, Int64 keyValue2, int keyFieldIndex3, Int64 keyValue3);
        void  SelectAllFor(StatementPtr   &, int keyFieldIndex1, Int64 keyValue1, int keyFieldIndex2, Int64 keyValue2, int keyFieldIndex3, Int64 keyValue3, int selectFieldIndex, Int64Vector  &values, int sortFieldIndex=UNSORTED, bool distinct=!DISTINCT) const;
        void  SelectAllFor(StatementPtr   &, int keyFieldIndex1, Int64 keyValue1, int keyFieldIndex2, Int64 keyValue2, int keyFieldIndex3, Int64 keyValue3, int selectFieldIndex, StringVector &values, int sortFieldIndex=UNSORTED, bool distinct=!DISTINCT) const;
        Int32 GetCountFor (StatementPtr &, int keyFieldIndex1, Int64 keyValue1, int keyFieldIndex2, Int64 keyValue2, int keyFieldIndex3, Int64 keyValue3) const;
        bool  GetExistsFor(StatementPtr &, int keyFieldIndex1, Int64 keyValue1, int keyFieldIndex2, Int64 keyValue2, int keyFieldIndex3, Int64 keyValue3) const;
        void  GetMaxFor   (StatementPtr &, int keyFieldIndex1, Int64 keyValue1, int keyFieldIndex2, Int64 keyValue2, int keyFieldIndex3, Int64 keyValue3, int selectFieldIndex, Int32  &) const;
        void  GetMaxFor   (StatementPtr &, int keyFieldIndex1, Int64 keyValue1, int keyFieldIndex2, Int64 keyValue2, int keyFieldIndex3, Int64 keyValue3, int selectFieldIndex, Int64  &) const;
        void  GetMaxFor   (StatementPtr &, int keyFieldIndex1, Int64 keyValue1, int keyFieldIndex2, Int64 keyValue2, int keyFieldIndex3, Int64 keyValue3, int selectFieldIndex, String &) const;

        void  SelectAllFor(StatementPtr   &, int keyFieldIndex1, Int64 keyValue1, int keyFieldIndex2, String const &keyValue2, int keyFieldIndex3, Int64 keyValue3, int selectFieldIndex, Int64Vector &values, int sortFieldIndex=UNSORTED, bool distinct=!DISTINCT) const;

        // Shorthand methods intended primarily for use on id fields with
        // two associated qualifier fields (e.g., a type and number). These
        // version produce results made up of pairs.
        void  SelectAllFor(StatementPtr   &, int keyFieldIndex1, Int64 keyValue1, int keyFieldIndex2, Int64 keyValue2, int keyFieldIndex3, Int64 keyValue3, int selectFieldIndex1, int selectFieldIndex2, Int64StringVector  &values, int sortFieldIndex=UNSORTED, bool distinct=!DISTINCT) const;

        // Binders for SQLite queries.
        void Bind(StatementPtr const &q, Int32 index, Int64         value) const;
        void Bind(StatementPtr const &q, Int32 index, Int32         value) const;
        void Bind(StatementPtr const &q, Int32 index, double        value) const;
        void Bind(StatementPtr const &q, Int32 index, String const &value) const;
        void Bind(StatementPtr const &q, Int32 index, Binary const &value) const;

        // Special command binders are required for use with Int64 values.
        // Use the "field index" form for values being inserted or updated.
        // Use the "bool" form in "where" clauses. For clarity, always pass
        // ID_SAFE as the fourth parameter in the "bool" form.
        void Bind(StatementPtr const &c, Int32 index, Int64 value, int fieldIndex) const;
        void Bind(StatementPtr const &c, Int32 index, Int64 value, bool) const;

        // Shorthand execs to match the binders.
        void Exec(StatementPtr const &)                const;
        void Exec(StatementPtr const &, Int32  &value) const;
        void Exec(StatementPtr const &, Int64  &value) const;
        void Exec(StatementPtr const &, double &value) const;
        void Exec(StatementPtr const &, String &value) const;
        void Exec(StatementPtr const &, Binary &value) const;

        Int64 GetLastInsertedRowId() const;

    public:
        virtual ~Table() {}

        DatabasePtr const &GetDatabase() const { return this->_db; }

        static Int64 const ID_NULL = 0;
        static bool  const ID_SAFE = true;

        struct FieldEntry
        {
            String::value_type const *fieldName;
            int                       fieldBits;
            String::value_type const *fieldSql;
        };

        static int const BIT_EMPTY   = 0;
        static int const BIT_INT32   = 1 << 0;
        static int const BIT_INT64   = 1 << 1;
        static int const BIT_INTID   = 1 << 2;
        static int const BIT_FLT32   = 1 << 3;  /* reserved */
        static int const BIT_FLT64   = 1 << 4;
        static int const BIT_NCSTR   = 1 << 5;
        static int const BIT_WCSTR   = 1 << 6;  /* reserved */
        static int const BIT_BLOB    = 1 << 7;
        static int const BIT_PKEYASC = 1 << 12; /* primary key asc */
        static int const BIT_PKEYINC = 1 << 13; /* primary key asc autoincrement */
        static int const BIT_UNIQUE  = 1 << 14;
        static int const BIT_NULLOK  = 1 << 15;
        static int const BIT_NOCASE  = 1 << 16;

        virtual String GetTableName()    const = 0;
        virtual int    GetFieldCount()   const = 0;
        virtual String GetFieldName(int) const = 0;
        virtual int    GetFieldBits(int) const = 0;
        virtual String GetFieldSql (int) const = 0;

        bool IsIntegral(int fieldIndex) const { return 0 != (GetFieldBits(fieldIndex) & (BIT_INT32   | BIT_INT64 | BIT_INTID)); }
        bool IsFloating(int fieldIndex) const { return 0 != (GetFieldBits(fieldIndex) & (BIT_FLT32   | BIT_FLT64)); }
        bool IsString  (int fieldIndex) const { return 0 != (GetFieldBits(fieldIndex) & (BIT_NCSTR   | BIT_WCSTR)); }
        bool IsBinary  (int fieldIndex) const { return 0 != (GetFieldBits(fieldIndex) &  BIT_BLOB); }
        bool IsIntId   (int fieldIndex) const { return 0 != (GetFieldBits(fieldIndex) &  BIT_INTID); }
        bool IsPKey    (int fieldIndex) const { return 0 != (GetFieldBits(fieldIndex) & (BIT_PKEYASC | BIT_PKEYINC)); }
        bool IsPKeyAsc (int fieldIndex) const { return 0 != (GetFieldBits(fieldIndex) &  BIT_PKEYASC); }
        bool IsPKeyInc (int fieldIndex) const { return 0 != (GetFieldBits(fieldIndex) &  BIT_PKEYINC); }
        bool IsUnique  (int fieldIndex) const { return 0 != (GetFieldBits(fieldIndex) &  BIT_UNIQUE); }
        bool IsNullOk  (int fieldIndex) const { return 0 != (GetFieldBits(fieldIndex) &  BIT_NULLOK); }
        bool IsNoCase  (int fieldIndex) const { return 0 != (GetFieldBits(fieldIndex) &  BIT_NOCASE); }

        virtual int    GetConstraintCount()   const;
        virtual String GetConstraintName(int) const;
        virtual String GetConstraintSql (int) const;

                void Create();
        virtual void Fill  ();
        virtual void Index ();
        virtual void Verify() const;

        // Returns " from <tableName>" for when an alias is unnecessary
        // or unacceptable (e.g., as in a "delete from <tableName>").
        String From() const;

        // Returns " from <tableName> as <asTable>".
        String From(String const &asTable) const;

        // Returns " join <tableName> as <asTable>".
        String Join(String const &asTable) const;

        // Returns " join <tableName> as <asTable> on <asTable>.<fieldName>".
        String Join(String const &asTable, int fieldIndex) const;

        // Returns " <fieldName>" or ", <fieldName>" where a table alias
        // is unnecessary or unacceptable (e.g., in the set clause of an
        // update statement).
        String Name(int fieldIndex, bool comma=true) const;

        // Returns " <asTable>.<fieldName>" or ", <asTable>.<fieldName>".
        String Name(String const &asTable, int fieldIndex, bool comma=true) const;

        // Returns " ifnull(<asTable>.<fieldName>, ...)" or ", ifnull(<asTable>.<fieldName>, ...)".
        String IfNull(String const &asTable, int fieldIndex, Int32         value, bool comma=true) const;
        String IfNull(String const &asTable, int fieldIndex, Int64         value, bool comma=true) const;
        String IfNull(String const &asTable, int fieldIndex, double        value, bool comma=true) const;
        String IfNull(String const &asTable, int fieldIndex, String const &value, bool comma=true) const;

        // Returns " ifnull(<asTable>.<fieldName>, Table::ID_NULL)" or ", ifnull(<asTable>.<fieldName>, Table::ID_NULL)".
        String IfNullId  (String const &asTable, int fieldIndex, bool comma=true) const;

        // Returns " ifnull(<asTable>.<fieldName>, zeroblob(0))" or ", ifnull(<asTable>.<fieldName>, zeroblob(0))".
        String IfNullBlob(String const &asTable, int fieldIndex, bool comma=true) const;

        // Returns " ifnull(<asTable>.<fieldName>, '')" or ", ifnull(<asTable>.<fieldName>, '')".
        String IfNullText(String const &asTable, int fieldIndex, bool comma=true) const;

        // Returns an equality expression for a field and a value list. Depending
        // on the list size, either an "equal or" expression or an "in" expression
        // is returned. In either case, the entire expression is wrapped in parens.
        String InValues(int fieldIndex, Int32Vector const &values) const;
        String InValues(int fieldIndex, Int64Vector const &values) const;

        String InValues(String const &asTable, int fieldIndex, Int32Vector const &values) const;
        String InValues(String const &asTable, int fieldIndex, Int64Vector const &values) const;

        // Returns a string appropriately quoted for sql (e.g., "a'b"
        // results in 'a''b').
        static String Quote(String const &);
    };
}
}
#endif/*KEYSIGHT_BBOX_ENGINE_CORE_SQLITEIMPL1_TABLE_H*/
