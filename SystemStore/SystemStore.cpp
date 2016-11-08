#include "Common/BaseDefs.h"
#include "SystemStore.h"
#include "UserTable.h"
#include "Constants.h"

SystemStore::SystemStore()
{
    // Open a database file in create/write mode
    _db = std::make_shared<SQLite::Database>(SYSTEM_DB_NAME, SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);
    //std::cout << "SQLite database file '" << db.getFilename().c_str() << "' opened successfully\n";
}


SystemStore::~SystemStore()
{
}

int SystemStore::Init()
{
    return 0;
}