////////////////////////////////////////////////////////////////////////////////
//
// File Name:	CommonPath.h
// Class Name:
// Description:	用于存储一些公用路径
// Author:		罗志彬
// Date:		2019年4月22日
//
////////////////////////////////////////////////////////////////////////////////

#ifndef COMMONPATH_H
#define COMMONPATH_H

#pragma once

#include "CommonString.hpp"
#include "QtCommon/qtstring.hpp"
#define PATH_APPDATA (CQtString::get_app_path () + ("AppData\\"))
#define PATH_ROAMING (CQtString::get_app_path () + ("RoamingData\\"))
#define PATH_TEMPLATE (CQtString::get_app_path () + ("Template\\"))
#define PATH_RESOURCE (CQtString::get_app_path () + ("Resource\\"))
#define INI_FILE	(CQtString::get_app_path()+ ("Setting.ini"))
#define LOG_FILE	( CQtString::get_app_path() + ("Log.txt"))
#endif //__COMMONPATH_H__
