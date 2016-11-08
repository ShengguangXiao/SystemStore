#include "Common/BaseDefs.h"
#include "UserTable.h"

namespace AOI
{
namespace SystemStore
{
    namespace
    {
        Table::FieldEntry const myFields[] =
        {
            { SL("id"),                  Table::BIT_INTID | Table::BIT_PKEYINC, SL("") },
            { SL("name"),                Table::BIT_NCSTR,                      SL("") },
            { SL("password"),            Table::BIT_NCSTR,                      SL("") },
            { SL("restriction"),         Table::BIT_NCSTR,                      SL("") },            
        };

        BOOST_STATIC_ASSERT(sizeof(myFields) / sizeof(myFields[0]) == UserTable::COUNT_);

        String::value_type const *constraints[] =
        {
            SL("foreign key (light_id) references light (id)"),
        };
    }

    int UserTable::GetFieldBits(int index) const
    {
        assert(index >= 0 && index < GetFieldCount() && "Field index out of range in UserTable::GetFieldBits.");
        return myFields[index].fieldBits;
    }

    String UserTable::GetFieldName(int index) const
    {
        assert(index >= 0 && index < GetFieldCount() && "Field index out of range in UserTable::GetFieldName.");
        return myFields[index].fieldName;
    }

    String UserTable::GetFieldSql(int index) const
    {
        assert(index >= 0 && index < GetFieldCount() && "Field index out of range in UserTable::GetFieldSql.");
        return myFields[index].fieldSql;
    }

    /*static*/String UserTable::StaticGetTableName()
    {
        return SL("users");
    }
}
}