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

#include "IconThread.h"
#include "PixmapEdit.h"
#include <QByteArray>
#include <QPainter>
#include <QPixmap>
#include <QBuffer>
#include <QIcon>
#include <QFile>

IconThread::IconThread(ServerManager *smgr, QStringList serverList, int squareSize, QObject *parent) :
    QThread(parent), smgr(smgr), serverList(serverList), squareSize(squareSize)
{

}

void IconThread::run()
{
    if (smgr->ServerManagerMode == ServerManager::RemoteMode)
    {
        ServerManager *ismgr = new ServerManager();
        ismgr->ServerManagerMode = ServerManager::RemoteMode;
        if (ismgr->connectToServer(smgr->getCurrentSessionHostname(),smgr->getCurrentSessionPassword(),smgr->getCurrentSessionPort(),smgr->getCurrentSessionSSL()))
        {
            ismgr->disconnectFromServer();
            ismgr->deleteLater();
        }
        else
        {
            ismgr->deleteLater();
        }
    }
    else
    {
        foreach(QString serverName, serverList)
        {
            QString iconPath = smgr->getIconPath(serverName);
            if (iconPath.left(6) != "ERROR_" && QFile::exists(iconPath))
            {
                QPixmap serverPixmap = QPixmap(iconPath);
                if (!serverPixmap.isNull())
                {
                    if (serverPixmap.size() != QSize(squareSize, squareSize))
                    {
                        PixmapEdit sPixmapEdit;
                        sPixmapEdit.setPixmap(serverPixmap);
                        sPixmapEdit.centerPixmapAtSquare(squareSize, true);
                        serverPixmap = sPixmapEdit.getPixmap();
                    }
                    QByteArray iconBytes;
                    QBuffer iconBuffer(&iconBytes);
                    iconBuffer.open(QIODevice::WriteOnly);
                    QDataStream iconOut(&iconBuffer);
                    iconOut << serverPixmap;
                    iconBuffer.close();
                    emit setServerIcon(serverName, iconBytes);
                }
            }
        }
    }
}
