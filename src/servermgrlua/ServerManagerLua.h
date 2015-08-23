/*****************************************************************************
* servermgrlua Syping Gaming Team Server Manager Framework Lua
* Copyright (C) 2015 Syping Gaming Team
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

#ifndef SERVERMANAGERLUA_H
#define SERVERMANAGERLUA_H

#include <QObject>

extern "C"
{
#include "lua/lapi.h"
#include "lua/lauxlib.h"
#include "lua/lcode.h"
#include "lua/lctype.h"
#include "lua/ldebug.h"
#include "lua/ldo.h"
#include "lua/lfunc.h"
#include "lua/lgc.h"
#include "lua/llex.h"
#include "lua/llimits.h"
#include "lua/lmem.h"
#include "lua/lobject.h"
#include "lua/lopcodes.h"
#include "lua/lparser.h"
#include "lua/lstate.h"
#include "lua/lstring.h"
#include "lua/ltable.h"
#include "lua/ltm.h"
#include "lua/lua.h"
#include "lua/luaconf.h"
#include "lua/lualib.h"
#include "lua/lundump.h"
#include "lua/lvm.h"
#include "lua/lzio.h"
}

class ServerManagerLua : public QObject
{
    Q_OBJECT

public:
    explicit ServerManagerLua(QObject *parent = 0);
    QString runLuaScriptFile(QString filePath);
    QString runLuaScriptString(QString luaScript);
    void closeLuaState();
    QString getLuaGlobalString(QString globalValue);
    void setLuaGlobalString(QString globalValue, QString newString);
    void runLuaFunction(QString function);
    ~ServerManagerLua();

private:
    void initLua();
    void setStandardGlobalValues();
    QString getReturnValue();
    static int sm_run_background(lua_State *L);
    static int sm_gui_msgbox(lua_State *L);
    static int sm_gui_inputbox(lua_State *L);
    lua_State *lp;

};

#endif // SERVERMANAGERLUA_H
