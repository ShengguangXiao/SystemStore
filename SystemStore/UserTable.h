#ifndef AOI_SYSTEMSTORE_USERTABLE_H
#define AOI_SYSTEMSTORE_USERTABLE_H

/*****************************************************************************
 * UserTable.h -- $Id$
 *
 * Purpose
 *   Declares the UserTable class which is the base class for classes that
 *   contain information about machine users.
 *
 * Indentation
 *   Four characters. No tabs!
 *
 * Modifications
 *   2016-11-09 (XSG) Created.
 *
 * Copyright (c) 2016-2016, Xiao Shengguang.  All rights reserved.
 ****************************************************************************/

#include "IdBasedTable.h"

namespace AOI
{
namespace SystemStore
{
    class UserTable : public IdBasedTable
    {
    public:
        explicit UserTable(DatabasePtr const &database ) : IdBasedTable( database )   {}
        virtual ~UserTable() {};
        
        enum FieldIndex
        {
            ID,
            NAME,
            PASSWORD,
            ROLE,
            RESTRICTION,
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
        *  UserTable *
        *************/
        static String StaticGetTableName();

        Int64 Insert
        (
            String const &name,
            String const &password,
            int           role,
            String const &restriction
        );

        void UpdatePassword
        (
            Int64         id,
            String const &password
        );

        void UpdateRestriction
        (
            Int64         id,
            String const &restriction
        );

        Int64 Select
        (
            String const &name, 
            String const &password
        );

    private:
        StatementPtr        insert;
        StatementPtr        updatePassword;
        StatementPtr        updateRestriction;
        StatementPtr        select;
    };
}
}

#endif