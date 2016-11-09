#include "Common/BaseDefs.h"
#include "SystemStore.h"
#include "UserTable.h"
#include "Constants.h"
#include <SQLiteCpp/SQLiteCpp.h>

namespace AOI
{
namespace SystemStore
{

struct SystemStore::Impl { // as before
    std::shared_ptr<SQLite::Database> db;
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
    if ( !_pImpl->db->tableExists ( UserTable::StaticGetTableName() ) )
        UserTable(_pImpl->db).Create();
    return 0;
}

void SystemStore::AddUser(const String &name, const String &password, UserRole role, const String &restriction)
{
    UserTable(_pImpl->db).Insert(name, password, ToInt32(role), restriction);
}

}
}