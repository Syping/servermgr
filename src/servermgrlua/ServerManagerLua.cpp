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
#include <QMessageBox>
#include <QProcess>
#include <QInputDialog>

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
    lua_register(lp, "sm_run_background", sm_run_background);
    //lua_register(lp, "sm_gui_msgbox", sm_gui_msgbox);
    //lua_register(lp, "sm_gui_inputbox", sm_gui_inputbox);
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

void ServerManagerLua::registerLuaFunction(QString function, lua_CFunction cfunction)
{
    lua_register(lp, function.toStdString().c_str(), cfunction);
}

// Lua Functions

int ServerManagerLua::sm_run_background(lua_State *L)
{
    int n = lua_gettop(L);
    QString execLine;
    if (n >= 1)
    {
        execLine = QString(lua_tostring(L, 1));
        QProcess execProcess;
        execProcess.startDetached(execLine);
    }
    return 0;
}

int ServerManagerLua::sm_gui_msgbox(lua_State *L)
{
    int n = lua_gettop(L);
    const char* text = "";
    const char* title = "Server Manager Lua";
    const char* mode = "";
    if (n == 0) return 0;
    if (n == 1)
    {
        text = lua_tostring(L, 1);
    }
    if (n == 2)
    {
        text = lua_tostring(L, 1);
        title = lua_tostring(L, 2);
    }
    if (n >= 3)
    {
        text = lua_tostring(L, 1);
        title = lua_tostring(L, 2);
        mode = lua_tostring(L, 3);
    }
    QString smode = mode;
    QWidget *vThis = new QWidget();
    vThis->setWindowModality(Qt::ApplicationModal);
    if (smode == "0" || smode == "information") {QMessageBox::information(vThis, QString(title), QString(text));}
    else if (smode == "1" || smode == "warning") {QMessageBox::warning(vThis, QString(title), QString(text));}
    else {QMessageBox::information(vThis, QString(title), QString(text));}
    return 0;
}

int ServerManagerLua::sm_gui_inputbox(lua_State *L)
{
    int n = lua_gettop(L);
    const char* title = "Server Manager Lua";
    const char* text = "Input:";
    QString inputText;
    QLineEdit::EchoMode InputBoxEchoMode = QLineEdit::Normal;

    if(n == 1)
    {
        text = lua_tostring(L, 1);
    }
    if(n == 2)
    {
        text = lua_tostring(L, 1);
        title = lua_tostring(L, 2);
    }
    if(n == 3)
    {
        text = lua_tostring(L, 1);
        title = lua_tostring(L, 2);
        inputText = QString(lua_tostring(L, 3));
    }
    if (n >= 4)
    {
        title = lua_tostring(L, 1);
        text = lua_tostring(L, 2);
        inputText = QString(lua_tostring(L, 3));
        if (lua_toboolean(L, 4))
        {
            InputBoxEchoMode = QLineEdit::Password;
        }
    }

    QWidget *vThis = new QWidget();
    vThis->setWindowModality(Qt::ApplicationModal);

    bool ok;
    inputText = QInputDialog::getText(vThis, QString(title), QString(text), InputBoxEchoMode, inputText, &ok);

    if (ok)
    {
        lua_pushstring(L, inputText.toStdString().c_str());
        return 1;
    }
    return 0;
}
