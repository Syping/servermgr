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
    initLua();
}

ServerManagerLua::~ServerManagerLua()
{
    closeLuaState();
}

void ServerManagerLua::initLua()
{
    lp = luaL_newstate();
    luaL_openlibs(lp);
}

void ServerManagerLua::closeLuaState()
{
    lua_close(lp);
}

void ServerManagerLua::setStandardGlobalValues()
{
    lua_setglobal(lp, "SM_LUA_RETURN");
    lua_pushstring(lp, "");
}

QString ServerManagerLua::getReturnValue()
{
    lua_getglobal(lp, "SM_LUA_RETURN");
    return QString(lua_tostring(lp, -1));
}

QString ServerManagerLua::runLuaScriptFile(QString filePath)
{
    setStandardGlobalValues();
    luaL_dofile(lp, filePath.toStdString().c_str());
    return getReturnValue();
}

QString ServerManagerLua::runLuaScriptString(QString luaScript)
{
    setStandardGlobalValues();
    luaL_dostring(lp, luaScript.toStdString().c_str());
    return getReturnValue();
}

QString ServerManagerLua::getLuaGlobalString(QString globalValue)
{
    lua_getglobal(lp, globalValue.toStdString().c_str());
    return QString(lua_tostring(lp, -1));
}

void ServerManagerLua::setLuaGlobalString(QString globalValue, QString newString)
{
    lua_setglobal(lp, globalValue.toStdString().c_str());
    lua_pushstring(lp, newString.toStdString().c_str());
}

void ServerManagerLua::runLuaFunction(QString function)
{
    lua_getglobal(lp, function.toStdString().c_str());
    lua_call(lp, 0, 0);
}
