// TestSystemStore.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "..\SystemStore\SystemStore.h"
#include "Common\BaseDefs.h"
#include <iostream>

using namespace AOI::SystemStore;

void TestUserTable()
{
    SystemStore systemStore;
    int nStatus = OK;

    nStatus = systemStore.AddUser("Engineer", "Engineer", UserRole::ENGINEER, "No Restriction");
    if ( nStatus != OK )
        std::cout << "Failed to add user, error message: " << systemStore.GetErrMsg() << std::endl;
    
    __int64 Id = 0;
    nStatus = systemStore.UserLogin("Engineer", "Engineer", Id);
    if ( nStatus != OK )
        std::cout << "Failed to log in, error message: " << systemStore.GetErrMsg() << std::endl;

    std::string passwordNew = "Yanliyuan1234$%";
    nStatus = systemStore.UpdatePassword("Engineer", "Engineer", passwordNew);
    if ( nStatus != OK )
        std::cout << "Failed to log in, error message: " << systemStore.GetErrMsg() << std::endl;

    nStatus = systemStore.UserLogin("Engineer", "Engineer", Id);
    if ( nStatus != OK )
        std::cout << "Failed to log in, error message: " << systemStore.GetErrMsg() << std::endl;

    nStatus = systemStore.UserLogin("Engineer", passwordNew, Id);
    if ( nStatus != OK )
        std::cout << "Failed to log in, error message: " << systemStore.GetErrMsg() << std::endl;

    UserRole role;
    AOI::String restriction;
    nStatus = systemStore.GetUserRoleAndRestriction(Id, role, restriction );
    if ( nStatus != OK )
        std::cout << "Failed to get role and restriction, error message: " << systemStore.GetErrMsg() << std::endl;
    else
    {
        std::cout << "Success to get role and restriction" << std::endl;
        std::cout << "Role: " << static_cast<__int32>(role) << std::endl;
        std::cout << "Restriction: " << restriction << std::endl;
    }
}

void TestParamTable()
{   
    SystemStore systemStore;
    int nStatus = OK;

    nStatus = systemStore.AddParam("Language", 1);
    if ( nStatus != OK )
        std::cout << "Failed to add parameter, error message: " << systemStore.GetErrMsg() << std::endl;
    
    nStatus = systemStore.AddParam("accurary", 1.8);
    if ( nStatus != OK )
        std::cout << "Failed to add parameter, error message: " << systemStore.GetErrMsg() << std::endl;

    nStatus = systemStore.UpdateParam("Language", 2);
    if ( nStatus != OK )
        std::cout << "Failed to add parameter, error message: " << systemStore.GetErrMsg() << std::endl;
    
    nStatus = systemStore.UpdateParam("accurary", 2.8);
    if ( nStatus != OK )
        std::cout << "Failed to add parameter, error message: " << systemStore.GetErrMsg() << std::endl;

    int nValue = 0;
    nStatus = systemStore.GetParam("Language", nValue);
    if ( nStatus != OK )
        std::cout << "Failed to add parameter, error message: " << systemStore.GetErrMsg() << std::endl;
    else
        std::cout << "Success get int param, value: " << nValue << std::endl;
    
    double dValue = 0;
    nStatus = systemStore.GetParam("accurary", dValue);
    if ( nStatus != OK )
        std::cout << "Failed to add parameter, error message: " << systemStore.GetErrMsg() << std::endl;
     else
        std::cout << "Success get float param, value: " << dValue << std::endl;
}

int _tmain(int argc, _TCHAR* argv[])
{
    if ( AOI::FileUtils::Exists(SystemStore::GetDatabaseName()))
        AOI::FileUtils::Remove(SystemStore::GetDatabaseName());

    TestUserTable();
    TestParamTable();
	return 0;
}

