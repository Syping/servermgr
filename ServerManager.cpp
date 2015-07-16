/*****************************************************************************
* servermgr Syping Gaming Team Server Manager
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

#include "ServerManager.h"
#include "config.h"

#include <QCryptographicHash>
#include <QProcess>

ServerManager::ServerManager(QObject *parent) : QObject(parent)
{
    configFile = new QSettings(ProductVendor,ProductName);
}

QStringList ServerManager::getServerList()
{
    return configFile->value("serverList",QStringList()).toStringList();
}

QString ServerManager::getIconPath(QString serverName)
{
    configFile->beginGroup("server " + serverName);
    QString iconPath = configFile->value("icon",ProductImg).toString();
    configFile->endGroup();
    return iconPath;
}

bool ServerManager::addServer(QString serverName)
{
    QStringList serverList = configFile->value("serverList",QStringList()).toStringList();
    if (!serverList.contains(serverName))
    {
        serverList.append(serverName);
        configFile->setValue("serverList",serverList);
        return true;
    }
    return false;
}

void ServerManager::deleteServer(QString serverName)
{
    QStringList serverList = configFile->value("serverList",QStringList()).toStringList();
    serverList.removeAll(serverName);
    configFile->setValue("serverList",serverList);
    configFile->beginGroup("server " + serverName);
    configFile->remove("start");
    configFile->remove("stop");
    configFile->remove("config");
    configFile->remove("update");
    configFile->remove("icon");
    configFile->endGroup();
}

QString ServerManager::getStartCommand(QString serverName)
{
    configFile->beginGroup("server " + serverName);
    QString serverStart = configFile->value("start").toString();
    configFile->endGroup();
    return serverStart;
}

void ServerManager::setStartCommand(QString serverName, QString startCommand)
{
    configFile->beginGroup("server " + serverName);
    configFile->setValue("start", startCommand);
    configFile->endGroup();
}

QString ServerManager::getStopCommand(QString serverName)
{
    configFile->beginGroup("server " + serverName);
    QString serverStop = configFile->value("stop").toString();
    configFile->endGroup();
    return serverStop;
}

void ServerManager::setStopCommand(QString serverName, QString stopCommand)
{
    configFile->beginGroup("server " + serverName);
    configFile->setValue("stop", stopCommand);
    configFile->endGroup();
}

QString ServerManager::getConfigCommand(QString serverName)
{
    configFile->beginGroup("server " + serverName);
    QString serverConfig = configFile->value("config").toString();
    configFile->endGroup();
    return serverConfig;
}

void ServerManager::setConfigCommand(QString serverName, QString configCommand)
{
    configFile->beginGroup("server " + serverName);
    configFile->setValue("config", configCommand);
    configFile->endGroup();
}

QString ServerManager::getUpdateCommand(QString serverName)
{
    configFile->beginGroup("server " + serverName);
    QString serverUpdate = configFile->value("update").toString();
    configFile->endGroup();
    return serverUpdate;
}

void ServerManager::setUpdateCommand(QString serverName, QString updateCommand)
{
    configFile->beginGroup("server " + serverName);
    configFile->setValue("update", updateCommand);
    configFile->endGroup();
}

QString ServerManager::getAttachCommand(QString serverName)
{
    configFile->beginGroup("server " + serverName);
    QString serverAttach = configFile->value("attach").toString();
    configFile->endGroup();
    return serverAttach;
}

void ServerManager::setAttachCommand(QString serverName, QString attachCommand)
{
    configFile->beginGroup("server " + serverName);
    configFile->setValue("attach", attachCommand);
    configFile->endGroup();
}

bool ServerManager::startServer(QString serverName)
{
    configFile->beginGroup("server " + serverName);
    QString serverStart = configFile->value("start","").toString();
    configFile->endGroup();
    if (serverStart != "")
    {
        QProcess serverProcess;
        serverProcess.startDetached(serverStart);
        return true;
    }
    return false;
}

bool ServerManager::stopServer(QString serverName)
{
    configFile->beginGroup("server " + serverName);
    QString serverStop = configFile->value("stop","").toString();
    configFile->endGroup();
    if (serverStop != "")
    {
        QProcess serverProcess;
        serverProcess.startDetached(serverStop);
        return true;
    }
    return false;
}

bool ServerManager::configServer(QString serverName)
{
    configFile->beginGroup("server " + serverName);
    QString serverConfig = configFile->value("config","").toString();
    configFile->endGroup();
    if (serverConfig != "")
    {
        QProcess serverProcess;
        serverProcess.startDetached(serverConfig);
        return true;
    }
    return false;
}

bool ServerManager::updateServer(QString serverName)
{
    configFile->beginGroup("server " + serverName);
    QString serverUpdate = configFile->value("update","").toString();
    configFile->endGroup();
    if (serverUpdate != "")
    {
        QProcess serverProcess;
        serverProcess.startDetached(serverUpdate);
        return true;
    }
    return false;
}

bool ServerManager::attachServer(QString serverName)
{
    configFile->beginGroup("server " + serverName);
    QString serverAttach = configFile->value("attach","").toString();
    configFile->endGroup();
    if (serverAttach != "")
    {
        QProcess serverProcess;
        serverProcess.startDetached(serverAttach);
        return true;
    }
    return false;
}

void ServerManager::setIconPath(QString serverName, QString iconPath)
{
    configFile->beginGroup("server " + serverName);
    configFile->setValue("icon", iconPath);
    configFile->endGroup();
}

QString ServerManager::getAdminPasswordHash()
{
    configFile->beginGroup("auth");
    QString passwordHash = configFile->value("password","").toString();
    configFile->endGroup();
    return passwordHash;
}

void ServerManager::setAdminPasswordHash(QString passwordHash)
{
    configFile->beginGroup("auth");
    configFile->setValue("password", passwordHash);
    configFile->endGroup();
}

void ServerManager::setAdminPassword(QString password)
{
    configFile->beginGroup("auth");
    configFile->setValue("password", getPasswordHashFromString(password));
    configFile->endGroup();
}

QString ServerManager::getPasswordHashFromString(QString password)
{
    QCryptographicHash *pwHash = new QCryptographicHash(QCryptographicHash::Md4);
    pwHash->addData(password.toUtf8());
    QString hashString = QString::fromUtf8(pwHash->result().toBase64());
    return hashString;
}
