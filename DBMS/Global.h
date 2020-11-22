#pragma once

#define SYSPATH CString("DBMSROOT\\databases.db")
#define DATAFILEPATH CString("DBMSROOT\\data")

#define TYPE_BOOL      1
#define TYPE_DATETIME  2
#define TYPE_DOUBLE    3
#define TYPE_INTEGER   4
#define TYPE_VARCHAR   5

#include <string>
#include<fstream>
#include <atlstr.h>
#include"DBLogic.h"
#include"Tool.h"


using namespace std;

class Global
{

};

