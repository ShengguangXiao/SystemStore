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
            { SL("name"),                Table::BIT_NCSTR | Table::BIT_UNIQUE,  SL("") },
            { SL("password"),            Table::BIT_NCSTR,                      SL("") },
            { SL("role"),                Table::BIT_INT32,                      SL("") },
            { SL("restriction"),         Table::BIT_NCSTR,                      SL("") },            
        };

        BOOST_STATIC_ASSERT(sizeof(myFields) / sizeof(myFields[0]) == UserTable::COUNT_);
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

    Int64 UserTable::Insert
    (
        String const &name,
        String const &password,
        int           role,
        String const &restriction
    )
    {
        try
        {
            if (!this->insert)
            {
                int const fi [ ] = { NAME, PASSWORD, ROLE, RESTRICTION };
                this->insert = BuildInsertCommand(fi, fi + sizeof(fi) / sizeof(fi [ 0 ]));
            }

            int i = 0;
            Bind(this->insert, ++i, name);
            Bind(this->insert, ++i, password);
            Bind(this->insert, ++i, role);
            Bind(this->insert, ++i, restriction);
            Exec(this->insert);

            return GetLastInsertedRowId();
        }
        catch (...)
        {
            this->insert.reset();
            throw;
        }
    }

    void UserTable::UpdatePassword
    (
        Int64         id,
        String const &password
    )
    {
        Update(id, this->updatePassword, PASSWORD, password);
    }

    void UserTable::UpdateRestriction
    (
        Int64         id,
        String const &restriction
    )
    {
        Update(id, this->updateRestriction, RESTRICTION, restriction);
    }

    Int64 UserTable::SelectUser
    (
        String const &name,
        String const &password
    )
    {
        try
        {
            if (!this->selectUser)
            {
                this->selectUser = BuildSelectCommand(ID, NAME, PASSWORD);
            }

            int i = 0;
            Bind(this->selectUser, ++i, name);
            Bind(this->selectUser, ++i, password);

            Int64 id;
            Exec(this->selectUser, id);
            return id;
        }catch (...)
        {
            //This reset must have. Because the exception may throw from command->getColumn(0), so in the Exec function,
            //the reset doesn't run.
            this->selectUser->reset();
            throw;
        }
    }

    void UserTable::SelectRole(Int64 id, Int32 &value) const
    {
        Select(id, this->selectRole, ROLE, value);
    }

    void UserTable::SelectRestriction(Int64 id, String &value) const
    {
        Select(id, this->selectRestriction, RESTRICTION, value);
    }

}
}