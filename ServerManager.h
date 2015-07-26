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

#ifdef DISABLE_SSL
#include <QTcpSocket>
#else
#include <QSslSocket>
#endif

class ServerManager : public QObject
{
    Q_OBJECT
    Q_ENUMS(ServerManagerMode)

public:
    explicit ServerManager(QObject *parent = 0);

    // Server Manager Public
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
    bool isConnected();
    bool isConnecting();
    bool connectToServer(QString hostname, QString password, int port = 9509, bool useSSL = false);
    void disconnectFromServer();
    bool autologinEnabled();
    void setAutologinEnabled(QString hostname, QString password, int port = 9509, bool useSSL = false);
    void setAutologinDisabled();
    bool connectToServerWithAutologin();
    bool isConnectionLocal();
    QString getCurrentSessionHostname();
    QString getCurrentSessionPassword();
    int getCurrentSessionPort();
    bool getCurrentSessionSSL();

    // Server Manager Mode
    enum SMMode
    {
        LocalMode,
        RemoteMode
    };
    SMMode ServerManagerMode;

private:

    // Server Manager LocalMode
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

    // Server Manager RemoteMode
    QStringList getServerListRemote();
    QString getIconPathRemote(QString serverName);
    bool addServerRemote(QString serverName);
    bool deleteServerRemote(QString serverName);
    QString getStartCommandRemote(QString serverName);
    bool setStartCommandRemote(QString serverName, QString startCommand);
    QString getStopCommandRemote(QString serverName);
    bool setStopCommandRemote(QString serverName, QString stopCommand);
    QString getConfigCommandRemote(QString serverName);
    bool setConfigCommandRemote(QString serverName, QString configCommand);
    QString getUpdateCommandRemote(QString serverName);
    bool setUpdateCommandRemote(QString serverName, QString updateCommand);
    QString getAttachCommandRemote(QString serverName);
    bool setAttachCommandRemote(QString serverName, QString attachCommand);
    bool startServerRemote(QString serverName);
    bool stopServerRemote(QString serverName);
    bool configServerRemote(QString serverName);
    bool updateServerRemote(QString serverName);
    bool attachServerRemote(QString serverName);
    bool setIconPathRemote(QString serverName, QString iconPath);
    QString getAdminPasswordHashRemote();
    bool setAdminPasswordHashRemote(QString passwordHash);
    bool setAdminPasswordRemote(QString password);
    QStringList getArgsFromReturnRemote();

private:
    QSettings *configFile;
#ifdef DISABLE_SSL
    QTcpSocket *tcpSocket;
#else
    QSslSocket *tcpSocket;
#endif
    QString sessionHost;
    QString sessionPasswd;
    int sessionPort;
    bool sessionSSL;
    int lastReturn;
};

#endif // SERVERMANAGER_H
