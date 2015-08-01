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

#ifndef CONNECTTHREAD_H
#define CONNECTTHREAD_H

#include "ServerManager.h"
#include <QObject>
#include <QThread>

class ConnectThread : public QThread
{
    Q_OBJECT

public:
    explicit ConnectThread(QObject *parent = 0, ServerManager *smgr = 0, bool autologin = false, bool activateAutologin = false, QString hostname = "SM_LOCAL", int port = 9509, bool useSSL = false, QString password = "");

private:
    ServerManager *smgr;
    bool autologin;
    bool activateAutologin;
    QString serverHost;
    int serverPort;
    bool serverSSL;
    QString serverPasswd;

signals:
    void connectionIssued(bool isSuccess);

protected:
    void run();
};

#endif // CONNECTTHREAD_H
