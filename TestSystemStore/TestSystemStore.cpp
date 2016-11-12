// TestSystemStore.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "..\SystemStore\SystemStore.h"
#include "Common\BaseDefs.h"
#include <iostream>

using namespace AOI::SystemStore;

int _tmain(int argc, _TCHAR* argv[])
{
    if ( AOI::FileUtils::Exists(SystemStore::GetDatabaseName()))
        AOI::FileUtils::Remove(SystemStore::GetDatabaseName());

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

	return 0;
}

