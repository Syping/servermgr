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

#include "ServerManagerLua.h"

ServerManagerLua::ServerManagerLua(QObject *parent) : QObject(parent)
{

}

lua_State* ServerManagerLua::initLua()
{
    lua_State *lp;
    lp = luaL_newstate();
    luaL_openlibs(lp);
    return lp;
}

void ServerManagerLua::haltLua(lua_State *lp)
{
    lua_close(lp);
}

QString ServerManagerLua::runLuaScriptFile(lua_State *lp, QString filePath)
{
    luaL_dofile(lp, filePath.toStdString().c_str());
}

QString ServerManagerLua::runLuaScriptString(lua_State *lp, QString luaScript)
{
    luaL_dostring(lp, luaScript.toStdString().c_str());
}
