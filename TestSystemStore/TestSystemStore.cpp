// TestSystemStore.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "..\SystemStore\SystemStore.h"

using namespace AOI::SystemStore;

int _tmain(int argc, _TCHAR* argv[])
{
    SystemStore systemStore;
    systemStore.Init();
	return 0;
}

