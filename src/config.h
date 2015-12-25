/*****************************************************************************
* servermgr Syping Gaming Team Server Manager
* Copyright (C) 2015-2016 Syping Gaming Team
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*     http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*****************************************************************************/

#ifndef CONFIG_H
#define CONFIG_H

#include <QString>

// Choose SHARED if not STATIC
#ifndef STATIC
#ifndef SHARED
#ifdef QT_STATIC
#define STATIC
#else
#define SHARED
#endif
#endif
#endif

#ifndef WIN64
#ifdef Q_WS_WIN64
#define WIN64
#else
#ifdef Q_QS_WIN64
#define WIN64
#endif
#endif
#endif

#ifndef DISABLE_UPDATE_MANAGER
#ifndef UPDATE_MANAGER
#ifdef SHARED
#ifdef WIN32
#define UPDATE_MANAGER
#else
#ifdef MACX
#define UPDATE_MANAGER
#endif
#endif
#endif
#endif
#else
#undef UPDATE_MANAGER
#endif

#ifndef ProductItl
#define ProductItl "SERVERMGR"
#endif
#ifndef ProductName
#define ProductName "Server Manager"
#endif
#ifndef ProductVendor
#define ProductVendor "Syping Gaming Team"
#endif
#ifndef ProductVersion
#define ProductVersion "1.2.0"
#endif
#ifndef ProductPixmap
#define ProductPixmap ":/pixmap/server-pixmap.png"
#endif
#ifdef SM_ANDROID
#ifndef ProductImg
#define ProductImg ":/pixmap/server-pixmap.png"
#endif
#else
#ifndef ProductImg
#define ProductImg ":/icon/server-icon.png"
#endif
#endif
#ifndef ProductIcon
#define ProductIcon ":/icon/server-icon_16.png"
#endif
#ifndef VendorIcon
#define VendorIcon ":/icon/server-icon.png"
#endif

#ifndef AboutVendor
#define AboutVendor "No about Vendor Text<br><br>Copyright &copy; SypingGT 2015"
#endif
#ifndef AboutProduct
#define AboutProduct "Server Manager is a server managing tool<br/><br/>Copyright &copy; SypingPT 2015"
#endif

#ifndef TranslationPath
#define TranslationPath "$ED/trplugs"
#endif


#ifndef VersionType
#ifdef MACX
#define VersionType "mxqt4"
#else
#ifdef UNIX
#define VersionType "unixqt"
#else
#ifdef QT5
#ifdef WIN64
#define VersionType "64qt5"
#else
#define VersionType "32qt5"
#endif
#else
#ifdef WIN64
#define VersionType "64qt4"
#elif defined(MSVC)
#define VersionType "vcqt4"
#else
#ifdef WIN32
#define VersionType "32qt4"
#else
#define VersionType "unknownqt"
#endif
#endif
#endif
#endif
#endif
#endif

#ifndef ApplicationID
#ifndef UNIX
#ifdef WIN64
#define ApplicationID "Server Manager"
#elif defined(MSVC)
#define ApplicationID "Server Manager"
#else
#define ApplicationID "{24ABE582-3070-4945-98D2-0D782817F96C}"
#endif
#else
#define ApplicationID "Server Manager"
#endif
#endif

#ifndef UpdatePath
#define UpdatePath "http://servermgr.syping.de/update/"
#endif

#ifndef ProductCompiler
#ifdef GCC
#ifdef __VERSION__
#define ProductCompiler QString("GCC " + QString(__VERSION__))
#else
#define ProductCompiler "GCC"
#endif
#elif defined(MSVC)
#ifdef MSVC2005
#define ProductCompiler "MSVC2005"
#elif defined(MSVC2008)
#define ProductCompiler "MSVC2008"
#elif defined(MSVC2010)
#define ProductCompiler "MSVC2010"
#elif defined(MSVC2012)
#define ProductCompiler "MSVC2012"
#elif defined(MSVC2013)
#define ProductCompiler "MSVC2013"
#else
#define ProductCompiler "MSVC"
#endif
#else
#define ProductCompiler "Unknown"
#endif
#endif

#endif // CONFIG_H
