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

#include "ui_frmServerManager.h"
#include "frmServerManager.h"
#include "ConnectThread.h"
#include "SMStyleTweaks.h"
#include "IconThread.h"
#include "frmConfig.h"
#include "frmIcon.h"
#include "config.h"
#include <QCryptographicHash>
#include <QStyleFactory>
#include <QCommonStyle>
#include <QInputDialog>
#include <QMessageBox>
#include <QByteArray>
#include <QSettings>
#include <QProcess>
#include <QBuffer>
#include <QTimer>
#include <QStyle>
#include <QDebug>
#include <QFile>
#include <QFont>
#include <QIcon>

frmServerManager::frmServerManager(QString languagePath, bool designedMode, QWidget *parent) :
    QMainWindow(parent), languagePath(languagePath), designedMode(designedMode), ui(new Ui::frmServerManager)
{
    ui->setupUi(this);
    iconWTDefined = false;
    smgr = new ServerManager(this);
    standardIcon = QIcon(ProductImg);
    QIcon windowIcon = QIcon(ProductIcon);
    windowIcon.addPixmap(QPixmap(ProductImg));
    windowIcon.addPixmap(QPixmap(ProductPixmap));
    this->setWindowTitle(ProductName);
    this->setWindowIcon(windowIcon);

    // Change Page IDs
    pageLogin = 0;
    pageInterface = 1;
    pagePleaseWait = 2;

    // Change Text and Version things
    ui->labSMVersion->setText(ui->labSMVersion->text().arg(ProductVersion));

    // Change Visibles
    ui->swSM->setCurrentIndex(pageLogin);
    ui->labTop->setVisible(false);
    ui->statusBar->setVisible(false);
    ui->labDesignedLoginDes->setVisible(false);
    setAdminMode(false);

    // Add widgets to status bar
    labStats = new KPTLabel();
    ui->statusBar->addWidget(labStats,1);
    ui->statusBar->layout()->setContentsMargins(2,0,6,0);
    labStats->setText(tr("Welcome to Syping's %1").arg(ProductName));

    // SM Style designedMode
    SMStyleSheet = ui->pServerManager->styleSheet();
    if (!designedMode)
    {
        ui->pServerManager->setStyleSheet("");
    }

    // Change Designed Mode Font size
    QFont designedFont;
    designedFont.setPixelSize(15);
    ui->txtHostnameDesigned->setFont(designedFont);
    ui->txtPasswordDesigned->setFont(designedFont);
    ui->cmdDesignedLogin->setFont(designedFont);
    ui->labPleaseWait->setFont(designedFont);
    ui->labDesignedLoginDes->setFont(designedFont);
    ui->cbStayLoggedInDesigned->setFont(designedFont);
    ui->cbUseEncryptedConnectionDesigned->setFont(designedFont);
    mouseOverLogin = false;
#ifdef DISABLE_SSL
    ui->cbUseEncryptedConnectionDesigned->setVisible(false);
#endif

    // Change Designed Mode Style
    SMStyleTweaks *SMStyle = new SMStyleTweaks;
    ui->cmdDesignedLogin->setStyle(SMStyle);
    if (designedMode)
    {
        ui->pServerManager->setStyle(SMStyle);
    }

    // Change Icon size
    izSquare = 24;
    ui->lwServer->setIconSize(QSize(izSquare, izSquare));

    // Check for Autologin
    if (smgr->autologinEnabled())
    {
        QTimer::singleShot(10,this,SLOT(connectToServer()));
    }

}

frmServerManager::~frmServerManager()
{
    delete ui;
}

void frmServerManager::connectToServer()
{
    if (smgr->autologinEnabled())
    {
        conWT = new ConnectThread(this, smgr, true);

        connect(conWT, SIGNAL(connectionIssued(bool)), this, SLOT(on_connectionIssued(bool)));
        conWT->start(QThread::LowPriority);

        ui->swSM->setCurrentIndex(pagePleaseWait);
    }
    else
    {
        QStringList remoteHostList = ui->txtHostnameDesigned->text().split(":");
        bool remoteEncrypted = ui->cbUseEncryptedConnectionDesigned->isChecked();
        bool remoteActivateAutologin = ui->cbStayLoggedInDesigned->isChecked();
        QString remotePasswd = ui->txtPasswordDesigned->text();
        QString remoteHost;
        int remotePort;
        if (remoteHostList.length() == 0)
        {
            remoteHost = "SM_LOCAL";
            remotePort = 0;
        }
        else if (remoteHostList.length() == 1)
        {
            remoteHost = remoteHostList.at(0);
            remotePort = 9509;
        }
        else if (remoteHostList.length() == 2)
        {
            bool portOk;
            remoteHost = remoteHostList.at(0);
            remotePort = remoteHostList.at(1).toInt(&portOk);
            if (!portOk)
            {
                QMessageBox::warning(this, ProductName, "Incorrect port format, only numbers are accepted");
                return;
            }
        }
        else
        {
            QMessageBox::warning(this, ProductName, "Incorrect address format");
            return;
        }

        conWT = new ConnectThread(this, smgr, false, remoteActivateAutologin, remoteHost, remotePort, remoteEncrypted, remotePasswd);

        connect(conWT, SIGNAL(connectionIssued(bool)), this, SLOT(on_connectionIssued(bool)));
        conWT->start(QThread::LowPriority);

        ui->swSM->setCurrentIndex(pagePleaseWait);
    }
}

void frmServerManager::on_connectionIssued(bool isSuccess)
{
    if (isSuccess)
    {
        // Visible things for Server Manager Widget
        ui->swSM->setCurrentIndex(pageInterface);

        // Get server list
        QStringList serverList = smgr->getServerList();
        foreach (const QString &serverName, serverList)
        {
            QListWidgetItem *newItem = new QListWidgetItem(serverName);
            newItem->setIcon(standardIcon);
            ui->lwServer->addItem(newItem);
        }

        if (serverList.length() != 0)
        {
            iconWT = new IconThread(smgr, serverList, 24, this);
            iconWTDefined = true;

            connect(iconWT,SIGNAL(setServerIcon(QString,QByteArray)),this,SLOT(setServerIcon(QString,QByteArray)));
            iconWT->start(QThread::LowPriority);
        }
    }
    else
    {
        int lastReturn = smgr->getLastReturnValue();
        if (lastReturn == 404)
        {
            QMessageBox::warning(this, ProductName, tr("Incorrect password for %1 Server!").arg(ProductName));
        }
        else if (lastReturn == 700)
        {
            QMessageBox::warning(this, ProductName, tr("Can't read %1 Server Stream!").arg(ProductName));
        }
        else if (lastReturn == 600)
        {
            QMessageBox::warning(this, ProductName, tr("Can't read %1 Server Stream!").arg(ProductName));
        }
        else if (lastReturn == 800)
        {
            QMessageBox::warning(this, ProductName, tr("Can't connnect to %1 Server!").arg(ProductName));
        }
        else
        {
            QMessageBox::warning(this, ProductName, tr("Can't connnect to %1 Server!").arg(ProductName));
        }

        // Visible things for Server Manager Widget
        ui->swSM->setCurrentIndex(pageLogin);

        // Disable autologin if enabled
        if (smgr->autologinEnabled()) smgr->setAutologinDisabled();

    }
    conWT->terminate();
    conWT->deleteLater();
    delete conWT;
}

void frmServerManager::on_cmdNewServer_clicked()
{
    if (smgr->isConnected())
    {
        bool snok;
        QString serverName;
        serverName = QInputDialog::getText(this,tr("New Server"),tr("Please type a name for this server"),QLineEdit::Normal,"",&snok);
        if (snok)
        {
            if (smgr->addServer(serverName))
            {
                QListWidgetItem *newItem = new QListWidgetItem(serverName);
                newItem->setIcon(standardIcon);
                ui->lwServer->addItem(newItem);
            }
            else
            {
                QMessageBox::information(this,tr("New Server"),tr("Server already exists"));
            }
        }
    }
    else
    {
        QMessageBox::warning(this,tr("New Server"),tr("%1 connection lost").arg(ProductName));
    }
}

void frmServerManager::on_cmdDeleteServer_clicked()
{
    if (smgr->isConnected())
    {
        QList<QListWidgetItem*> selectedItems = ui->lwServer->selectedItems();
        if (selectedItems.length() == 1)
        {
            QListWidgetItem *serverItem = selectedItems.at(0);
            QString serverName = serverItem->text();
            int retb = QMessageBox::question(this,tr("Delete Server"),tr("Are you sure to remove %1 from the server list?").arg(serverName),QMessageBox::Yes | QMessageBox::No,QMessageBox::No);
            if (retb == QMessageBox::Yes)
            {
                smgr->deleteServer(serverName);
                delete serverItem;
            }
        }
        else
        {
            QMessageBox::information(this,tr("Delete Server"),tr("No server is selected"));
        }
    }
    else
    {
        QMessageBox::warning(this,tr("Delete Server"),tr("%1 connection lost").arg(ProductName));
    }
}

void frmServerManager::on_cmdCStart_clicked()
{
    if (smgr->isConnected())
    {
        QList<QListWidgetItem*> selectedItems = ui->lwServer->selectedItems();
        if (selectedItems.length() == 1)
        {
            bool snok;
            QListWidgetItem *serverItem = selectedItems.at(0);
            QString serverName = serverItem->text();
            QString serverStart = smgr->getStartCommand(serverName);
            serverStart = QInputDialog::getText(this,tr("Choose Start"),tr("Please type in start command"),QLineEdit::Normal,serverStart,&snok);
            if (snok)
            {
                if (!smgr->setStartCommand(serverName, serverStart))
                {
                    QMessageBox::warning(this,tr("Choose Start"),tr("%1 server-side error").arg(ProductName));
                }
            }
        }
        else
        {
            QMessageBox::information(this,tr("Choose Start"),tr("No server is selected"));
        }
    }
    else
    {
        QMessageBox::warning(this,tr("Choose Start"),tr("%1 connection lost").arg(ProductName));
    }
}

void frmServerManager::on_cmdCStop_clicked()
{
    if (smgr->isConnected())
    {
        QList<QListWidgetItem*> selectedItems = ui->lwServer->selectedItems();
        if (selectedItems.length() == 1)
        {
            bool snok;
            QListWidgetItem *serverItem = selectedItems.at(0);
            QString serverName = serverItem->text();
            QString serverStop = smgr->getStopCommand(serverName);
            serverStop = QInputDialog::getText(this,tr("Choose Stop"),tr("Please type in stop command"),QLineEdit::Normal,serverStop,&snok);
            if (snok)
            {
                if (!smgr->setStopCommand(serverName, serverStop))
                {
                    QMessageBox::warning(this,tr("Choose Stop"),tr("%1 server-side error").arg(ProductName));
                }
            }
        }
        else
        {
            QMessageBox::information(this,tr("Choose Stop"),tr("No server is selected"));
        }
    }
    else
    {
        QMessageBox::warning(this,tr("Choose Server"),tr("%1 connection lost").arg(ProductName));
    }
}

void frmServerManager::on_cmdCConfig_clicked()
{
    if (smgr->isConnected())
    {
        QList<QListWidgetItem*> selectedItems = ui->lwServer->selectedItems();
        if (selectedItems.length() == 1)
        {
            bool snok;
            QListWidgetItem *serverItem = selectedItems.at(0);
            QString serverName = serverItem->text();
            QString serverConfig = smgr->getConfigCommand(serverName);
            serverConfig = QInputDialog::getText(this,tr("Choose Config"),tr("Please type in config command"),QLineEdit::Normal,serverConfig,&snok);
            if (snok)
            {
                if (!smgr->setConfigCommand(serverName, serverConfig))
                {
                    QMessageBox::warning(this,tr("Choose Config"),tr("%1 server-side error").arg(ProductName));
                }
            }
        }
        else
        {
            QMessageBox::information(this,tr("Choose Config"),tr("No server is selected"));
        }
    }
    else
    {
        QMessageBox::warning(this,tr("Choose Config"),tr("%1 connection lost").arg(ProductName));
    }
}

void frmServerManager::on_cmdCUpdate_clicked()
{
    if (smgr->isConnected())
    {
        QList<QListWidgetItem*> selectedItems = ui->lwServer->selectedItems();
        if (selectedItems.length() == 1)
        {
            bool snok;
            QListWidgetItem *serverItem = selectedItems.at(0);
            QString serverName = serverItem->text();
            QString serverUpdate = smgr->getUpdateCommand(serverName);
            serverUpdate = QInputDialog::getText(this,tr("Choose Update"),tr("Please type in update command"),QLineEdit::Normal,serverUpdate,&snok);
            if (snok)
            {
                if (!smgr->setUpdateCommand(serverName, serverUpdate))
                {
                    QMessageBox::warning(this,tr("Choose Update"),tr("%1 server-side error").arg(ProductName));
                }
            }
        }
        else
        {
            QMessageBox::information(this,tr("Choose Update"),tr("No server is selected"));
        }
    }
    else
    {
        QMessageBox::warning(this,tr("Choose Update"),tr("%1 connection lost").arg(ProductName));
    }
}

void frmServerManager::on_cmdCAttach_clicked()
{
    if (smgr->isConnected())
    {
        QList<QListWidgetItem*> selectedItems = ui->lwServer->selectedItems();
        if (selectedItems.length() == 1)
        {
            bool snok;
            QListWidgetItem *serverItem = selectedItems.at(0);
            QString serverName = serverItem->text();
            QString serverAttach = smgr->getAttachCommand(serverName);
            serverAttach = QInputDialog::getText(this,tr("Choose Attach"),tr("Please type in attach command"),QLineEdit::Normal,serverAttach,&snok);
            if (snok)
            {
                if (!smgr->setAttachCommand(serverName, serverAttach))
                {
                    QMessageBox::warning(this,tr("Choose Attach"),tr("%1 server-side error").arg(ProductName));
                }
            }
        }
        else
        {
            QMessageBox::information(this,tr("Choose Attach"),tr("No server is selected"));
        }
    }
    else
    {
        QMessageBox::warning(this,tr("Choose Attach"),tr("%1 connection lost").arg(ProductName));
    }
}

void frmServerManager::on_cmdStart_clicked()
{
    if (smgr->isConnected())
    {
        QList<QListWidgetItem*> selectedItems = ui->lwServer->selectedItems();
        if (selectedItems.length() == 1)
        {
            QListWidgetItem *serverItem = selectedItems.at(0);
            QString serverName = serverItem->text();
            bool success = smgr->startServer(serverName);
            if (!success)
            {
                if (smgr->getLastReturnValue() == 200 || smgr->getLastReturnValue() == 300)
                {
                    QMessageBox::information(this,tr("Start"),tr("No command registered for start"));
                }
                else
                {
                    QMessageBox::warning(this,tr("Start"),tr("%1 server-side error").arg(ProductName));
                }
            }
        }
        else
        {
            QMessageBox::information(this,tr("Start"),tr("No server is selected"));
        }
    }
    else
    {
        QMessageBox::warning(this,tr("Start"),tr("%1 connection lost").arg(ProductName));
    }
}

void frmServerManager::on_cmdStop_clicked()
{
    if (smgr->isConnected())
    {
        QList<QListWidgetItem*> selectedItems = ui->lwServer->selectedItems();
        if (selectedItems.length() == 1)
        {
            QListWidgetItem *serverItem = selectedItems.at(0);
            QString serverName = serverItem->text();
            bool success = smgr->stopServer(serverName);
            if (!success)
            {
                if (smgr->getLastReturnValue() == 200 || smgr->getLastReturnValue() == 300)
                {
                    QMessageBox::information(this,tr("Stop"),tr("No command registered for stop"));
                }
                else
                {
                    QMessageBox::warning(this,tr("Stop"),tr("%1 server-side error").arg(ProductName));
                }
            }
        }
        else
        {
            QMessageBox::information(this,tr("Stop"),tr("No server is selected"));
        }
    }
    else
    {
        QMessageBox::warning(this,tr("Stop"),tr("%1 connection lost").arg(ProductName));
    }
}

void frmServerManager::on_cmdConfig_clicked()
{
    if (smgr->isConnected())
    {
        QList<QListWidgetItem*> selectedItems = ui->lwServer->selectedItems();
        if (selectedItems.length() == 1)
        {
            QListWidgetItem *serverItem = selectedItems.at(0);
            QString serverName = serverItem->text();
            bool success = smgr->configServer(serverName);
            if (!success)
            {
                if (smgr->getLastReturnValue() == 200 || smgr->getLastReturnValue() == 300)
                {
                    QMessageBox::information(this,tr("Config"),tr("No command registered for config"));
                }
                else
                {
                    QMessageBox::warning(this,tr("Config"),tr("%1 server-side error").arg(ProductName));
                }
            }
        }
        else
        {
            QMessageBox::information(this,tr("Config"),tr("No server is selected"));
        }
    }
    else
    {
        QMessageBox::warning(this,tr("Config"),tr("%1 connection lost").arg(ProductName));
    }
}

void frmServerManager::on_cmdUpdate_clicked()
{
    if (smgr->isConnected())
    {
        QList<QListWidgetItem*> selectedItems = ui->lwServer->selectedItems();
        if (selectedItems.length() == 1)
        {
            QListWidgetItem *serverItem = selectedItems.at(0);
            QString serverName = serverItem->text();
            bool success = smgr->updateServer(serverName);
            if (!success)
            {
                if (smgr->getLastReturnValue() == 200 || smgr->getLastReturnValue() == 300)
                {
                    QMessageBox::information(this,tr("Update"),tr("No command registered for update"));
                }
                else
                {
                    QMessageBox::warning(this,tr("Update"),tr("%1 server-side error").arg(ProductName));
                }
            }
        }
        else
        {
            QMessageBox::information(this,tr("Update"),tr("No server is selected"));
        }
    }
    else
    {
        QMessageBox::warning(this,tr("Update"),tr("%1 connection lost").arg(ProductName));
    }
}

void frmServerManager::on_cmdAttach_clicked()
{
    if (smgr->isConnected())
    {
        QList<QListWidgetItem*> selectedItems = ui->lwServer->selectedItems();
        if (selectedItems.length() == 1)
        {
            QListWidgetItem *serverItem = selectedItems.at(0);
            QString serverName = serverItem->text();
            bool success = smgr->attachServer(serverName);
            if (!success)
            {
                if (smgr->getLastReturnValue() == 200 || smgr->getLastReturnValue() == 300)
                {
                    QMessageBox::information(this,tr("Attach"),tr("No command registered for attach"));
                }
                else
                {
                    QMessageBox::warning(this,tr("Attach"),tr("%1 server-side error").arg(ProductName));
                }
            }
        }
        else
        {
            QMessageBox::information(this,tr("Attach"),tr("No server is selected"));
        }
    }
    else
    {
        QMessageBox::warning(this,tr("Attach"),tr("%1 connection lost").arg(ProductName));
    }
}

void frmServerManager::setAdminMode(bool admin)
{
    if (admin)
    {
        noAdmin = false;
        ui->cmdDAdmin->setVisible(true);
        ui->cmdCAttach->setVisible(true);
        ui->cmdCStart->setVisible(true);
        ui->cmdCStop->setVisible(true);
        ui->cmdCConfig->setVisible(true);
        if (smgr->isConnectionLocal())
        {
            ui->cmdCIcon->setVisible(true);
        }
        ui->cmdCUpdate->setVisible(true);
        ui->cmdNewServer->setVisible(true);
        ui->cmdDeleteServer->setVisible(true);
        ui->cmdAdmin->setVisible(false);
        ui->cmdAttach->setVisible(false);
        ui->cmdStart->setVisible(false);
        ui->cmdStop->setVisible(false);
        ui->cmdConfig->setVisible(false);
        ui->cmdUpdate->setVisible(false);
        ui->cmdDisconnect->setVisible(false);
        ui->cmdConfigLocal->setVisible(false);
        ui->labLocal->setVisible(false);
    }
    else
    {
        noAdmin = true;
        ui->cmdDAdmin->setVisible(false);
        ui->cmdCAttach->setVisible(false);
        ui->cmdCStart->setVisible(false);
        ui->cmdCStop->setVisible(false);
        ui->cmdCConfig->setVisible(false);
        ui->cmdCIcon->setVisible(false);
        ui->cmdCUpdate->setVisible(false);
        ui->cmdNewServer->setVisible(false);
        ui->cmdDeleteServer->setVisible(false);
        ui->cmdAdmin->setVisible(true);
        ui->cmdAttach->setVisible(true);
        ui->cmdStart->setVisible(true);
        ui->cmdStop->setVisible(true);
        ui->cmdConfig->setVisible(true);
        ui->cmdUpdate->setVisible(true);
        ui->cmdDisconnect->setVisible(true);
        ui->cmdConfigLocal->setVisible(true);
        ui->labLocal->setVisible(true);
    }
}

void frmServerManager::on_cmdAdmin_clicked()
{
    if (smgr->isConnected())
    {
        if (smgr->ServerManagerMode == ServerManager::LocalMode)
        {
            QString passwordHash = smgr->getAdminPasswordHash();
            if (passwordHash == "")
            {
                bool ok;
                QString pwInput = QInputDialog::getText(this,tr("Admin"),tr("Type a password for the admin mode and host mode"),QLineEdit::Password,"",&ok);
                if (ok)
                {
                    smgr->setAdminPassword(pwInput);
                    setAdminMode(true);
                }
            }
            else
            {
                bool ok;
                QString pwInput = QInputDialog::getText(this,tr("Admin"),tr("Please type the admin/host password"),QLineEdit::Password,"",&ok);
                if (ok)
                {
                    if (smgr->getPasswordHashFromString(pwInput) == passwordHash)
                    {
                        setAdminMode(true);
                    }
                    else
                    {
                        QMessageBox::warning(this,tr("Admin"),tr("Incorrect password"));
                    }
                }
            }
        }
        else
        {
            setAdminMode(true);
        }
    }
    else
    {
        QMessageBox::warning(this,tr("Admin"),tr("%1 connection lost").arg(ProductName));
    }
}

void frmServerManager::on_cmdDAdmin_clicked()
{
    setAdminMode(false);
}

void frmServerManager::on_cmdCIcon_clicked()
{
    if (smgr->isConnected())
    {
        QList<QListWidgetItem*> selectedItems = ui->lwServer->selectedItems();
        if (selectedItems.length() == 1)
        {
            QListWidgetItem *serverItem = selectedItems.at(0);
            QString serverName = serverItem->text();
            QString iconPath = smgr->getIconPath(serverName);
            QIcon tempIcon = serverItem->icon();
            frmIcon *iconWindow = new frmIcon(this, designedMode);
            iconWindow->setWindowIcon(this->windowIcon());
            iconWindow->setSquareSize(izSquare);
            iconWindow->loadIcons();
            iconWindow->setCurrentIcon(tempIcon,iconPath);
            iconWindow->show();
            iconWindow->exec();
            if (iconWindow->isIconChanged())
            {
                iconPath = iconWindow->getCurrentIconPath();
                tempIcon = iconWindow->getCurrentIcon();
                serverItem->setIcon(tempIcon);
                if (!smgr->setIconPath(serverName, iconPath))
                {
                    QMessageBox::warning(this,tr("Choose Icon"),tr("%1 server-side error").arg(ProductName));
                }
            }
            iconWindow->deleteLater();
            delete iconWindow;
        }
        else
        {
            QMessageBox::information(this,tr("Choose Icon"),tr("No server is selected"));
        }
    }
    else
    {
        QMessageBox::warning(this,tr("Choose Icon"),tr("%1 connection lost").arg(ProductName));
    }
}

void frmServerManager::on_cmdDisconnect_clicked()
{
    if (iconWTDefined)
    {
        iconWT->terminate();
        iconWT->deleteLater();
        iconWTDefined = false;
        delete iconWT;
    }

    smgr->disconnectFromServer();
    smgr->setAutologinDisabled();
    ui->swSM->setCurrentIndex(pageLogin);
    ui->lwServer->clear();
    ui->statusBar->setVisible(false);
    ui->txtPasswordDesigned->clear();
    ui->cbStayLoggedInDesigned->setChecked(false);
    ui->cbUseEncryptedConnectionDesigned->setChecked(false);
    if (ui->txtHostnameDesigned->text() == "")
    {
        ui->txtHostnameDesigned->setFocus();
    }
    else
    {
        ui->txtPasswordDesigned->setFocus();
    }
}

void frmServerManager::setServerIcon(QString serverName, QByteArray iconBytes)
{
    QList<QListWidgetItem*> itemsToReplace = ui->lwServer->findItems(serverName, Qt::MatchExactly);
    if (itemsToReplace.count() == 1)
    {
        QPixmap serverPixmap;
        QBuffer iconBuffer(&iconBytes);
        iconBuffer.open(QIODevice::ReadOnly);
        QDataStream iconIn(&iconBuffer);
        iconIn >> serverPixmap;
        iconBuffer.close();

        QIcon serverIcon;
        serverIcon.addPixmap(serverPixmap);

        QListWidgetItem *itemToReplace = itemsToReplace.at(0);
        itemToReplace->setIcon(serverIcon);
    }
}

void frmServerManager::closeEvent(QCloseEvent *ev)
{
    Q_UNUSED(ev);
    if (iconWTDefined)
    {
        iconWT->terminate();
    }
    qApp->exit(0);
}

void frmServerManager::on_cmdConfigLocal_clicked()
{
    frmConfig *configWindow = new frmConfig(this, languagePath, designedMode);
    configWindow->show();
    configWindow->exec();
    configWindow->deleteLater();
    delete configWindow;
}

void frmServerManager::on_cmdDesignedLogin_clicked()
{
    connectToServer();
}
