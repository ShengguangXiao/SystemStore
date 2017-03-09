#include "Common/BaseDefs.h"
#include <SQLiteCpp/SQLiteCpp.h>
#include "SystemStore.h"
#include "UserTable.h"
#include "ParamTable.h"
#include "Constants.h"
#include "Rijndael.h"

namespace AOI
{
namespace SystemStore
{

struct SystemStore::Impl { // as before
    DatabasePtr     db;
    UserTablePtr    userTable;
    ParamTablePtr   paramTable;
    String          errMsg;
};

SystemStore::SystemStore():_pImpl(std::make_unique<Impl>())
{
    // Open a database file in create/write mode
    _pImpl->db = std::make_shared<SQLite::Database>(SYSTEM_DB_NAME, SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);
    _Init();
}

SystemStore::~SystemStore()
{
}

Int32 SystemStore::_Init()
{
    _pImpl->userTable = std::make_shared<UserTable>( _pImpl->db );
    if ( !_pImpl->db->tableExists ( UserTable::StaticGetTableName() ) )
        _pImpl->userTable->Create();

    _pImpl->paramTable = std::make_shared<ParamTable>( _pImpl->db );
    if ( ! _pImpl->db->tableExists ( ParamTable::StaticGetTableName() ) )
        _pImpl->paramTable->Create();
    return 0;
}

/*static*/ String SystemStore::GetDatabaseName()
{
    return String(SYSTEM_DB_NAME);
}

String SystemStore::GetErrMsg() const
{
    return _pImpl->errMsg;
}

int SystemStore::AddUser(const String &name, const String &password, UserRole role, const String &restriction)
{
    try
    {
        _pImpl->userTable->Insert(name, _Encrypt( password ), ToInt32(role), restriction);
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
        Id = _pImpl->userTable->SelectUser(name, _Encrypt ( password ) );
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

int SystemStore::UpdatePassword(const String &name, const String &password, const String &passwordNew)
{
    try
    {
        Int64 Id = _pImpl->userTable->SelectUser(name, _Encrypt ( password ) );
        _pImpl->userTable->UpdatePassword(Id, _Encrypt ( passwordNew ) );
        return OK;
    }
    catch(SQLite::Exception &e)
    {
        if ( e.getErrorCode() != SQLite::UNKNOWN_ERROR )
            _pImpl->errMsg = e.getErrorStr();
        else
            _pImpl->errMsg = e.what();
        return NOK;
    }
}

int SystemStore::GetUserRoleAndRestriction(Int64 Id, UserRole&role, String &restriction)
{
    try
    {
        Int32 n32Role;
        _pImpl->userTable->SelectRole(Id, n32Role);
        role = static_cast<UserRole>(n32Role);
        _pImpl->userTable->SelectRestriction(Id, restriction);
        return OK;
    }
    catch(SQLite::Exception &e)
    {
        if ( e.getErrorCode() != SQLite::UNKNOWN_ERROR )
            _pImpl->errMsg = e.getErrorStr();
        else
            _pImpl->errMsg = e.what();
        return NOK;
    }
}

String SystemStore::_Encrypt(const String &strTarget)
{
    CRijndael oRijndael;
    oRijndael.MakeKey(ENCRYPT_KEY, "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0", 16, 16);
    char szDataOut[1000];
    memset(szDataOut, 0, sizeof(szDataOut));
    oRijndael.EncryptBlock(strTarget.c_str(), szDataOut);
    return String(szDataOut);
}

int SystemStore::AddParam(const String &name, Int32 value)
{
    String strValue = std::to_string(value);
    try
    {
        _pImpl->paramTable->Insert(name, strValue);
        return OK;
    }
    catch(SQLite::Exception &e)
    {
        _pImpl->errMsg = e.getErrorStr();
        return NOK;
    }
}

int SystemStore::AddParam(const String &name, double value)
{
    String strValue = std::to_string(value);
    try
    {
        _pImpl->paramTable->Insert(name, strValue);
        return OK;
    }
    catch(SQLite::Exception &e)
    {
        _pImpl->errMsg = e.getErrorStr();
        return NOK;
    }
}

int SystemStore::UpdateParam(const String &name, Int32 value)
{
    String strValue = std::to_string(value);
    try
    {
        _pImpl->paramTable->UpdateValue(name, strValue);
        return OK;
    }
    catch(SQLite::Exception &e)
    {
        _pImpl->errMsg = e.getErrorStr();
        return NOK;
    }
}

int SystemStore::UpdateParam(const String &name, double value)
{
    String strValue = std::to_string(value);
    try
    {
        _pImpl->paramTable->UpdateValue(name, strValue);
        return OK;
    }
    catch(SQLite::Exception &e)
    {
        _pImpl->errMsg = e.getErrorStr();
        return NOK;
    }
}

int SystemStore::GetParam(const String &name, Int32 &value)
{
    try
    {
        String strValue;
        _pImpl->paramTable->SelectValue(name, strValue);
        value = std::strtol ( strValue.c_str(), NULL, 10 );
        return OK;
    }
    catch(SQLite::Exception &e)
    {
        _pImpl->errMsg = e.getErrorStr();
        return NOK;
    }
}

int SystemStore::GetParam(const String &name, double &value)
{
    try
    {
        String strValue;
        _pImpl->paramTable->SelectValue(name, strValue);
        value = std::strtof ( strValue.c_str(), NULL );
        return OK;
    }
    catch(SQLite::Exception &e)
    {
        _pImpl->errMsg = e.getErrorStr();
        return NOK;
    }
}

}
}