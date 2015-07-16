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

#ifndef SERVERMANAGER_H
#define SERVERMANAGER_H

#include <QStringList>
#include <QSettings>
#include <QObject>

class ServerManager : public QObject
{
    Q_OBJECT
    Q_ENUMS(ServerManagerMode)

public:
    explicit ServerManager(QObject *parent = 0);
    QStringList getServerList();
    QString getIconPath(QString serverName);
    bool addServer(QString serverName);
    bool deleteServer(QString serverName);
    QString getStartCommand(QString serverName);
    bool setStartCommand(QString serverName, QString startCommand);
    QString getStopCommand(QString serverName);
    bool setStopCommand(QString serverName, QString stopCommand);
    QString getConfigCommand(QString serverName);
    bool setConfigCommand(QString serverName, QString configCommand);
    QString getUpdateCommand(QString serverName);
    bool setUpdateCommand(QString serverName, QString updateCommand);
    QString getAttachCommand(QString serverName);
    bool setAttachCommand(QString serverName, QString attachCommand);
    bool startServer(QString serverName);
    bool stopServer(QString serverName);
    bool configServer(QString serverName);
    bool updateServer(QString serverName);
    bool attachServer(QString serverName);
    bool setIconPath(QString serverName, QString iconPath);
    QString getAdminPasswordHash();
    bool setAdminPasswordHash(QString passwordHash);
    bool setAdminPassword(QString password);
    QString getPasswordHashFromString(QString password);

    // Server Manager Mode
    enum SMMode
    {
        LocalMode,
        RemoteMode
    };
    SMMode ServerManagerMode;

private:
    QStringList getServerListLocal();
    QString getIconPathLocal(QString serverName);
    bool addServerLocal(QString serverName);
    void deleteServerLocal(QString serverName);
    QString getStartCommandLocal(QString serverName);
    void setStartCommandLocal(QString serverName, QString startCommand);
    QString getStopCommandLocal(QString serverName);
    void setStopCommandLocal(QString serverName, QString stopCommand);
    QString getConfigCommandLocal(QString serverName);
    void setConfigCommandLocal(QString serverName, QString configCommand);
    QString getUpdateCommandLocal(QString serverName);
    void setUpdateCommandLocal(QString serverName, QString updateCommand);
    QString getAttachCommandLocal(QString serverName);
    void setAttachCommandLocal(QString serverName, QString attachCommand);
    bool startServerLocal(QString serverName);
    bool stopServerLocal(QString serverName);
    bool configServerLocal(QString serverName);
    bool updateServerLocal(QString serverName);
    bool attachServerLocal(QString serverName);
    void setIconPathLocal(QString serverName, QString iconPath);
    QString getAdminPasswordHashLocal();
    void setAdminPasswordHashLocal(QString passwordHash);
    void setAdminPasswordLocal(QString password);

signals:

public slots:

private:
    QSettings *configFile;
};

#endif // SERVERMANAGER_H
