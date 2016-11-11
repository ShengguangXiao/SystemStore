// TestSystemStore.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "..\SystemStore\SystemStore.h"
#include "Common\BaseDefs.h"
#include <iostream>

using namespace AOI::SystemStore;

int _tmain(int argc, _TCHAR* argv[])
{
    SystemStore systemStore;
    int nStatus = OK;

    systemStore.Init();
    nStatus = systemStore.AddUser("Engineer", "Engineer", UserRole::ENGINEER, "No Restriction");
    if ( nStatus != OK )
        std::cout << "Failed to add user, error message: " << systemStore.GetErrMsg() << std::endl;
    
    __int64 Id = 0;
    nStatus = systemStore.UserLogin("Engineer", "Engineer", Id);
    if ( nStatus != OK )
        std::cout << "Failed to log in, error message: " << systemStore.GetErrMsg() << std::endl;

	return 0;
}

