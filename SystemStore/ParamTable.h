#ifndef AOI_SYSTEMSTORE_PARAM_TABLE_H
#define AOI_SYSTEMSTORE_PARAM_TABLE_H
/*****************************************************************************
 * PrConfigTable.h -- $Id$
 *
 * Purpose
 *   Declares the ParamTable class.
 *
 * Indentation
 *   Four characters. No tabs!
 *
 * Modifications
 *   2016-09-16 (XSG) Created.
 *
 * Copyright (c) 2016-2017 Xiao Shengguang.  All rights reserved.
 ****************************************************************************/

#include "IdBasedTable.h"

namespace AOI
{
namespace SystemStore
{
    class ParamTable;

    using ParamTablePtr = std::shared_ptr<ParamTable>;

    class ParamTable: public IdBasedTable
    {
        using IdBasedTable::Select;

    public:
        explicit ParamTable(DatabasePtr const &database): IdBasedTable(database) {}
        virtual ~ParamTable() {}

        enum FieldIndex
        {
            ID,
            NAME,
            VALUE,
            COUNT_,
        };

        /********
        * Table *
        ********/
        virtual String GetTableName()    const override { return StaticGetTableName(); }
        virtual int    GetFieldCount()   const override { return COUNT_; }
        virtual String GetFieldName(int) const override;
        virtual int    GetFieldBits(int) const override;
        virtual String GetFieldSql (int) const override;

        /***************
        * IdBasedTable *
        ***************/
        virtual int GetFieldIndexOfId() const { return ID; }

        /*************
        * ParamTable *
        *************/
        static String StaticGetTableName();

        Int64 Insert(String const &name, String const &value);
        void SelectValue(String const &name, String &) const;
        void UpdateValue        (String const &name, String const &);

    private:
        StatementPtr         insert;
        StatementPtr         insertAll;
        StatementPtr mutable selectValue;
        StatementPtr         updateByName;
    };
}
}
#endif/*AOI_ENGINE_CORE_SQLITEIMPL1_PRCONFIGTABLE_H*/
