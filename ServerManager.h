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
public:
    explicit ServerManager(QObject *parent = 0);
    QStringList getServerList();
    QString getIconPath(QString serverName);
    bool addServer(QString serverName);
    void deleteServer(QString serverName);
    QString getStartCommand(QString serverName);
    void setStartCommand(QString serverName, QString startCommand);
    QString getStopCommand(QString serverName);
    void setStopCommand(QString serverName, QString stopCommand);
    QString getConfigCommand(QString serverName);
    void setConfigCommand(QString serverName, QString configCommand);
    QString getUpdateCommand(QString serverName);
    void setUpdateCommand(QString serverName, QString updateCommand);
    QString getAttachCommand(QString serverName);
    void setAttachCommand(QString serverName, QString attachCommand);
    bool startServer(QString serverName);
    bool stopServer(QString serverName);
    bool configServer(QString serverName);
    bool updateServer(QString serverName);
    bool attachServer(QString serverName);
    void setIconPath(QString serverName, QString iconPath);
    QString getAdminPasswordHash();
    void setAdminPasswordHash(QString passwordHash);
    void setAdminPassword(QString password);
    QString getPasswordHashFromString(QString password);

signals:

public slots:

private:
    bool isAdmin;
    QSettings *configFile;
};

#endif // SERVERMANAGER_H
