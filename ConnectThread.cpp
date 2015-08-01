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

#include "ConnectThread.h"

ConnectThread::ConnectThread(QObject *parent, ServerManager *smgr, bool autologin, bool activateAutologin, QString hostname, int port, bool useSSL, QString password) :
    QThread(parent),
    smgr(smgr),
    autologin(autologin),
    activateAutologin(activateAutologin),
    serverHost(hostname),
    serverPort(port),
    serverSSL(useSSL),
    serverPasswd(password)
{
}

void ConnectThread::run()
{
    if (smgr != 0)
    {
        if (autologin)
        {
            bool connectionSuccess = smgr->connectToServerWithAutologin();
            emit connectionIssued(connectionSuccess);
        }
        else if (serverHost == "SM_LOCAL")
        {
            smgr->ServerManagerMode = ServerManager::LocalMode;
            if (activateAutologin)
            {
                smgr->setAutologinEnabled(serverHost, serverPasswd, serverPort, serverSSL);
            }
            emit connectionIssued(true);
        }
        else if (serverHost == "")
        {
            smgr->ServerManagerMode = ServerManager::LocalMode;
            if (activateAutologin)
            {
                smgr->setAutologinEnabled(serverHost, serverPasswd, serverPort, serverSSL);
            }
            emit connectionIssued(true);
        }
        else
        {
            smgr->ServerManagerMode = ServerManager::RemoteMode;
            bool connectionSuccess = smgr->connectToServer(serverHost, serverPasswd, serverPort, serverSSL);
            if (activateAutologin)
            {
                smgr->setAutologinEnabled(serverHost, serverPasswd, serverPort, serverSSL);
            }
            emit connectionIssued(connectionSuccess);
        }
    }
    else
    {
        emit connectionIssued(false);
    }
}
