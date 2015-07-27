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

#ifndef ICONTHREAD_H
#define ICONTHREAD_H

#include "ServerManager.h"
#include <QStringList>
#include <QObject>
#include <QThread>

class IconThread : public QThread
{
    Q_OBJECT

public:
    explicit IconThread(ServerManager *smgr, QStringList serverList, int squareSize = 24, QObject *parent = 0);

private:
    ServerManager *smgr;
    QStringList serverList;
    int squareSize;

signals:
    void setServerIcon(QString serverName, QByteArray iconBytes);

protected:
    void run();

};

#endif // ICONTHREAD_H
