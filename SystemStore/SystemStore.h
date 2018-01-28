#pragma once

#include <string>
#include <memory>

#pragma warning(push)
#pragma warning(disable:4251)

#ifndef API_CALL
#ifdef  __cplusplus
#define API_CALL __declspec(dllimport)
#else
#define API_CALL extern __declspec(dllimport) __stdcall
#endif
#endif

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
using String =      std::string;
using Int64 =       __int64;
using Int32 =       __int32;

class API_CALL SystemStore
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
    int AddParam(const String &name, Int32 value);
    int AddParam(const String &name, double value);
    int UpdateParam(const String &name, Int32 value);
    int UpdateParam(const String &name, double value);
    int GetParam(const String &name, Int32 &value);
    int GetParam(const String &name, double &value);
private:
    String _Encrypt(const String &input);
    Int32 _Init();
    struct Impl;
    std::unique_ptr<Impl> _pImpl;
};

}
}

#pragma warning(pop)