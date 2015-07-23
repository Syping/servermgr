#include "IconThread.h"
#include <QByteArray>
#include <QPainter>
#include <QPixmap>
#include <QBuffer>
#include <QIcon>
#include <QFile>
#include <cmath>

IconThread::IconThread(ServerManager *smgr, QStringList serverList, QObject *parent) :
    QThread(parent), smgr(smgr), serverList(serverList)
{

}

void IconThread::run()
{
    foreach(QString serverName, serverList)
    {
        if (smgr->ServerManagerMode == ServerManager::RemoteMode)
        {
            // Working on that
        }
        else
        {
            QString iconPath = smgr->getIconPath(serverName);
            if (iconPath.left(6) != "ERROR_" && QFile::exists(iconPath))
            {
                QPixmap serverPixmap = QPixmap(iconPath);
                if (!serverPixmap.isNull())
                {
                    if (serverPixmap.size() != QSize(24,24))
                    {
                        if (serverPixmap.size().height() < serverPixmap.size().width())
                        {
                            QPixmap scaledWorkPixmap = serverPixmap.scaledToWidth(24,Qt::SmoothTransformation);
                            if (scaledWorkPixmap.size() != QSize(24,24))
                            {
                                int _mx2 = scaledWorkPixmap.size().height() - 24;
                                int _px2 = abs(_mx2);
                                double _pxd = _px2 / 2;
                                int _px = floor(_pxd);

                                QPixmap *finishWorkPixmap = new QPixmap(24,24);
                                finishWorkPixmap->fill(Qt::transparent);
                                QPainter drawPainter(finishWorkPixmap);
                                drawPainter.drawPixmap(0, _px, 24, scaledWorkPixmap.size().height(), scaledWorkPixmap);
                                serverPixmap = QPixmap(*(finishWorkPixmap));
                            }
                            else
                            {
                                serverPixmap = scaledWorkPixmap;
                            }
                        }
                        else
                        {
                            QPixmap scaledWorkPixmap = serverPixmap.scaledToHeight(24,Qt::SmoothTransformation);
                            if (scaledWorkPixmap.size() != QSize(24,24))
                            {
                                int _mx2 = scaledWorkPixmap.size().width() - 24;
                                int _px2 = abs(_mx2);
                                double _pxd = _px2 / 2;
                                int _px = floor(_pxd);

                                QPixmap *finishWorkPixmap = new QPixmap(24,24);
                                finishWorkPixmap->fill(Qt::transparent);
                                QPainter drawPainter(finishWorkPixmap);
                                drawPainter.drawPixmap(_px, 0, scaledWorkPixmap.size().width(), 24, scaledWorkPixmap);
                                serverPixmap = QPixmap(*(finishWorkPixmap));
                            }
                            else
                            {
                                serverPixmap = scaledWorkPixmap;
                            }
                        }
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
