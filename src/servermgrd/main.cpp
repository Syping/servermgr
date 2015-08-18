/*****************************************************************************
* servermgrd Syping Gaming Team Server Manager Server
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
#include "server.h"

#include <QApplication>
#include <QSettings>
#include <iostream>
#include <QDir>
using namespace std;

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);

    // Set Dir
    QDir::setCurrent(QFileInfo(a.applicationFilePath()).absoluteDir().absolutePath());

    // Read Settings
    QSettings settings("sm.ini",QSettings::IniFormat);
    settings.beginGroup("Server");
    int port = settings.value("Port",9509).toInt();
    bool useSSL = false;
#ifndef DISABLE_SSL
    useSSL = settings.value("EnableSSL",false).toBool();
#else
    Q_UNUSED(useSSL);
#endif
    settings.endGroup();

    // Define ServerManager
    ServerManager *smgr = new ServerManager();
    smgr->ServerManagerMode = ServerManager::LocalMode;

    // Start Server
    server *nServer = new server(smgr, useSSL);
#ifdef QT5
    if (!nServer->listen(QHostAddress::AnyIPv4, port))
    {
        qFatal("Can't bind Server on port: " + port);
    }
#else
    if (!nServer->listen(QHostAddress::Any, port))
    {
        qFatal("Can't bind Server on port: " + port);
    }
#endif
    else
    {
        server *v6Server = new server(smgr, useSSL);
        if (!v6Server->listen(QHostAddress::AnyIPv6, port))
        {
            qDebug() << "IPv6 bind failed, continue...";
        }
        cout << "---------------------------------\n";
        cout << "|                               |\n";
        cout << "|       Syping SM Server        |\n";
#if defined(Q_WS_X11)
        cout << "|       Version 1.1.2 LR1       |\n";
#else
        cout << "|       Version 1.1.2 R1        |\n";
#endif
        cout << "|                               |\n";
        cout << "|       Port: ";
        int portlen = QString(QString::number(port) + " (TCP)").length();
        cout << QString(QString::number(port) + " (TCP)").toStdString();
        while (portlen != 18)
        {
            portlen++;
            cout << " ";
        }
        cout << "|\n";
        cout << "|                               |\n";
        cout << "---------------------------------\n\n";
    }

    cout << "Server intialized\n";

    return a.exec();
}
