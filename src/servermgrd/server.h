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

#ifndef SERVER_H
#define SERVER_H

#include "ServerManager.h"

#include <QTcpServer>
#include <QObject>
#include <QMap>

class server : public QTcpServer
{
    Q_OBJECT
public:
    server(ServerManager *smgr, bool useSSL, QString pemFile, QString keyFile, QString caFile, QObject *parent = 0);

private:
    ServerManager *smgr;
    bool useSSL;
    QString pemFile;
    QString keyFile;
    QString caFile;

protected:
    void incomingConnection(qintptr handle);
};

#endif // SERVER_H
