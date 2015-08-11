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

#include "core.h"
#include "server.h"

server::server(ServerManager *smgr, bool useSSL, QObject *parent) :
    QTcpServer(parent), smgr(smgr), useSSL(useSSL)
{
}

void server::incomingConnection(int handle)
{
    qDebug() << "Client incoming";
    core *cCore = new core(handle, smgr, useSSL, this);
    connect(cCore, SIGNAL(finished()), cCore, SLOT(deleteLater()));
    cCore->start();
}