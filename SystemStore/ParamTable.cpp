/*****************************************************************************
 * PrConfigTable.cpp -- $Id$
 *
 * Purpose
 *   Implements the ParamTable class.
 *
 * Indentation
 *   Four characters. No tabs!
 *
 * Modifications
 *   2010-12-06 (XSG) Created.
 *
 * Copyright (c) 2016-2017 Keysight Technologies, Inc.  All rights reserved.
 ****************************************************************************/

#include "Common/BaseDefs.h"
#include "ParamTable.h"

namespace AOI
{
namespace SystemStore
{
    namespace
    {
        Table::FieldEntry const myFields[] =
        {
            { SL("id"),           Table::BIT_INTID | Table::BIT_PKEYINC, SL("") },
            { SL("name"),         Table::BIT_NCSTR | Table::BIT_UNIQUE,  SL("") },
            { SL("value"),        Table::BIT_NCSTR,                      SL("") },
        };

        BOOST_STATIC_ASSERT(sizeof(myFields) / sizeof(myFields[0]) == ParamTable::COUNT_);
    }

    /***********************
    * Table implementation *
    ***********************/
    int ParamTable::GetFieldBits(int index) const
    {
        assert(index >= 0 && index < GetFieldCount() && "Field index out of range in SQLiteImpl1::PrConfigTable::GetFieldBits.");
        return myFields[index].fieldBits;
    }

    String ParamTable::GetFieldName(int index) const
    {
        assert(index >= 0 && index < GetFieldCount() && "Field index out of range in SQLiteImpl1::PrConfigTable::GetFieldName.");
        return myFields[index].fieldName;
    }

    String ParamTable::GetFieldSql(int index) const
    {
        assert(index >= 0 && index < GetFieldCount() && "Field index out of range in SQLiteImpl1::PrConfigTable::GetFieldSql.");
        return myFields[index].fieldSql;
    }    

    /*******************************
    * PrConfigTable implementation *
    *******************************/

    /*static*/String ParamTable::StaticGetTableName()
    {
        return SL("param");
    }

    Int64 ParamTable::Insert(String const &name, String const &value)
    {
        try
        {
            if (!this->insert)
            {
                int const fi[] = { NAME, VALUE };
                this->insert = BuildInsertCommand(fi, fi + sizeof(fi) / sizeof(fi[0]));
            }

            int i = 0;
            Bind(this->insert, ++i, name);
            Bind(this->insert, ++i, value);
            Exec(this->insert);

            return GetLastInsertedRowId();
        }
        catch (...)
        {
            this->insert.reset();
            throw;
        }
    }

    void ParamTable::SelectValue(String const &name, String &value) const
    {
        this->selectValue = BuildSelectCommand(VALUE, NAME);

        Bind(this->selectValue, 1, name);
        Exec(this->selectValue, value);
    }
    
    void ParamTable::UpdateValue(String const &name, String const &value)
    {
        if (!this->updateByName)
        {
            String fmt = SL("update %s set %s = ? where %s = ?;");
            String sql = (boost::format(fmt) % GetTableName() % GetFieldName(VALUE) % GetFieldName(NAME)).str();
            this->updateByName = std::make_shared<SQLite::Statement>(*GetDatabase().get(), sql);
        }

        int i = 0;
        Bind(this->updateByName, ++i, value);
        Bind(this->updateByName, ++i, name);
        Exec(this->updateByName);
    }
}
}
