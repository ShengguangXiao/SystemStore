#pragma once

#include <memory>
#include "Common\BaseDefs.h"

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

#define OK          (0)
#define NOK         (-1)

class SystemStore
{
public:
    SystemStore();
    ~SystemStore();
    int Init();
    String GetErrMsg() const;
    int AddUser(const String & name, const String & password, UserRole role, const String &restriction);
    int UserLogin(const String &name, const String &password, Int64 &Id);
private:
    struct Impl;
    std::unique_ptr<Impl> _pImpl;
};

}
}