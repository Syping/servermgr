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

#ifndef FRMSERVERMANAGER_H
#define FRMSERVERMANAGER_H

#include <QListWidget>
#include <QMainWindow>
#include <QCloseEvent>
#include <QSettings>
#include "ServerManager.h"
#include "ConnectThread.h"
#include "IconThread.h"
#include "KPTLabel.h"

namespace Ui {
    class frmServerManager;
}

class frmServerManager : public QMainWindow
{
    Q_OBJECT

public:
    explicit frmServerManager(QString languagePath = "", bool designedMode = true, QWidget *parent = 0);
    void setAdminMode(bool admin);
    ~frmServerManager();

public slots:
    void setServerIcon(QString serverName, QByteArray iconBytes);

private slots:
    void on_connectionIssued(bool isSuccess);
    void on_cmdNewServer_clicked();
    void on_cmdDeleteServer_clicked();
    void on_cmdCStart_clicked();
    void on_cmdCStop_clicked();
    void on_cmdCConfig_clicked();
    void on_cmdCUpdate_clicked();
    void on_cmdStart_clicked();
    void on_cmdStop_clicked();
    void on_cmdConfig_clicked();
    void on_cmdUpdate_clicked();
    void on_cmdAdmin_clicked();
    void on_cmdDAdmin_clicked();
    void on_cmdAttach_clicked();
    void on_cmdCAttach_clicked();
    void on_cmdCIcon_clicked();
    void connectToServer();
    void on_cmdDisconnect_clicked();
    void on_cmdConfigLocal_clicked();
    void on_cmdDesignedLogin_clicked();
    void on_cmdAndroidLogin_clicked();
    void on_cmdStartAndroid_clicked();
    void on_cmdStopAndroid_clicked();
    void on_cmdDisconnectAndroid_clicked();
    void on_cmdUpdateAndroid_clicked();

private:
    QString languagePath;
    bool designedMode;
    Ui::frmServerManager *ui;
    QListWidget *activeLW;
    ConnectThread *conWT;
    QString SMStyleSheet;
    ServerManager *smgr;
    IconThread *iconWT;
    KPTLabel *labStats;
    QIcon standardIcon;
    int pagePleaseWait;
    int pageInterface;
    int pageLogin;
    int izSquare;
    bool mouseOverLogin;
    bool iconWTDefined;
    bool noAdmin;

protected:
    void closeEvent(QCloseEvent *ev);
};

#endif // FRMSERVERMANAGER_H
