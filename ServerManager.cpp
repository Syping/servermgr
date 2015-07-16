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
#ifdef DISABLE_SSL
    tcpSocket = new QTcpSocket();
#else
    tcpSocket = new QSslSocket();
#endif
    ServerManagerMode = LocalMode;
}

// Server Manager Public

QStringList ServerManager::getServerList()
{
    if (ServerManagerMode == LocalMode)
    {
        return getServerListLocal();
    }
    else
    {
        return getServerListRemote();
    }
}

QString ServerManager::getIconPath(QString serverName)
{
    if (ServerManagerMode == LocalMode)
    {
        return getIconPathLocal(serverName);
    }
    else
    {
        return getIconPathRemote(serverName);
    }
}

bool ServerManager::addServer(QString serverName)
{
    if (ServerManagerMode == LocalMode)
    {
        return addServerLocal(serverName);
    }
    else
    {
        return addServerRemote(serverName);
    }
}

bool ServerManager::deleteServer(QString serverName)
{
    if (ServerManagerMode == LocalMode)
    {
        deleteServerLocal(serverName);
        return true;
    }
    else
    {
        return deleteServerRemote(serverName);
    }
}

QString ServerManager::getStartCommand(QString serverName)
{
    if (ServerManagerMode == LocalMode)
    {
        return getStartCommandLocal(serverName);
    }
    else
    {
        return getStartCommandRemote(serverName);
    }
}

bool ServerManager::setStartCommand(QString serverName, QString startCommand)
{
    if (ServerManagerMode == LocalMode)
    {
        setStartCommandLocal(serverName, startCommand);
        return true;
    }
    else
    {
        return setStartCommandRemote(serverName, startCommand);
    }
}

QString ServerManager::getStopCommand(QString serverName)
{
    if (ServerManagerMode == LocalMode)
    {
        return getStopCommandLocal(serverName);
    }
    else
    {
        return getStopCommandRemote(serverName);
    }
}

bool ServerManager::setStopCommand(QString serverName, QString stopCommand)
{
    if (ServerManagerMode == LocalMode)
    {
        setStopCommandLocal(serverName, stopCommand);
        return true;
    }
    else
    {
        return setStopCommandRemote(serverName, stopCommand);
    }
}

QString ServerManager::getConfigCommand(QString serverName)
{
    if (ServerManagerMode == LocalMode)
    {
        return getConfigCommandLocal(serverName);
    }
    else
    {
        return getConfigCommandRemote(serverName);
    }
}

bool ServerManager::setConfigCommand(QString serverName, QString configCommand)
{
    if (ServerManagerMode == LocalMode)
    {
        setConfigCommandLocal(serverName, configCommand);
        return true;
    }
    else
    {
        return setConfigCommandRemote(serverName, configCommand);
    }
}

QString ServerManager::getUpdateCommand(QString serverName)
{
    if (ServerManagerMode == LocalMode)
    {
        return getUpdateCommandLocal(serverName);
    }
    else
    {
        return getUpdateCommandRemote(serverName);
    }
}

bool ServerManager::setUpdateCommand(QString serverName, QString updateCommand)
{
    if (ServerManagerMode == LocalMode)
    {
        setUpdateCommandLocal(serverName, updateCommand);
        return true;
    }
    else
    {
        return setUpdateCommandRemote(serverName, updateCommand);
    }
}

QString ServerManager::getAttachCommand(QString serverName)
{
    if (ServerManagerMode == LocalMode)
    {
        return getAttachCommandLocal(serverName);
    }
    else
    {
        return getAttachCommandRemote(serverName);
    }
}

bool ServerManager::setAttachCommand(QString serverName, QString attachCommand)
{
    if (ServerManagerMode == LocalMode)
    {
        setAttachCommandLocal(serverName, attachCommand);
        return true;
    }
    else
    {
        return setAttachCommandRemote(serverName, attachCommand);
    }
}

bool ServerManager::startServer(QString serverName)
{
    if (ServerManagerMode == LocalMode)
    {
        return startServerLocal(serverName);
    }
    else
    {
        return startServerRemote(serverName);
    }
}

bool ServerManager::stopServer(QString serverName)
{
    if (ServerManagerMode == LocalMode)
    {
        return stopServerLocal(serverName);
    }
    else
    {
        return stopServerRemote(serverName);
    }
}

bool ServerManager::configServer(QString serverName)
{
    if (ServerManagerMode == LocalMode)
    {
        return configServerLocal(serverName);
    }
    else
    {
        return configServerRemote(serverName);
    }
}

bool ServerManager::updateServer(QString serverName)
{
    if (ServerManagerMode == LocalMode)
    {
        return updateServerLocal(serverName);
    }
    else
    {
        return updateServerRemote(serverName);
    }
}

bool ServerManager::attachServer(QString serverName)
{
    if (ServerManagerMode == LocalMode)
    {
        return attachServerLocal(serverName);
    }
    else
    {
        return attachServerRemote(serverName);
    }
}

bool ServerManager::setIconPath(QString serverName, QString iconPath)
{
    if (ServerManagerMode == LocalMode)
    {
        setIconPathLocal(serverName, iconPath);
        return true;
    }
    else
    {
        return setIconPathRemote(serverName, iconPath);
    }
}

QString ServerManager::getAdminPasswordHash()
{
    if (ServerManagerMode == LocalMode)
    {
        return getAdminPasswordHashLocal();
    }
    else
    {
        return getAdminPasswordHashRemote();
    }
}

bool ServerManager::setAdminPasswordHash(QString passwordHash)
{
    if (ServerManagerMode == LocalMode)
    {
        setAdminPasswordHashLocal(passwordHash);
        return true;
    }
    else
    {
        return setAdminPasswordRemote(passwordHash);
    }
}

bool ServerManager::setAdminPassword(QString password)
{
    if (ServerManagerMode == LocalMode)
    {
        setAdminPasswordLocal(password);
        return true;
    }
    else
    {
        return setAdminPasswordRemote(password);
    }
}

QString ServerManager::getPasswordHashFromString(QString password)
{
    QCryptographicHash *pwHash = new QCryptographicHash(QCryptographicHash::Md4);
    pwHash->addData(password.toUtf8());
    QString hashString = QString::fromUtf8(pwHash->result().toBase64());
    return hashString;
}

bool ServerManager::isConnected()
{
    if (ServerManagerMode == LocalMode)
    {
        return true;
    }
    else
    {
        if (tcpSocket->state() == QAbstractSocket::ConnectedState)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
}

bool ServerManager::isConnecting()
{
    if (ServerManagerMode == LocalMode)
    {
        return true;
    }
    else
    {
        if (tcpSocket->state() == QAbstractSocket::ConnectedState)
        {
            return true;
        }
        else
        {
            if (tcpSocket->state() == QAbstractSocket::ConnectingState)
            {
                return true;
            }
            else
            {
                return false;
            }
        }
    }
}

// Server Manager LocalMode

QStringList ServerManager::getServerListLocal()
{
    return configFile->value("serverList",QStringList()).toStringList();
}

QString ServerManager::getIconPathLocal(QString serverName)
{
    configFile->beginGroup("server " + serverName);
    QString iconPath = configFile->value("icon",ProductImg).toString();
    configFile->endGroup();
    return iconPath;
}

bool ServerManager::addServerLocal(QString serverName)
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

void ServerManager::deleteServerLocal(QString serverName)
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

QString ServerManager::getStartCommandLocal(QString serverName)
{
    configFile->beginGroup("server " + serverName);
    QString serverStart = configFile->value("start").toString();
    configFile->endGroup();
    return serverStart;
}

void ServerManager::setStartCommandLocal(QString serverName, QString startCommand)
{
    configFile->beginGroup("server " + serverName);
    configFile->setValue("start", startCommand);
    configFile->endGroup();
}

QString ServerManager::getStopCommandLocal(QString serverName)
{
    configFile->beginGroup("server " + serverName);
    QString serverStop = configFile->value("stop").toString();
    configFile->endGroup();
    return serverStop;
}

void ServerManager::setStopCommandLocal(QString serverName, QString stopCommand)
{
    configFile->beginGroup("server " + serverName);
    configFile->setValue("stop", stopCommand);
    configFile->endGroup();
}

QString ServerManager::getConfigCommandLocal(QString serverName)
{
    configFile->beginGroup("server " + serverName);
    QString serverConfig = configFile->value("config").toString();
    configFile->endGroup();
    return serverConfig;
}

void ServerManager::setConfigCommandLocal(QString serverName, QString configCommand)
{
    configFile->beginGroup("server " + serverName);
    configFile->setValue("config", configCommand);
    configFile->endGroup();
}

QString ServerManager::getUpdateCommandLocal(QString serverName)
{
    configFile->beginGroup("server " + serverName);
    QString serverUpdate = configFile->value("update").toString();
    configFile->endGroup();
    return serverUpdate;
}

void ServerManager::setUpdateCommandLocal(QString serverName, QString updateCommand)
{
    configFile->beginGroup("server " + serverName);
    configFile->setValue("update", updateCommand);
    configFile->endGroup();
}

QString ServerManager::getAttachCommandLocal(QString serverName)
{
    configFile->beginGroup("server " + serverName);
    QString serverAttach = configFile->value("attach").toString();
    configFile->endGroup();
    return serverAttach;
}

void ServerManager::setAttachCommandLocal(QString serverName, QString attachCommand)
{
    configFile->beginGroup("server " + serverName);
    configFile->setValue("attach", attachCommand);
    configFile->endGroup();
}

bool ServerManager::startServerLocal(QString serverName)
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

bool ServerManager::stopServerLocal(QString serverName)
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

bool ServerManager::configServerLocal(QString serverName)
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

bool ServerManager::updateServerLocal(QString serverName)
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

bool ServerManager::attachServerLocal(QString serverName)
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

void ServerManager::setIconPathLocal(QString serverName, QString iconPath)
{
    configFile->beginGroup("server " + serverName);
    configFile->setValue("icon", iconPath);
    configFile->endGroup();
}

QString ServerManager::getAdminPasswordHashLocal()
{
    configFile->beginGroup("auth");
    QString passwordHash = configFile->value("password","").toString();
    configFile->endGroup();
    return passwordHash;
}

void ServerManager::setAdminPasswordHashLocal(QString passwordHash)
{
    configFile->beginGroup("auth");
    configFile->setValue("password", passwordHash);
    configFile->endGroup();
}

void ServerManager::setAdminPasswordLocal(QString password)
{
    configFile->beginGroup("auth");
    configFile->setValue("password", getPasswordHashFromString(password));
    configFile->endGroup();
}

// Server Manager RemoteMode

QStringList ServerManager::getServerListRemote()
{
    // Not finished
    return QStringList();
}

QString ServerManager::getIconPathRemote(QString serverName)
{
    // Not finished
    return QString("");
}

bool ServerManager::addServerRemote(QString serverName)
{
    // Not finished
    return false;
}

bool ServerManager::deleteServerRemote(QString serverName)
{
    // Not finished
    return false;
}

QString ServerManager::getStartCommandRemote(QString serverName)
{
    // Not finished
    return QString("");
}

bool ServerManager::setStartCommandRemote(QString serverName, QString startCommand)
{
    // Not finished
    return false;
}

QString ServerManager::getStopCommandRemote(QString serverName)
{
    // Not finished
    return QString("");
}

bool ServerManager::setStopCommandRemote(QString serverName, QString stopCommand)
{
    // Not finished
    return false;
}

QString ServerManager::getConfigCommandRemote(QString serverName)
{
    // Not finished
    return QString("");
}

bool ServerManager::setConfigCommandRemote(QString serverName, QString configCommand)
{
    // Not finished
    return false;
}

QString ServerManager::getUpdateCommandRemote(QString serverName)
{
    // Not finished
    return QString("");
}

bool ServerManager::setUpdateCommandRemote(QString serverName, QString updateCommand)
{
    // Not finished
    return false;
}

QString ServerManager::getAttachCommandRemote(QString serverName)
{
    // Not finished
    return QString("");
}

bool ServerManager::setAttachCommandRemote(QString serverName, QString attachCommand)
{
    // Not finished
    return false;
}

bool ServerManager::startServerRemote(QString serverName)
{
    // Not finished
    return false;
}

bool ServerManager::stopServerRemote(QString serverName)
{
    // Not finished
    return false;
}

bool ServerManager::configServerRemote(QString serverName)
{
    // Not finished
    return false;
}

bool ServerManager::updateServerRemote(QString serverName)
{
    // Not finished
    return false;
}

bool ServerManager::attachServerRemote(QString serverName)
{
    // Not finished
    return false;
}

bool ServerManager::setIconPathRemote(QString serverName, QString iconPath)
{
    // Not finished
    return false;
}

QString ServerManager::getAdminPasswordHashRemote()
{
    // Not finished
    return QString("");
}

bool ServerManager::setAdminPasswordHashRemote(QString passwordHash)
{
    // Not finished
    return false;
}

bool ServerManager::setAdminPasswordRemote(QString password)
{
    // Not finished
    return false;
}

