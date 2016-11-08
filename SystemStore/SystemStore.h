#pragma once

#include <memory>

#include <SQLiteCpp/SQLiteCpp.h>
#include <SQLiteCpp/VariadicBind.h>

class SystemStore
{
public:
    SystemStore();
    ~SystemStore();
    int Init();
private:
    std::shared_ptr<SQLite::Database> _db;
};

