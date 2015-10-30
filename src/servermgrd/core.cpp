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

#include "PixmapEdit.h"
#include "core.h"

#include <QHostAddress>
#include <QStringList>
#include <QPixmap>
#include <QString>
#include <QBuffer>
#include <QDir>

#include <iostream>
using namespace std;

core::core(qintptr handle, ServerManager *smgr, bool useSSL, QObject *parent) :
    QThread(parent), handle(handle), smgr(smgr), useSSL(useSSL)
{
    sslca = QSslCertificate::fromPath(QLatin1String("ca.pem"));
    workInThread = true;
}

void core::run()
{

#ifdef DISABLE_SSL
    QTcpSocket *client = new QTcpSocket();
#else
    QSslSocket *client = new QSslSocket();
#endif

    if (!client->setSocketDescriptor(handle))
    {
        emit error(client->error());
        return;
    }

    qclientIP = client->peerAddress().toString();
    if (qclientIP.right(2) == "%0")
    {
        qclientIP.remove(qclientIP.length() - 2,2);
        qclientIP.insert(0,"[");
        qclientIP.insert(qclientIP.length(),"]");
    }
    clientIP = qclientIP.toStdString();
    cout << clientIP + ": core connect\n";

    if (useSSL)
    {
        client->setPrivateKey("server.key", QSsl::Rsa,QSsl::Pem);
        client->setLocalCertificate("server.pem", QSsl::Pem);
        client->setCaCertificates(sslca);
        client->startServerEncryption();
        client->waitForEncrypted(30000);
        if (client->isEncrypted())
        {
            cout << clientIP + ": core ssl\n";
        }
        else
        {
            cout << clientIP + ": core ssl failed\n";
            client->disconnectFromHost();
            return;
        }
    }

    client->waitForReadyRead(5000);
    cout << clientIP + ": core wait read\n";

    if (client->canReadLine())
    {
        cout << clientIP + ": core go read\n";
        QByteArray readed = client->readLine().trimmed();
        QString readstr = QString::fromUtf8(readed);
        QStringList readlist = readstr.split(" ");
        QString method = "";
        QString address = "";
        QString protocol = "";
        if (readlist.length() >= 3)
        {
            method = readlist.at(0);
            address = readlist.at(1);
            protocol = readlist.at(2);
        }
        else if (readlist.length() >= 2)
        {
            method = readlist.at(0);
            address = readlist.at(1);
        }
        else if (readlist.length() >= 1)
        {
            method = readlist.at(0);
            client->write("SM/1.1 600 Don't Understand\n");
            client->flush();
            client->disconnectFromHost();
            cout << clientIP + ": core disconnect 600\n";
            return;
        }
        else
        {
            client->write("SM/1.1 600 Don't Understand\n");
            client->flush();
            client->disconnectFromHost();
            cout << clientIP + ": core disconnect 600\n";
            return;
        }
        if (method == "SM/1.1" && address == "login")
        {
            if (readlist.length() >= 2)
            {
                cout << clientIP + ": login\n";
                QString password;
                QString version = "1.1";
                foreach(QString argstr,readlist)
                {
                    if (argstr.left(11) == "--password=")
                    {
                        password = fromSMEscape(argstr.remove(0,11));
                        cout << clientIP + ": Sended Password: " + password.toStdString() + "\n";
                    }
                    if (argstr.left(10) == "--version=")
                    {
                        version = fromSMEscape(argstr.remove(0,10));
                        cout << clientIP + ": Sended Version: " + version.toStdString() + "\n";
                    }
                }
                clientVersion = version;
                if (smgr->getPasswordHashFromString(password) == smgr->getAdminPasswordHash())
                {
                    client->write("SM/1.1 return --at=login --id=200 --description=Success\n");
                    client->flush();
                    cout << clientIP + ": core connected 200\n";
                    readThread(client);
                    return;
                }
                else
                {
                    client->write("SM/1.1 return --at=login --id=404 --description=Not&nbsp;Found\n");
                    client->flush();
                    cout << clientIP + ": core disconnected 404\n";
                    client->disconnectFromHost();
                    return;
                }
            }
            cout << clientIP + ": core disconnected ep\n";
            client->write("SM/1.1 return --at=connect --id=ep --description=Error&nbsp;in&nbsp;Protocol\n");
            client->flush();
            client->disconnectFromHost();
            return;
        }
        cout << clientIP + ": core disconnected up\n";
        client->write("SM/1.1 return --at=connect --id=up --description=Unknown&nbsp;Protocol\n");
        client->flush();
        client->disconnectFromHost();
        return;
    }
    else
    {
        cout << clientIP + ": core disconnected nr\n";
        client->write("SM/1.1 return --at=connect --id=nr --description=No&nbsp;Response\n");
        client->flush();
        client->disconnectFromHost();
        return;
    }
}

void core::readThread(QObject *socket)
{
    qDebug() << "Success login";
#ifndef DISABLE_SSL
    QSslSocket *client = (QSslSocket*)socket;
#else
    QTcpSocket *client = (QTcpSocket*)socket;
#endif
    while(workInThread)
    {;
        client->waitForReadyRead(1000);
        if (client->canReadLine())
        {
            cout << clientIP + ": thread go read\n";
            QByteArray readed = client->readLine().trimmed();
            QString readstr = QString::fromUtf8(readed);
            QStringList readlist = readstr.split(" ");
            if (readlist.at(0) == "SM/1.1" && readlist.at(1) == "action")
            {
                QString action;
                QString arg1;
                QString arg2;
                bool retV;
                foreach(QString argstr, readlist)
                {
                    if (argstr.left(5) == "--at=")
                    {
                        action = fromSMEscape(argstr.remove(0,5));
                    }
                    if (argstr.left(7) == "--arg1=")
                    {
                        arg1 = fromSMEscape(argstr.remove(0,7));
                    }
                    if (argstr.left(7) == "--arg2=")
                    {
                        arg2 = fromSMEscape(argstr.remove(0,7));
                    }
                }
                if (action == "addserver")
                {
                    retV = smgr->addServer(arg1);
                }
                else if (action == "delserver")
                {
                    retV = smgr->deleteServer(arg1);
                }
                else if (action == "setstartcmd")
                {
                    retV = smgr->setStartCommand(arg1, arg2);
                }
                else if (action == "setstopcmd")
                {
                    retV = smgr->setStopCommand(arg1, arg2);
                }
                else if (action == "setconfigcmd")
                {
                    retV = smgr->setConfigCommand(arg1, arg2);
                }
                else if (action == "setupdatecmd")
                {
                    retV = smgr->setUpdateCommand(arg1, arg2);
                }
                else if (action == "setattachcmd")
                {
                    retV = smgr->setAttachCommand(arg1, arg2);
                }
                else if (action == "startserver")
                {
                    retV = smgr->startServer(arg1);
                }
                else if (action == "stopserver")
                {
                    retV = smgr->stopServer(arg1);
                }
                else if (action == "configserver")
                {
                    retV = smgr->configServer(arg1);
                }
                else if (action == "updateserver")
                {
                    retV = smgr->updateServer(arg1);
                }
                else if (action == "attachserver")
                {
                    retV = smgr->attachServer(arg1);
                }
                else if (action == "seticonpath")
                {
                    retV = smgr->setIconPath(arg1, arg2);
                }
                else if (action == "setadminpw")
                {
                    retV = smgr->setAdminPassword(arg1);
                }
                else if (action == "setadminpwhash")
                {
                    retV = smgr->setAdminPasswordHash(arg1);
                }
                else
                {
                    retV = false;
                }
                QString retS = "300";
                if (retV) { retS = "200"; }
                client->write(QString("SM/1.1 return --at=" + toSMEscape(action) + " --id=" + retS + "\n").toUtf8());
                client->flush();
            }
            else if (readlist.at(0) == "SM/1.1" && readlist.at(1) == "request")
            {
                QString request;
                QString arg1;
                QString arg2;
                QString retV;
                QString retS = "200";
                foreach(QString argstr, readlist)
                {
                    if (argstr.left(5) == "--rq=")
                    {
                        request = fromSMEscape(argstr.remove(0,5));
                    }
                    if (argstr.left(7) == "--arg1=")
                    {
                        arg1 = fromSMEscape(argstr.remove(0,7));
                    }
                    if (argstr.left(7) == "--arg2=")
                    {
                        arg2 = fromSMEscape(argstr.remove(0,7));
                    }
                }
                if (request == "getserverlist")
                {
                    retV = smgr->getServerList().join("&sms;");
                }
                else if (request == "geticonbytes")
                {
                    retV = smgr->getIconBytes(arg1, arg2.toInt());
                    qDebug() << retV << "retV";
                    if (retV == "")
                    {
                        retS = "300";
                    }
                }
                else if (request == "geticonpath")
                {
                    if (clientIP == "127.0.0.1" || clientIP == "[::1:]" || clientIP == "::1:" || client->peerAddress() == client->localAddress())
                    {
                        retV = smgr->getIconPath(arg1);
                    }
                    else
                    {
                        retV = smgr->getIconPath(arg1);
                    }
                }
                else if (request == "getstartcmd")
                {
                    retV = smgr->getStartCommand(arg1);
                }
                else if (request == "getstopcmd")
                {
                    retV = smgr->getStopCommand(arg1);
                }
                else if (request == "getconfigcmd")
                {
                    retV = smgr->getConfigCommand(arg1);
                }
                else if (request == "getupdatecmd")
                {
                    retV = smgr->getUpdateCommand(arg1);
                }
                else if (request == "getattachcmd")
                {
                    retV = smgr->getAttachCommand(arg1);
                }
                else if (request == "getadminpwhash")
                {
                    retV = smgr->getAdminPasswordHash();
                }
                else
                {
                    retV = "";
                    retS = "300";
                }
                qDebug() << QString("SM/1.1 return --rq=" + toSMEscape(request) + " --id=" + retS + " --arg1=" + toSMEscape(retV) + "\n").toUtf8() << "line";
                client->write(QString("SM/1.1 return --rq=" + toSMEscape(request) + " --id=" + retS + " --arg1=" + toSMEscape(retV) + "\n").toUtf8());
                client->flush();
            }
            else if (readlist.at(0) == "SM/1.1" && readlist.at(1) == "disconnect")
            {
                cout << clientIP + ": core disconnected dk\n";
                client->disconnectFromHost();
                workInThread = false;
            }
            else
            {
                client->write("SM/1.1 return --at=unknown --rq=unknown --id=ep --description=Error&nbsp;in&nbsp;Protocol\n");
                client->flush();
            }
        }
        else if (client->state() == QTcpSocket::UnconnectedState)
        {
            cout << clientIP + ": core disconnected nr\n";
            workInThread = false;
        }
    }
}

QString core::toSMEscape(const QString &SMstring)
{
    QString newString = SMstring;
    newString = newString.replace("&","&amp;");
    newString = newString.replace("\n","&br;");
    newString = newString.replace(" ","&nbsp;");
    return newString;
}

QString core::fromSMEscape(const QString &SMstring)
{
    QString newString = SMstring;
    return newString.replace("&nbsp;"," ").replace("&br;","\n").replace("&amp;","&");
}

