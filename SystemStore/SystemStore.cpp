#include "Common/BaseDefs.h"
#include <SQLiteCpp/SQLiteCpp.h>
#include "SystemStore.h"
#include "UserTable.h"
#include "Constants.h"
#include "Rijndael.h"

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
        Id = _pImpl->userTable->Select(name, _Encrypt ( password ) );
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

String SystemStore::_Encrypt(const String &strTarget)
{
 //   int len = strTarget.length();
	//char a;
	//String strFinal(strTarget);

	//for (int i = 0; i <= (len-1); i++)
	//{
	//	a = strTarget.at(i); 
	//	int b = (int)a; //get the ASCII value of 'a'
	//	b += 2; //Mulitply the ASCII value by 2
	//	if (b > 254) { b = 254; }
	//	a = (char)b; //Set the new ASCII value back into the char
	//	strFinal.insert(i , 1, a); //Insert the new Character back into the string
	//}
	//return String(strFinal, 0, len);
    //return strTarget;
    		CRijndael oRijndael;
		oRijndael.MakeKey(ENCRYPT_KEY, "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0", 16, 16);
		char szDataIn[] = "Engineer";
		char szDataOut[1000];
        memset(szDataOut, 0, sizeof (szDataOut));
		oRijndael.EncryptBlock(strTarget.c_str(), szDataOut);
        return String(szDataOut);
}

}
}