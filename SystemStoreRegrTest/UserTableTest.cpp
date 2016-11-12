// TestSystemStore.cpp : Defines the entry point for the console application.

#include "stdafx.h"
#include "..\SystemStore\SystemStore.h"
#include "Common\BaseDefs.h"
#include <iostream>

using namespace AOI::SystemStore;

static AOI::String TEST_RESTRICTION = 
"<?xml version=\"1.0\"?>\n\
<restrictions version=\"1.0.0.0\">\n\
    <!-- File Menu -->\n\
    <restriction key=\"Auto\" default=\"true\" value=\"true\" />\n\
    <restriction key=\"Auto_Start\" default=\"true\" value=\"true\" />\n\
    <restriction key=\"Auto_Stop\" default=\"true\" value=\"true\" />\n\
    <!-- Program Menu -->\n\
    <!-- Config Menu -->\n\
    <!-- Calibration Menu -->\n\
    <!-- Utility Menu -->\n\
    <!-- System Menu -->\n\
</restrictions>";

static void TestCreateUser()
{
    std::cout << std::endl << "------------------------------------------";
    std::cout << std::endl << "USER TABLE CREATE USER TEST #1 STARTING";
    std::cout << std::endl << "------------------------------------------";
    std::cout << std::endl;

    SystemStore systemStore;
    int nStatus = OK;

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

void TestUpdatePassword()
{
    std::cout << std::endl << "------------------------------------------";
    std::cout << std::endl << "USER TABLE UPDATE PASSWORD TEST #1 STARTING";
    std::cout << std::endl << "------------------------------------------";
    std::cout << std::endl;

    SystemStore systemStore;
    __int64 Id;
    int nStatus = OK;

    std::string passwordNew = "Yanliyuan1234$%";
    nStatus = systemStore.UpdatePassword("Engineer", "Engineer", passwordNew);
    if ( nStatus != OK )
        std::cout << "Failed to update password, error message: " << systemStore.GetErrMsg() << std::endl;
    else
        std::cout << "Success to update password" << std::endl;

    nStatus = systemStore.UserLogin("Engineer", "Engineer", Id);
    if ( nStatus != OK )
        std::cout << "Failed to log in use old password, error message: " << systemStore.GetErrMsg() << std::endl;

    nStatus = systemStore.UserLogin("Engineer", passwordNew, Id);
    if ( nStatus != OK )
        std::cout << "Failed to log in use new password, error message: " << systemStore.GetErrMsg() << std::endl;
    else
        std::cout << "Success to log in use new password" << std::endl;

    std::cout << std::endl << "----------------------------------------------";
    std::cout << std::endl << "USER TABLE GET ROLE AND RESTICTION #1 STARTING";
    std::cout << std::endl << "----------------------------------------------";
    std::cout << std::endl;

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

void TestUserTable()
{
    try
    {
    if ( AOI::FileUtils::Exists(SystemStore::GetDatabaseName()))
        AOI::FileUtils::Remove(SystemStore::GetDatabaseName());
    }catch ( std::exception &e)
    {
        std::cout << "Failed to delete old db file, error: " << e.what() << std::endl;
        return;
    }

    TestCreateUser();
    TestLogin();
    TestUpdatePassword();
}