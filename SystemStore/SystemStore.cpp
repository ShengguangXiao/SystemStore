#include "Common/BaseDefs.h"
#include <SQLiteCpp/SQLiteCpp.h>
#include "SystemStore.h"
#include "UserTable.h"
#include "Constants.h"

namespace AOI
{
namespace SystemStore
{

struct SystemStore::Impl { // as before
    DatabasePtr     db;
    UserTablePtr    userTable;
    String          errMsg;
};

SystemStore::SystemStore():_pImpl(std::make_unique<Impl>())
{
    // Open a database file in create/write mode
    _pImpl->db = std::make_shared<SQLite::Database>(SYSTEM_DB_NAME, SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);
}


SystemStore::~SystemStore()
{
}

int SystemStore::Init()
{
    _pImpl->userTable = std::make_shared<UserTable>( _pImpl->db );
    if ( !_pImpl->db->tableExists ( UserTable::StaticGetTableName() ) )
        _pImpl->userTable->Create();
    return 0;
}

String SystemStore::GetErrMsg() const
{
    return _pImpl->errMsg;
}

int SystemStore::AddUser(const String &name, const String &password, UserRole role, const String &restriction)
{
    try
    {
        _pImpl->userTable->Insert(name, password, ToInt32(role), restriction);
        return OK;
    }
    catch(SQLite::Exception &e)
    {
        _pImpl->errMsg = e.getErrorStr();
        return NOK;
    }
}

int SystemStore::UserLogin(const String &name, const String &password, Int64 &Id)
{
    try
    {
        Id = _pImpl->userTable->Select(name, password);
        return OK;
    }
    catch(SQLite::Exception &e)
    {
        if ( e.getErrorCode() != SQLite::UNKNOWN_ERROR )
            _pImpl->errMsg = e.getErrorStr();
        else
            _pImpl->errMsg = e.what();
        Id = 0;
        return NOK;
    }
}

}
}