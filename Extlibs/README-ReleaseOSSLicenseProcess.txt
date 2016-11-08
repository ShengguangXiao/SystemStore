How to release OSS license agreement for x1149
----------------------------------------------
To use Open Source Software in x1149, user need to first fill up
the OSS review spreadsheet and send it for legal review

To release OSS license agreement with x1149, please copy the
content of OSS license and append to 'license.txt' found in Extlib folder

This copy file is embedded into Logger.csproj (xml file). Search
for 'CopyFilesName' element found under 'PropertyGroup' of Logger.csproj
for both Debug and Release platform

<PropertyGroup Condition=" '$(Configuration)|$(Platform)' == 'Debug|x86' ">
    <PlatformTarget>x86</PlatformTarget>
    <DebugSymbols>true</DebugSymbols>
    <DebugType>full</DebugType>
    <Optimize>false</Optimize>
    <OutputPath>..\..\..\Obj\2010\Win32_Debug\bin\</OutputPath>
    <DefineConstants>DEBUG;TRACE</DefineConstants>
    <ErrorReport>prompt</ErrorReport>
    <WarningLevel>4</WarningLevel>
    <CopyFilesName>..\..\ExtLibs\license.txt</CopyFilesName>
</PropertyGroup>

To release Telerik license agreement with x1149, this file is embeded in
CopyFilesName function in Sequencer.csproj

<PropertyGroup Condition=" '$(Configuration)|$(Platform)' == 'Debug|AnyCPU' ">
    <DebugSymbols>true</DebugSymbols>
    <DebugType>full</DebugType>
    <Optimize>false</Optimize>
    <OutputPath>..\..\..\Obj\2010\Win32_Debug\bin\</OutputPath>
    <DefineConstants>DEBUG;TRACE</DefineConstants>
    <ErrorReport>prompt</ErrorReport>
    <WarningLevel>4</WarningLevel>
    <CopyFilesName>..\..\ExtLibs\Telerik\Telerik_license-agreement.pdf</CopyFilesName>
  </PropertyGroup>
  
Also make sure x1149 installer pick up this file from output build folder
