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
    ServerManagerMode = LocalMode;
}

QStringList ServerManager::getServerList()
{
    if (ServerManagerMode == LocalMode)
    {
        return getServerListLocal();
    }
    else
    {

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

    }
}

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

QString ServerManager::getPasswordHashFromString(QString password)
{
    QCryptographicHash *pwHash = new QCryptographicHash(QCryptographicHash::Md4);
    pwHash->addData(password.toUtf8());
    QString hashString = QString::fromUtf8(pwHash->result().toBase64());
    return hashString;
}
