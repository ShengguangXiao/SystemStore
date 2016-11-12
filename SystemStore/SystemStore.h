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
    static String GetDatabaseName();
    String GetErrMsg() const;
    int AddUser(const String & name, const String & password, UserRole role, const String &restriction);
    int UserLogin(const String &name, const String &password, Int64 &Id);
    int UpdatePassword(const String &name, const String &password, const String &passwordNew);
    int GetUserRoleAndRestriction(Int64 Id, UserRole&role, String &restriction );
private:
    String _Encrypt(const String &input);
    Int32 _Init();
    struct Impl;
    std::unique_ptr<Impl> _pImpl;
};

}
}