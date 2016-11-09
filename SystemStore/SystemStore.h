#pragma once

#include <memory>
#include "Common\\BaseDefs.h"

namespace AOI
{
namespace SystemStore
{

enum class UserRole
{
    OPERATOR,
    ADMINISTRATOR,
    ENGINEER,
    DEVELOPER,
    END_,
};

class SystemStore
{
public:
    SystemStore();
    ~SystemStore();
    int Init();
    void AddUser(const String & name, const String & password, UserRole role, const String &restriction);
private:
    struct Impl;
    std::unique_ptr<Impl> _pImpl;
};

}
}