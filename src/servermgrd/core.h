/*****************************************************************************
* servermgrd Syping Gaming Team Server Manager Server
* Copyright (C) 2015-2016 Syping Gaming Team
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

#ifndef CORE_H
#define CORE_H

#include "ServerManager.h"

#include <QThread>
#include <QObject>
#include <QTcpSocket>
#ifndef DISABLE_SSL
#include <QSslCertificate>
#endif

#include <iostream>
using namespace std;

class core : public QThread
{
    Q_OBJECT
public:
    core(qintptr handle, ServerManager *smgr, bool useSSL, QString pemFile, QString keyFile, QString caFile, QObject *parent = 0);
    void run();

private:
    qintptr handle;
    ServerManager *smgr;
    bool useSSL;
    QString pemFile;
    QString keyFile;
    QString caFile;

    bool workInThread;
    string clientIP;
    QString qclientIP;
    QString clientVersion;
    QList<QSslCertificate> sslca;
    void readThread(QObject *socket);
    QString toSMEscape(const QString &string);
    QString fromSMEscape(const QString &string);


signals:
    void error(QTcpSocket::SocketError socketError);
};

#endif // CORE_H
