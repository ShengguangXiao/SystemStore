// TestSystemStore.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "..\SystemStore\SystemStore.h"

using namespace AOI::SystemStore;

int _tmain(int argc, _TCHAR* argv[])
{
    SystemStore systemStore;
    systemStore.Init();
    systemStore.AddUser("Engineer", "Engineer", UserRole::ENGINEER, "No Restriction");
	return 0;
}

