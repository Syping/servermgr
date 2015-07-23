#include "IconThread.h"
#include <QByteArray>
#include <QPixmap>
#include <QBuffer>
#include <QIcon>
#include <QFile>

IconThread::IconThread(ServerManager *smgr, QStringList serverList, QObject *parent) :
    QThread(parent), smgr(smgr), serverList(serverList)
{

}

void IconThread::run()
{
    foreach(QString serverName, serverList)
    {
        QString iconPath = smgr->getIconPath(serverName);
        if (iconPath.left(6) != "ERROR_")
        {
            if (smgr->ServerManagerMode == ServerManager::RemoteMode)
            {
                // Working on that
            }
            else
            {
                if (QFile::exists(iconPath))
                {
                    QIcon serverIcon = QIcon(iconPath);
                    if (!serverIcon.isNull())
                    {
                        QByteArray iconBytes;
                        QBuffer iconBuffer(&iconBytes);
                        iconBuffer.open(QIODevice::WriteOnly);
                        QDataStream iconOut(&iconBuffer);
                        iconOut << serverIcon;
                        iconBuffer.close();

                        emit setServerIcon(serverName, iconBytes);
                    }
                }
            }
        }
    }
}
