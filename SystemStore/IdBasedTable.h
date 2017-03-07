#ifndef AGILENT_BBOX_ENGINE_CORE_SQLITEIMPL1_IDBASEDTABLE_H
#define AGILENT_BBOX_ENGINE_CORE_SQLITEIMPL1_IDBASEDTABLE_H
/*****************************************************************************
 * IdBasedTable.h -- $Id$
 *
 * Purpose
 *   Declares the IdBasedTable class.
 *
 * Indentation
 *   Four characters. No tabs!
 *
 * Modifications
 *   2012-03-26 (MM) Added a copy method.
 *   2012-03-02 (MM) Changed to a new storage organization.
 *   2010-12-10 (MM) Added generic Select/Update methods.
 *   2010-12-06 (MM) Created.
 *
 * Copyright (c) 2010, 2012 Xiao Shengguang.  All rights reserved.
 ****************************************************************************/

#include "Table.h"
#include "Constants.h"

namespace AOI
{
namespace SystemStore
{
    class IdBasedTable: public Table
    {
        StatementPtr copcmd;
        StatementPtr delcmd;

    protected:
        explicit IdBasedTable(DatabasePtr const &database): Table(database) {}

    public:
        virtual ~IdBasedTable() {}

        /********
        * Table *
        ********/
        virtual String GetTableName()    const = 0;
        virtual int    GetFieldCount()   const = 0;
        virtual String GetFieldName(int) const = 0;
        virtual int    GetFieldBits(int) const = 0;
        virtual String GetFieldSql (int) const = 0;

        /***************
        * IdBasedTable *
        ***************/
        virtual int GetFieldIndexOfId() const = 0;

        virtual Int64 Copy  (Int64 id);
        virtual void  Delete(Int64 id);

    protected:
        // Builds "select" commands/queries where the key field
        // is the id.
        StatementPtr BuildSelectCommandWithId(int valueFieldIndex) const;
        StatementPtr BuildSelectQueryWithId  (int const *valueFieldIndexBegin, int const *valueFieldIndexEnd) const;

        // Builds "update" commands where the key field is the id.
        StatementPtr BuildUpdateCommandWithId(int valueFieldIndex) const;
        StatementPtr BuildUpdateCommandWithId(int const *valueFieldIndexBegin, int const *valueFieldIndexEnd) const;

        void Select(Int64 id, StatementPtr &select, int fieldIndex, Int32  &) const;
        void Select(Int64 id, StatementPtr &select, int fieldIndex, Int64  &) const;
        void Select(Int64 id, StatementPtr &select, int fieldIndex, double &) const;
        void Select(Int64 id, StatementPtr &select, int fieldIndex, String &) const;
        void Select(Int64 id, StatementPtr &select, int fieldIndex, Binary &) const;

        void Select(Int64 id, StatementPtr &select, int fieldIndex, Enum::YesNo &) const;

        void Update(Int64 id, StatementPtr &update, int fieldIndex, Int32);
        void Update(Int64 id, StatementPtr &update, int fieldIndex, Int64);
        void Update(Int64 id, StatementPtr &update, int fieldIndex, double);
        void Update(Int64 id, StatementPtr &update, int fieldIndex, String const &);
        void Update(Int64 id, StatementPtr &update, int fieldIndex, Binary const &);
    };
}
}
#endif/*AGILENT_BBOX_ENGINE_CORE_SQLITEIMPL1_IDBASEDTABLE_H*/
