// TestSystemStore.cpp : Defines the entry point for the console application.

#include "stdafx.h"
#include "..\SystemStore\SystemStore.h"
#include "Common\BaseDefs.h"
#include <iostream>

using namespace AOI::SystemStore;

static AOI::String TEST_RESTRICTION = "<?xml version=\"1.0\"?>\
	<restrictions version=\"1.0.0.0\">\
		<!-- File Menu --> \
		<restriction key=\"Auto\" default=\"true\" value=\"true\" /> \
		<restriction key=\"Auto_Start\" default=\"true\" value=\"true\" /> \
		<restriction key=\"Auto_Stop\" default=\"true\" value=\"true\" /> \
		<!-- Program Menu --> \
		<!-- Config Menu --> \
        <!-- Calibration Menu --> \
        <!-- Utility Menu --> \
        <!-- System Menu --> \
	</restrictions>";

static void TestCreateUser()
{
    std::cout << std::endl << "------------------------------------------";
    std::cout << std::endl << "USER TABLE CREATE USER TEST #1 STARTING";
    std::cout << std::endl << "------------------------------------------";
    std::cout << std::endl;

    SystemStore systemStore;
    int nStatus = OK;

    systemStore.Init();
    nStatus = systemStore.AddUser("Engineer", "Engineer", UserRole::ENGINEER, TEST_RESTRICTION);
    if ( nStatus != OK )
        std::cout << "Failed to add user \"Engineer\", error message: " << systemStore.GetErrMsg() << std::endl;
    else
        std::cout << "Success to add user \"Engineer\"" << std::endl;

    nStatus = systemStore.AddUser("Op", "Operator", UserRole::OPERATOR, TEST_RESTRICTION);
    if ( nStatus != OK )
        std::cout << "Failed to add user \"Operator\", error message: " << systemStore.GetErrMsg() << std::endl;
    else
        std::cout << "Success to add user \"Operator\"" << std::endl;

    nStatus = systemStore.AddUser("Admin", "Admin", UserRole::ADMINISTRATOR, TEST_RESTRICTION);
    if ( nStatus != OK )
        std::cout << "Failed to add user \"Admin\", error message: " << systemStore.GetErrMsg() << std::endl;
    else
        std::cout << "Success to add user \"Admin\"" << std::endl;

    nStatus = systemStore.AddUser("Developer", "Developer", UserRole::DEVELOPER, TEST_RESTRICTION);
    if ( nStatus != OK )
        std::cout << "Failed to add user \"Developer\", error message: " << systemStore.GetErrMsg() << std::endl;
    else
        std::cout << "Success to add user \"Developer\"" << std::endl;

    //Test create the duplicated user name
    nStatus = systemStore.AddUser("Engineer", "Engineer", UserRole::ENGINEER, TEST_RESTRICTION);
    if ( nStatus != OK )
        std::cout << "Failed to add user \"Engineer\", error message: " << systemStore.GetErrMsg() << std::endl;
    else
        std::cout << "Success to add user \"Engineer\"" << std::endl;

    //Test create with empty restriction
    nStatus = systemStore.AddUser("Engineer1", "Engineer1", UserRole::ENGINEER, "");
    if ( nStatus != OK )
        std::cout << "Failed to add user \"Engineer1\", error message: " << systemStore.GetErrMsg() << std::endl;
    else
        std::cout << "Success to add user \"Engineer1\"" << std::endl;
}

static void TestLogin()
{
    std::cout << std::endl << "------------------------------------------";
    std::cout << std::endl << "USER TABLE LOGIN TEST #1 STARTING";
    std::cout << std::endl << "------------------------------------------";
    std::cout << std::endl;

    SystemStore systemStore;
    systemStore.Init();
    __int64 Id;
    int nStatus = OK;

    nStatus = systemStore.UserLogin("Engineer", "Engineer", Id);
    if ( nStatus != OK )
        std::cout << "Failed to log in, error message: " << systemStore.GetErrMsg() << std::endl;
    else
        std::cout << "Success log in, user ID: " << Id << std::endl;

    nStatus = systemStore.UserLogin("Engineer2", "Engineer2", Id);
    if ( nStatus != OK )
        std::cout << "Failed to log in, error message: " << systemStore.GetErrMsg() << std::endl;
    else
        std::cout << "Success log in, user ID: " << Id << std::endl;

    nStatus = systemStore.UserLogin("Op", "Operator", Id);
    if ( nStatus != OK )
        std::cout << "Failed to log in, error message: " << systemStore.GetErrMsg() << std::endl;
    else
        std::cout << "Success log in, user ID: " << Id << std::endl;
}

void TestUserTable()
{
    if ( AOI::FileUtils::Exists(SystemStore::GetDatabaseName()))
        AOI::FileUtils::Remove(SystemStore::GetDatabaseName());

    TestCreateUser();
    TestLogin();
}