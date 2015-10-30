/*****************************************************************************
* servermgrfw Syping Gaming Team Server Manager Framework
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
#include "PixmapEdit.h"
#include "config.h"
#include <QCryptographicHash>
#include <QHostAddress>
#include <QByteArray>
#include <QProcess>
#include <QBuffer>

ServerManager::ServerManager(QObject *parent) : QObject(parent)
{
    configFile = new QSettings(ProductVendor, ProductName);
#ifdef DISABLE_SSL
    tcpSocket = new QTcpSocket();
#else
    tcpSocket = new QSslSocket();
#endif
    ServerManagerMode = LocalMode;
    sessionHost = "SM_LOCAL";
    sessionPasswd = "";
    sessionPort = 9509;
    sessionSSL = false;
    lastReturn = 0;
}

// Server Manager Public

QStringList ServerManager::getServerList()
{
    if (ServerManagerMode == LocalMode)
    {
        return getServerListLocal();
    }
    else
    {
        return getServerListRemote();
    }
}

QString ServerManager::getIconPath(QString serverName)
{
    if (ServerManagerMode == LocalMode)
    {
        return getIconPathLocal(serverName);
    }
    else
    {
        return getIconPathRemote(serverName);
    }
}

QString ServerManager::getIconBytes(QString serverName, int squareSize)
{
    if (ServerManagerMode == LocalMode)
    {
        return getIconBytesLocal(serverName, squareSize);
    }
    else
    {
        return getIconBytesRemote(serverName, squareSize);
    }
}

bool ServerManager::addServer(QString serverName)
{
    if (ServerManagerMode == LocalMode)
    {
        return addServerLocal(serverName);
    }
    else
    {
        return addServerRemote(serverName);
    }
}

bool ServerManager::deleteServer(QString serverName)
{
    if (ServerManagerMode == LocalMode)
    {
        deleteServerLocal(serverName);
        return true;
    }
    else
    {
        return deleteServerRemote(serverName);
    }
}

QString ServerManager::getStartCommand(QString serverName)
{
    if (ServerManagerMode == LocalMode)
    {
        return getStartCommandLocal(serverName);
    }
    else
    {
        return getStartCommandRemote(serverName);
    }
}

bool ServerManager::setStartCommand(QString serverName, QString startCommand)
{
    if (ServerManagerMode == LocalMode)
    {
        setStartCommandLocal(serverName, startCommand);
        return true;
    }
    else
    {
        return setStartCommandRemote(serverName, startCommand);
    }
}

QString ServerManager::getStopCommand(QString serverName)
{
    if (ServerManagerMode == LocalMode)
    {
        return getStopCommandLocal(serverName);
    }
    else
    {
        return getStopCommandRemote(serverName);
    }
}

bool ServerManager::setStopCommand(QString serverName, QString stopCommand)
{
    if (ServerManagerMode == LocalMode)
    {
        setStopCommandLocal(serverName, stopCommand);
        return true;
    }
    else
    {
        return setStopCommandRemote(serverName, stopCommand);
    }
}

QString ServerManager::getConfigCommand(QString serverName)
{
    if (ServerManagerMode == LocalMode)
    {
        return getConfigCommandLocal(serverName);
    }
    else
    {
        return getConfigCommandRemote(serverName);
    }
}

bool ServerManager::setConfigCommand(QString serverName, QString configCommand)
{
    if (ServerManagerMode == LocalMode)
    {
        setConfigCommandLocal(serverName, configCommand);
        return true;
    }
    else
    {
        return setConfigCommandRemote(serverName, configCommand);
    }
}

QString ServerManager::getUpdateCommand(QString serverName)
{
    if (ServerManagerMode == LocalMode)
    {
        return getUpdateCommandLocal(serverName);
    }
    else
    {
        return getUpdateCommandRemote(serverName);
    }
}

bool ServerManager::setUpdateCommand(QString serverName, QString updateCommand)
{
    if (ServerManagerMode == LocalMode)
    {
        setUpdateCommandLocal(serverName, updateCommand);
        return true;
    }
    else
    {
        return setUpdateCommandRemote(serverName, updateCommand);
    }
}

QString ServerManager::getAttachCommand(QString serverName)
{
    if (ServerManagerMode == LocalMode)
    {
        return getAttachCommandLocal(serverName);
    }
    else
    {
        return getAttachCommandRemote(serverName);
    }
}

bool ServerManager::setAttachCommand(QString serverName, QString attachCommand)
{
    if (ServerManagerMode == LocalMode)
    {
        setAttachCommandLocal(serverName, attachCommand);
        return true;
    }
    else
    {
        return setAttachCommandRemote(serverName, attachCommand);
    }
}

bool ServerManager::startServer(QString serverName)
{
    if (ServerManagerMode == LocalMode)
    {
        return startServerLocal(serverName);
    }
    else
    {
        return startServerRemote(serverName);
    }
}

bool ServerManager::stopServer(QString serverName)
{
    if (ServerManagerMode == LocalMode)
    {
        return stopServerLocal(serverName);
    }
    else
    {
        return stopServerRemote(serverName);
    }
}

bool ServerManager::configServer(QString serverName)
{
    if (ServerManagerMode == LocalMode)
    {
        return configServerLocal(serverName);
    }
    else
    {
        return configServerRemote(serverName);
    }
}

bool ServerManager::updateServer(QString serverName)
{
    if (ServerManagerMode == LocalMode)
    {
        return updateServerLocal(serverName);
    }
    else
    {
        return updateServerRemote(serverName);
    }
}

bool ServerManager::attachServer(QString serverName)
{
    if (ServerManagerMode == LocalMode)
    {
        return attachServerLocal(serverName);
    }
    else
    {
        return attachServerRemote(serverName);
    }
}

bool ServerManager::setIconPath(QString serverName, QString iconPath)
{
    if (ServerManagerMode == LocalMode)
    {
        setIconPathLocal(serverName, iconPath);
        return true;
    }
    else
    {
        return setIconPathRemote(serverName, iconPath);
    }
}

QString ServerManager::getAdminPasswordHash()
{
    if (ServerManagerMode == LocalMode)
    {
        return getAdminPasswordHashLocal();
    }
    else
    {
        return getAdminPasswordHashRemote();
    }
}

bool ServerManager::setAdminPasswordHash(QString passwordHash)
{
    if (ServerManagerMode == LocalMode)
    {
        setAdminPasswordHashLocal(passwordHash);
        return true;
    }
    else
    {
        return setAdminPasswordRemote(passwordHash);
    }
}

bool ServerManager::setAdminPassword(QString password)
{
    if (ServerManagerMode == LocalMode)
    {
        setAdminPasswordLocal(password);
        return true;
    }
    else
    {
        return setAdminPasswordRemote(password);
    }
}

QString ServerManager::getPasswordHashFromString(QString password)
{
    QCryptographicHash *pwHash = new QCryptographicHash(QCryptographicHash::Md4);
    pwHash->addData(password.toUtf8());
    QString hashString = QString::fromUtf8(pwHash->result().toBase64());
    return hashString;
}

bool ServerManager::isConnected()
{
    if (ServerManagerMode == LocalMode)
    {
        return true;
    }
    else
    {
        if (tcpSocket->state() == QAbstractSocket::ConnectedState)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
}

bool ServerManager::isConnecting()
{
    if (ServerManagerMode == LocalMode)
    {
        return true;
    }
    else
    {
        if (tcpSocket->state() == QAbstractSocket::ConnectedState)
        {
            return true;
        }
        else
        {
            if (tcpSocket->state() == QAbstractSocket::ConnectingState)
            {
                return true;
            }
            else
            {
                return false;
            }
        }
    }
}

bool ServerManager::connectToServer(QString hostname, QString password, int port, bool useSSL)
{
    if (ServerManagerMode == RemoteMode)
    {
#ifdef DISABLE_SSL
        tcpSocket = new QTcpSocket();
        tcpSocket->connectToHost(hostname, port);
        Q_UNUSED(useSSL);
#else
        tcpSocket = new QSslSocket();
        if (useSSL)
        {
            tcpSocket->connectToHostEncrypted(hostname, port);
        }
        else
        {
            tcpSocket->connectToHost(hostname, port);
        }
#endif
        bool wfc;
        wfc = tcpSocket->waitForConnected(30000);
#ifndef DISABLE_SSL
        if (useSSL)
        {
            wfc = tcpSocket->waitForEncrypted(30000);
        }
#endif
        if (wfc)
        {
            QString spw = password;
            tcpSocket->write("SM/1.1 login");
            tcpSocket->write(" --password=");
            spw = spw.replace("&","&amp;");
            spw = spw.replace(" ","&nbsp;");
            tcpSocket->write(spw.toUtf8());
            tcpSocket->write("\n");
            tcpSocket->flush();
            tcpSocket->waitForBytesWritten(5000);
            tcpSocket->waitForReadyRead(5000);
            if (tcpSocket->canReadLine())
            {
                QByteArray readed = tcpSocket->readLine().trimmed();
                QString readstr = QString::fromUtf8(readed);
                QStringList readlist = readstr.split(" ");
                QString description;
                int id = 500;
                if (readlist.length() >= 2)
                {
                    if (readlist.at(0) == "SM/1.1" && readlist.at(1) == "return")
                    {
                        foreach(QString argstr,readlist)
                        {
                            if (argstr.left(5) == "--id=")
                            {
                                id = QVariant(argstr.remove(0,5).replace("&nbsp;"," ").replace("&amp;","&")).toInt();
                            }
                            if (argstr.left(14) == "--description=")
                            {
                                description = QVariant(argstr.remove(0,14).replace("&nbsp;"," ").replace("&amp;","&")).toInt();
                            }
                        }
                        if (id == 200)
                        {
                            sessionHost = hostname;
                            sessionPasswd = password;
                            sessionPort = port;
                            sessionSSL = useSSL;
                            lastReturn = id;
                            return true;
                        }
                        else
                        {
                            lastReturn = id;
                            return false;
                        }
                    }
                    else
                    {
                        lastReturn = id;
                        return false;
                    }
                }
                else
                {
                    lastReturn = 600;
                    return false;
                }
            }
            else
            {
                lastReturn = 700;
                return false;
            }
        }
        else
        {
            lastReturn = 800;
            return false;
        }
    }
    else
    {
        return false;
    }
}

void ServerManager::disconnectFromServer()
{
    if (ServerManagerMode == ServerManager::RemoteMode)
    {
        if (isConnected())
        {
            tcpSocket->write("SM/1.1 disconnect\n");
            tcpSocket->flush();
            tcpSocket->waitForBytesWritten(1000);
            tcpSocket->disconnectFromHost();
        }
        else if (isConnecting())
        {
            tcpSocket->disconnectFromHost();
        }
    }
}

bool ServerManager::autologinEnabled()
{
    configFile->beginGroup("autologin");
    bool autologin = configFile->value("enabled", false).toBool();
    configFile->endGroup();
    return autologin;
}

void ServerManager::setAutologinEnabled(QString hostname, QString password, int port, bool useSSL)
{
    configFile->beginGroup("autologin");
    configFile->setValue("enabled", true);
    configFile->setValue("hostname", hostname);
    configFile->setValue("password", password.toUtf8().toBase64().toHex().toBase64());
    configFile->setValue("port", port);
    configFile->setValue("useSSL", useSSL);
    configFile->endGroup();
}

void ServerManager::setAutologinDisabled()
{
    configFile->beginGroup("autologin");
    configFile->setValue("enabled", false);
    configFile->remove("hostname");
    configFile->remove("password");
    configFile->remove("port");
    configFile->remove("useSSL");
    configFile->endGroup();
}

bool ServerManager::connectToServerWithAutologin()
{
    configFile->beginGroup("autologin");
    QString hostname = configFile->value("hostname","").toString();
    if (hostname.isEmpty())
    {
        configFile->endGroup();
        return false;
    }
    else if (hostname == "SM_LOCAL")
    {
        configFile->endGroup();
        ServerManagerMode = LocalMode;
        return true;
    }
    QByteArray pwArray = configFile->value("password","").toByteArray();
    QString password;
    if (pwArray.isEmpty())
    {
        configFile->endGroup();
        return false;
    }
    password = QString::fromUtf8(QByteArray::fromBase64(QByteArray::fromHex(QByteArray::fromBase64(pwArray))));
    int port = configFile->value("port", 9509).toInt();
    bool useSSL = configFile->value("useSSL", false).toBool();
    configFile->endGroup();
    ServerManagerMode = RemoteMode;
    return connectToServer(hostname, password, port, useSSL);
}

bool ServerManager::isConnectionLocal()
{
    if (ServerManagerMode == RemoteMode)
    {
        if (isConnected())
        {
            if (tcpSocket->peerAddress() == QHostAddress::LocalHost || tcpSocket->peerAddress() == QHostAddress::LocalHostIPv6 || tcpSocket->peerAddress() == tcpSocket->localAddress())
            {
                return true;
            }
            return false;
        }
        return false;
    }
    else
    {
        return true;
    }
}

QString ServerManager::getCurrentSessionHostname()
{
    return sessionHost;
}

QString ServerManager::getCurrentSessionPassword()
{
    return sessionPasswd;
}

int ServerManager::getCurrentSessionPort()
{
    return sessionPort;
}

bool ServerManager::getCurrentSessionSSL()
{
    return sessionSSL;
}

// Server Manager LocalMode

QStringList ServerManager::getServerListLocal()
{
    return configFile->value("serverList",QStringList()).toStringList();
}

QString ServerManager::getIconPathLocal(QString serverName)
{
    configFile->beginGroup("server " + serverName);
    QString iconPath = configFile->value("icon", ProductImg).toString();
    configFile->endGroup();
    return iconPath;
}

QString ServerManager::getIconBytesLocal(QString serverName, int squareSize)
{
    QString iconPath = getIconPathLocal(serverName);
    QPixmap iconPixmap = QPixmap(iconPath);
    if (!iconPixmap.isNull())
    {
        PixmapEdit sPixmapEdit;
        sPixmapEdit.setPixmap(iconPixmap);
        bool success = sPixmapEdit.centerPixmapAtSquare(squareSize, false);
        if (success)
        {
            iconPixmap = sPixmapEdit.getPixmap();
            QByteArray iconArray;
            QBuffer iconBuffer(&iconArray);
            iconBuffer.open(QIODevice::WriteOnly);
            iconPixmap.save(&iconBuffer,"PNG");
            iconBuffer.close();
            return QString::fromUtf8(iconArray.toBase64());
        }
        else
        {
            return "";
        }
    }
    else
    {
        return "";
    }
}

bool ServerManager::addServerLocal(QString serverName)
{
    QStringList serverList = configFile->value("serverList",QStringList()).toStringList();
    if (!serverList.contains(serverName))
    {
        serverList.append(serverName);
        configFile->setValue("serverList",serverList);
        return true;
    }
    return false;
}

void ServerManager::deleteServerLocal(QString serverName)
{
    QStringList serverList = configFile->value("serverList",QStringList()).toStringList();
    serverList.removeAll(serverName);
    configFile->setValue("serverList",serverList);
    configFile->beginGroup("server " + serverName);
    configFile->remove("start");
    configFile->remove("stop");
    configFile->remove("config");
    configFile->remove("update");
    configFile->remove("icon");
    configFile->endGroup();
}

QString ServerManager::getStartCommandLocal(QString serverName)
{
    configFile->beginGroup("server " + serverName);
    QString serverStart = configFile->value("start").toString();
    configFile->endGroup();
    return serverStart;
}

void ServerManager::setStartCommandLocal(QString serverName, QString startCommand)
{
    configFile->beginGroup("server " + serverName);
    configFile->setValue("start", startCommand);
    configFile->endGroup();
}

QString ServerManager::getStopCommandLocal(QString serverName)
{
    configFile->beginGroup("server " + serverName);
    QString serverStop = configFile->value("stop").toString();
    configFile->endGroup();
    return serverStop;
}

void ServerManager::setStopCommandLocal(QString serverName, QString stopCommand)
{
    configFile->beginGroup("server " + serverName);
    configFile->setValue("stop", stopCommand);
    configFile->endGroup();
}

QString ServerManager::getConfigCommandLocal(QString serverName)
{
    configFile->beginGroup("server " + serverName);
    QString serverConfig = configFile->value("config").toString();
    configFile->endGroup();
    return serverConfig;
}

void ServerManager::setConfigCommandLocal(QString serverName, QString configCommand)
{
    configFile->beginGroup("server " + serverName);
    configFile->setValue("config", configCommand);
    configFile->endGroup();
}

QString ServerManager::getUpdateCommandLocal(QString serverName)
{
    configFile->beginGroup("server " + serverName);
    QString serverUpdate = configFile->value("update").toString();
    configFile->endGroup();
    return serverUpdate;
}

void ServerManager::setUpdateCommandLocal(QString serverName, QString updateCommand)
{
    configFile->beginGroup("server " + serverName);
    configFile->setValue("update", updateCommand);
    configFile->endGroup();
}

QString ServerManager::getAttachCommandLocal(QString serverName)
{
    configFile->beginGroup("server " + serverName);
    QString serverAttach = configFile->value("attach").toString();
    configFile->endGroup();
    return serverAttach;
}

void ServerManager::setAttachCommandLocal(QString serverName, QString attachCommand)
{
    configFile->beginGroup("server " + serverName);
    configFile->setValue("attach", attachCommand);
    configFile->endGroup();
}

bool ServerManager::startServerLocal(QString serverName)
{
    configFile->beginGroup("server " + serverName);
    QString serverStart = configFile->value("start","").toString();
    configFile->endGroup();
    if (serverStart != "")
    {
        QProcess serverProcess;
        serverProcess.startDetached(serverStart);
        return true;
    }
    return false;
}

bool ServerManager::stopServerLocal(QString serverName)
{
    configFile->beginGroup("server " + serverName);
    QString serverStop = configFile->value("stop","").toString();
    configFile->endGroup();
    if (serverStop != "")
    {
        QProcess serverProcess;
        serverProcess.startDetached(serverStop);
        return true;
    }
    return false;
}

bool ServerManager::configServerLocal(QString serverName)
{
    configFile->beginGroup("server " + serverName);
    QString serverConfig = configFile->value("config","").toString();
    configFile->endGroup();
    if (serverConfig != "")
    {
        QProcess serverProcess;
        serverProcess.startDetached(serverConfig);
        return true;
    }
    return false;
}

bool ServerManager::updateServerLocal(QString serverName)
{
    configFile->beginGroup("server " + serverName);
    QString serverUpdate = configFile->value("update","").toString();
    configFile->endGroup();
    if (serverUpdate != "")
    {
        QProcess serverProcess;
        serverProcess.startDetached(serverUpdate);
        return true;
    }
    return false;
}

bool ServerManager::attachServerLocal(QString serverName)
{
    configFile->beginGroup("server " + serverName);
    QString serverAttach = configFile->value("attach","").toString();
    configFile->endGroup();
    if (serverAttach != "")
    {
        QProcess serverProcess;
        serverProcess.startDetached(serverAttach);
        return true;
    }
    return false;
}

void ServerManager::setIconPathLocal(QString serverName, QString iconPath)
{
    configFile->beginGroup("server " + serverName);
    configFile->setValue("icon", iconPath);
    configFile->endGroup();
}

QString ServerManager::getAdminPasswordHashLocal()
{
    configFile->beginGroup("auth");
    QString passwordHash = configFile->value("password","").toString();
    configFile->endGroup();
    return passwordHash;
}

void ServerManager::setAdminPasswordHashLocal(QString passwordHash)
{
    configFile->beginGroup("auth");
    configFile->setValue("password", passwordHash);
    configFile->endGroup();
}

void ServerManager::setAdminPasswordLocal(QString password)
{
    configFile->beginGroup("auth");
    configFile->setValue("password", getPasswordHashFromString(password));
    configFile->endGroup();
}

// Server Manager RemoteMode

QStringList ServerManager::getServerListRemote()
{
    if (isConnected())
    {
        tcpSocket->write("SM/1.1 request --rq=getserverlist\n");
        tcpSocket->flush();
        QStringList args = getArgsFromReturnRemote();
        if (args.at(0) != "ERROR_NORETURN")
        {
            QString serverString = args.at(2);
            if (serverString != "")
            {
                return serverString.split("&sms;");
            }
            else
            {
                return QStringList("ERROR_NORETURN");
            }
        }
        else
        {
            return QStringList("ERROR_NORETURN");
        }
    }
    return QStringList("ERROR_DISCONNECTED");
}

QString ServerManager::getIconPathRemote(QString serverName)
{
    if (isConnected())
    {
        QString arg1 = serverName;
        arg1 = serverName.replace("&","&amp;");
        arg1 = serverName.replace(" ","&nbsp;");
        tcpSocket->write("SM/1.1 request --rq=geticonpath --arg1=" + arg1.toUtf8() + "\n");
        tcpSocket->flush();
        tcpSocket->waitForBytesWritten(5000);
        QStringList args = getArgsFromReturnRemote();
        if (args.at(0) != "ERROR_NORETURN")
        {
            return args.at(2);
        }
        else
        {
            return "ERROR_NORETURN";
        }
    }
    return "ERROR_DISCONNECTED";
}

QString ServerManager::getIconBytesRemote(QString serverName, int squareSize)
{
    if (isConnected())
    {
        QString arg1 = serverName;
        arg1 = serverName.replace("&","&amp;");
        arg1 = serverName.replace(" ","&nbsp;");
        QString arg2 = QString::number(squareSize);
        tcpSocket->write("SM/1.1 request --rq=geticonbytes --arg1=" + arg1.toUtf8() + " --arg2=" + arg2.toUtf8() + "\n");
        tcpSocket->flush();
        tcpSocket->waitForBytesWritten(5000);
        QStringList args = getArgsFromReturnRemote();
        if (args.at(0) != "ERROR_NORETURN")
        {
            return args.at(2);
        }
        else
        {
            return "ERROR_NORETURN";
        }
    }
    return "ERROR_DISCONNECTED";
}

bool ServerManager::addServerRemote(QString serverName)
{
    if (isConnected())
    {
        QString arg1 = serverName;
        arg1 = arg1.replace("&","&amp;");
        arg1 = arg1.replace(" ","&nbsp;");
        tcpSocket->write("SM/1.1 action --at=addserver --arg1=" + arg1.toUtf8() + "\n");
        tcpSocket->flush();
        tcpSocket->waitForBytesWritten(5000);
        QStringList args = getArgsFromReturnRemote();
        if (args.at(0) != "ERROR_NORETURN")
        {
            if (args.at(1) == "200")
            {
                return true;
            }
        }
    }
    return false;
}

bool ServerManager::deleteServerRemote(QString serverName)
{
    if (isConnected())
    {
        QString arg1 = serverName;
        arg1 = arg1.replace("&","&amp;");
        arg1 = arg1.replace(" ","&nbsp;");
        tcpSocket->write("SM/1.1 action --at=delserver --arg1=" + arg1.toUtf8() + "\n");
        tcpSocket->flush();
        tcpSocket->waitForBytesWritten(5000);
        QStringList args = getArgsFromReturnRemote();
        if (args.at(0) != "ERROR_NORETURN")
        {
            if (args.at(1) == "200")
            {
                return true;
            }
        }
    }
    return false;
}

QString ServerManager::getStartCommandRemote(QString serverName)
{
    if (isConnected())
    {
        QString arg1 = serverName;
        arg1 = arg1.replace("&","&amp;");
        arg1 = arg1.replace(" ","&nbsp;");
        tcpSocket->write("SM/1.1 request --rq=getstartcmd --arg1=" + arg1.toUtf8() + "\n");
        tcpSocket->flush();
        tcpSocket->waitForBytesWritten(5000);
        QStringList args = getArgsFromReturnRemote();
        if (args.at(0) != "ERROR_NORETURN")
        {
            return args.at(2);
        }
        else
        {
            return "ERROR_NORETURN";
        }
    }
    return "ERROR_DISCONNECTED";
}

bool ServerManager::setStartCommandRemote(QString serverName, QString startCommand)
{
    if (isConnected())
    {
        QString arg1 = serverName;
        arg1 = arg1.replace("&","&amp;");
        arg1 = arg1.replace(" ","&nbsp;");
        QString arg2 = startCommand;
        arg2 = arg2.replace("&","&amp;");
        arg2 = arg2.replace(" ","&nbsp;");
        tcpSocket->write("SM/1.1 action --at=setstartcmd --arg1=" + arg1.toUtf8() + " --arg2=" + arg2.toUtf8() + "\n");
        tcpSocket->flush();
        tcpSocket->waitForBytesWritten(5000);
        QStringList args = getArgsFromReturnRemote();
        if (args.at(0) != "ERROR_NORETURN")
        {
            if (args.at(1) == "200")
            {
                return true;
            }
        }
    }
    return false;
}

QString ServerManager::getStopCommandRemote(QString serverName)
{
    if (isConnected())
    {
        QString arg1 = serverName;
        arg1 = arg1.replace("&","&amp;");
        arg1 = arg1.replace(" ","&nbsp;");
        tcpSocket->write("SM/1.1 request --rq=getstopcmd --arg1=" + arg1.toUtf8() + "\n");
        tcpSocket->flush();
        tcpSocket->waitForBytesWritten(5000);
        QStringList args = getArgsFromReturnRemote();
        if (args.at(0) != "ERROR_NORETURN")
        {
            return args.at(2);
        }
        else
        {
            return "ERROR_NORETURN";
        }
    }
    return "ERROR_DISCONNECTED";
}

bool ServerManager::setStopCommandRemote(QString serverName, QString stopCommand)
{
    if (isConnected())
    {
        QString arg1 = serverName;
        arg1 = arg1.replace("&","&amp;");
        arg1 = arg1.replace(" ","&nbsp;");
        QString arg2 = stopCommand;
        arg2 = arg2.replace("&","&amp;");
        arg2 = arg2.replace(" ","&nbsp;");
        tcpSocket->write("SM/1.1 action --at=setstopcmd --arg1=" + arg1.toUtf8() + " --arg2=" + arg2.toUtf8() + "\n");
        tcpSocket->flush();
        tcpSocket->waitForBytesWritten(5000);
        QStringList args = getArgsFromReturnRemote();
        if (args.at(0) != "ERROR_NORETURN")
        {
            if (args.at(1) == "200")
            {
                return true;
            }
        }
    }
    return false;
}

QString ServerManager::getConfigCommandRemote(QString serverName)
{
    if (isConnected())
    {
        QString arg1 = serverName;
        arg1 = arg1.replace("&","&amp;");
        arg1 = arg1.replace(" ","&nbsp;");
        tcpSocket->write("SM/1.1 request --rq=getconfigcmd --arg1=" + arg1.toUtf8() + "\n");
        tcpSocket->flush();
        tcpSocket->waitForBytesWritten(5000);
        QStringList args = getArgsFromReturnRemote();
        if (args.at(0) != "ERROR_NORETURN")
        {
            return args.at(2);
        }
        else
        {
            return "ERROR_NORETURN";
        }
    }
    return "ERROR_DISCONNECTED";
}

bool ServerManager::setConfigCommandRemote(QString serverName, QString configCommand)
{
    if (isConnected())
    {
        QString arg1 = serverName;
        arg1 = arg1.replace("&","&amp;");
        arg1 = arg1.replace(" ","&nbsp;");
        QString arg2 = configCommand;
        arg2 = arg2.replace("&","&amp;");
        arg2 = arg2.replace(" ","&nbsp;");
        tcpSocket->write("SM/1.1 action --at=setconfigcmd --arg1=" + arg1.toUtf8() + " --arg2=" + arg2.toUtf8() + "\n");
        tcpSocket->flush();
        tcpSocket->waitForBytesWritten(5000);
        QStringList args = getArgsFromReturnRemote();
        if (args.at(0) != "ERROR_NORETURN")
        {
            if (args.at(1) == "200")
            {
                return true;
            }
        }
    }
    return false;
}

QString ServerManager::getUpdateCommandRemote(QString serverName)
{
    if (isConnected())
    {
        QString arg1 = serverName;
        arg1 = arg1.replace("&","&amp;");
        arg1 = arg1.replace(" ","&nbsp;");
        tcpSocket->write("SM/1.1 request --rq=getupdatecmd --arg1=" + arg1.toUtf8() + "\n");
        tcpSocket->flush();
        tcpSocket->waitForBytesWritten(5000);
        QStringList args = getArgsFromReturnRemote();
        if (args.at(0) != "ERROR_NORETURN")
        {
            return args.at(2);
        }
        else
        {
            return "ERROR_NORETURN";
        }
    }
    return "ERROR_DISCONNECTED";
}

bool ServerManager::setUpdateCommandRemote(QString serverName, QString updateCommand)
{
    if (isConnected())
    {
        QString arg1 = serverName;
        arg1 = arg1.replace("&","&amp;");
        arg1 = arg1.replace(" ","&nbsp;");
        QString arg2 = updateCommand;
        arg2 = arg2.replace("&","&amp;");
        arg2 = arg2.replace(" ","&nbsp;");
        tcpSocket->write("SM/1.1 action --at=setupdatecmd --arg1=" + arg1.toUtf8() + " --arg2=" + arg2.toUtf8() + "\n");
        tcpSocket->flush();
        tcpSocket->waitForBytesWritten(5000);
        QStringList args = getArgsFromReturnRemote();
        if (args.at(0) != "ERROR_NORETURN")
        {
            if (args.at(1) == "200")
            {
                return true;
            }
        }
    }
    return false;
}

QString ServerManager::getAttachCommandRemote(QString serverName)
{
    if (isConnected())
    {
        QString arg1 = serverName;
        arg1 = arg1.replace("&","&amp;");
        arg1 = arg1.replace(" ","&nbsp;");
        tcpSocket->write("SM/1.1 request --rq=getattachcmd --arg1=" + arg1.toUtf8() + "\n");
        tcpSocket->flush();
        tcpSocket->waitForBytesWritten(5000);
        QStringList args = getArgsFromReturnRemote();
        if (args.at(0) != "ERROR_NORETURN")
        {
            return args.at(2);
        }
        else
        {
            return "ERROR_NORETURN";
        }
    }
    return "ERROR_DISCONNECTED";
}

bool ServerManager::setAttachCommandRemote(QString serverName, QString attachCommand)
{
    if (isConnected())
    {
        QString arg1 = serverName;
        arg1 = arg1.replace("&","&amp;");
        arg1 = arg1.replace(" ","&nbsp;");
        QString arg2 = attachCommand;
        arg2 = arg2.replace("&","&amp;");
        arg2 = arg2.replace(" ","&nbsp;");
        tcpSocket->write("SM/1.1 action --at=setattachcmd --arg1=" + arg1.toUtf8() + " --arg2=" + arg2.toUtf8() + "\n");
        tcpSocket->flush();
        tcpSocket->waitForBytesWritten(5000);
        QStringList args = getArgsFromReturnRemote();
        if (args.at(0) != "ERROR_NORETURN")
        {
            if (args.at(1) == "200")
            {
                return true;
            }
        }
    }
    return false;
}

bool ServerManager::startServerRemote(QString serverName)
{
    if (isConnected())
    {
        QString arg1 = serverName;
        arg1 = arg1.replace("&","&amp;");
        arg1 = arg1.replace(" ","&nbsp;");
        tcpSocket->write("SM/1.1 action --at=startserver --arg1=" + arg1.toUtf8() + "\n");
        tcpSocket->flush();
        tcpSocket->waitForBytesWritten(5000);
        QStringList args = getArgsFromReturnRemote();
        if (args.at(0) != "ERROR_NORETURN")
        {
            if (args.at(1) == "200")
            {
                return true;
            }
        }
    }
    return false;
}

bool ServerManager::stopServerRemote(QString serverName)
{
    if (isConnected())
    {
        QString arg1 = serverName;
        arg1 = arg1.replace("&","&amp;");
        arg1 = arg1.replace(" ","&nbsp;");
        tcpSocket->write("SM/1.1 action --at=stopserver --arg1=" + arg1.toUtf8() + "\n");
        tcpSocket->flush();
        tcpSocket->waitForBytesWritten(5000);
        QStringList args = getArgsFromReturnRemote();
        if (args.at(0) != "ERROR_NORETURN")
        {
            if (args.at(1) == "200")
            {
                return true;
            }
        }
    }
    return false;
}

bool ServerManager::configServerRemote(QString serverName)
{
    if (isConnected())
    {
        QString arg1 = serverName;
        arg1 = arg1.replace("&","&amp;");
        arg1 = arg1.replace(" ","&nbsp;");
        tcpSocket->write("SM/1.1 action --at=configserver --arg1=" + arg1.toUtf8() + "\n");
        tcpSocket->flush();
        tcpSocket->waitForBytesWritten(5000);
        QStringList args = getArgsFromReturnRemote();
        if (args.at(0) != "ERROR_NORETURN")
        {
            if (args.at(1) == "200")
            {
                return true;
            }
        }
    }
    return false;
}

bool ServerManager::updateServerRemote(QString serverName)
{
    if (isConnected())
    {
        QString arg1 = serverName;
        arg1 = arg1.replace("&","&amp;");
        arg1 = arg1.replace(" ","&nbsp;");
        tcpSocket->write("SM/1.1 action --at=updateserver --arg1=" + arg1.toUtf8() + "\n");
        tcpSocket->flush();
        tcpSocket->waitForBytesWritten(5000);
        QStringList args = getArgsFromReturnRemote();
        if (args.at(0) != "ERROR_NORETURN")
        {
            if (args.at(1) == "200")
            {
                return true;
            }
        }
    }
    return false;
}

bool ServerManager::attachServerRemote(QString serverName)
{
    if (isConnected())
    {
        QString arg1 = serverName;
        arg1 = arg1.replace("&","&amp;");
        arg1 = arg1.replace(" ","&nbsp;");
        tcpSocket->write("SM/1.1 action --at=attachserver --arg1=" + arg1.toUtf8() + "\n");
        tcpSocket->flush();
        tcpSocket->waitForBytesWritten(5000);
        QStringList args = getArgsFromReturnRemote();
        if (args.at(0) != "ERROR_NORETURN")
        {
            if (args.at(1) == "200")
            {
                return true;
            }
        }
    }
    return false;
}

bool ServerManager::setIconPathRemote(QString serverName, QString iconPath)
{
    if (isConnected())
    {
        QString arg1 = serverName;
        arg1 = arg1.replace("&","&amp;");
        arg1 = arg1.replace(" ","&nbsp;");
        QString arg2 = iconPath;
        arg2 = arg2.replace("&","&amp;");
        arg2 = arg2.replace(" ","&nbsp;");
        tcpSocket->write("SM/1.1 action --at=seticonpath --arg1=" + arg1.toUtf8() + " --arg2=" + arg2.toUtf8() + "\n");
        tcpSocket->flush();
        tcpSocket->waitForBytesWritten(5000);
        QStringList args = getArgsFromReturnRemote();
        if (args.at(0) != "ERROR_NORETURN")
        {
            if (args.at(1) == "200")
            {
                return true;
            }
        }
    }
    return false;
}

QString ServerManager::getAdminPasswordHashRemote()
{
    if (isConnected())
    {
        tcpSocket->write("SM/1.1 request --rq=getadminpwhash\n");
        tcpSocket->flush();
        tcpSocket->waitForBytesWritten(5000);
        QStringList args = getArgsFromReturnRemote();
        if (args.at(0) != "ERROR_NORETURN")
        {
            return args.at(2);
        }
        else
        {
            return "ERROR_NORETURN";
        }
    }
    return "ERROR_DISCONNECTED";
}

bool ServerManager::setAdminPasswordHashRemote(QString passwordHash)
{
    if (isConnected())
    {
        QString arg1 = passwordHash;
        arg1 = arg1.replace("&","&amp;");
        arg1 = arg1.replace(" ","&nbsp;");
        tcpSocket->write("SM/1.1 action --at=setadminpwhash --arg1=" + arg1.toUtf8() + "\n");
        tcpSocket->flush();
        tcpSocket->waitForBytesWritten(5000);
        QStringList args = getArgsFromReturnRemote();
        if (args.at(0) != "ERROR_NORETURN")
        {
            if (args.at(1) == "200")
            {
                return true;
            }
        }
    }
    return false;
}

bool ServerManager::setAdminPasswordRemote(QString password)
{
    if (isConnected())
    {
        QString arg1 = password;
        arg1 = arg1.replace("&","&amp;");
        arg1 = arg1.replace(" ","&nbsp;");
        tcpSocket->write("SM/1.1 action --at=setadminpw --arg1=" + arg1.toUtf8() + "\n");
        tcpSocket->flush();
        tcpSocket->waitForBytesWritten(5000);
        QStringList args = getArgsFromReturnRemote();
        if (args.at(0) != "ERROR_NORETURN")
        {
            if (args.at(1) == "200")
            {
                return true;
            }
        }
    }
    return false;
}

QStringList ServerManager::getArgsFromReturnRemote()
{
    QStringList retlist;
    int readTrys = 0;
    returnToReadyRead:
    tcpSocket->waitForReadyRead(1000);
    if (tcpSocket->canReadLine())
    {
        QByteArray readed = tcpSocket->readLine().trimmed();
        QString readstr = QString::fromUtf8(readed);
        QStringList readlist = readstr.split(" ");
        QString atrq;
        QString arg1;
        QString arg2;
        QString reid;
        foreach(QString argstr, readlist)
        {
            if (argstr.left(5) == "--at=")
            {
                atrq = argstr.remove(0,5).replace("&nbsp;"," ").replace("&amp;","&");
            }
            if (argstr.left(5) == "--rq=")
            {
                atrq = argstr.remove(0,5).replace("&nbsp;"," ").replace("&amp;","&");
            }
            if (argstr.left(5) == "--id=")
            {
                reid = argstr.remove(0,5).replace("&nbsp;"," ").replace("&amp;","&");
            }
            if (argstr.left(7) == "--arg1=")
            {
                arg1 = argstr.remove(0,7).replace("&nbsp;"," ").replace("&amp;","&");
            }
            if (argstr.left(7) == "--arg2=")
            {
                arg2 = argstr.remove(0,7).replace("&nbsp;"," ").replace("&amp;","&");
            }
        }
        lastReturn = reid.toInt();
        retlist.append(atrq);
        retlist.append(reid);
        retlist.append(arg1);
        retlist.append(arg2);
        return retlist;
    }
    readTrys++;
    if (readTrys <= 5) goto returnToReadyRead;
    retlist.append("ERROR_NORETURN");
    lastReturn = 500;
    return retlist;
}

int ServerManager::getLastReturnValue()
{
    if (ServerManagerMode == RemoteMode)
    {
        return lastReturn;
    }
    else
    {
        return 200;
    }
}
