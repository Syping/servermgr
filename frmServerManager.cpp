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

#include "frmServerManager.h"
#include "ui_frmServerManager.h"
#include "frmIcon.h"
#include "config.h"
#include <QCryptographicHash>
#include <QStyleFactory>
#include <QCommonStyle>
#include <QInputDialog>
#include <QMessageBox>
#include <QSettings>
#include <QProcess>
#include <QTimer>
#include <QStyle>
#include <QDebug>
#include <QFile>
#include <QFont>
#include <QIcon>

frmServerManager::frmServerManager(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::frmServerManager)
{
    ui->setupUi(this);
    autoLogin = false;
    smgr = new ServerManager(this);
    standardIcon = QIcon(ProductImg);
    QIcon windowIcon = QIcon(ProductIcon);
    windowIcon.addPixmap(QPixmap(ProductImg));
    windowIcon.addPixmap(QPixmap(ProductPixmap));
    this->setWindowIcon(windowIcon);

    // Change Visibles
    ui->swSM->setCurrentIndex(1);
    ui->labDesignedLoginDes->setVisible(false);
    setAdminMode(false);

    // Change Designed Mode Font size
    QFont designedFont;
    designedFont.setPixelSize(15);
    ui->txtHostnameDesigned->setFont(designedFont);
    ui->txtPasswordDesigned->setFont(designedFont);
    ui->labDesignedLogin->setFont(designedFont);
    ui->labDesignedLoginDes->setFont(designedFont);
    ui->cbStayLoggedInDesigned->setFont(designedFont);
    ui->cbUseEncryptedConnectionDesigned->setFont(designedFont);
    mouseOverLogin = false;
    setWidgetDesign();
#ifdef DISABLE_SSL
    ui->cbUseEncryptedConnectionDesigned->setVisible(false);
#endif

    // Start Timer
    QTimer::singleShot(10,this,SLOT(on_timerLB_ticked()));

    // Check for Autologin
    if (smgr->autologinEnabled())
    {
        autoLogin = true;
        connectToServer();
    }

}

frmServerManager::~frmServerManager()
{
    delete ui;
}

void frmServerManager::connectToServer()
{
    if (autoLogin)
    {
        bool connectionSuccess = smgr->connectToServerWithAutologin();
        if (!connectionSuccess)
        {
            QMessageBox::warning(this,tr("Server Manager"),tr("Can't connnect to %1 Server!").arg("Server Manager"));
            autoLogin = false;
            return;
        }
    }
    else if (ui->txtHostnameDesigned->text() == "SM_LOCAL")
    {
        smgr->ServerManagerMode = ServerManager::LocalMode;
        if (ui->cbStayLoggedInDesigned->isChecked())
        {
            smgr->setAutologinEnabled(ui->txtHostnameDesigned->text(),ui->txtPasswordDesigned->text(),9509,ui->cbUseEncryptedConnectionDesigned->isChecked());
        }
    }
    else
    {
        smgr->ServerManagerMode = ServerManager::RemoteMode;
        bool connectionSuccess = smgr->connectToServer(ui->txtHostnameDesigned->text(),ui->txtPasswordDesigned->text(),9509,ui->cbUseEncryptedConnectionDesigned->isChecked());
        if (!connectionSuccess)
        {
            QMessageBox::warning(this,tr("Server Manager"),tr("Can't connnect to %1 Server!").arg("Server Manager"));
            return;
        }
        if (ui->cbStayLoggedInDesigned->isChecked())
        {
            smgr->setAutologinEnabled(ui->txtHostnameDesigned->text(),ui->txtPasswordDesigned->text(),9509,ui->cbUseEncryptedConnectionDesigned->isChecked());
        }
    }

    ui->swSM->setCurrentIndex(0);

    QStringList serverList = smgr->getServerList();
    foreach (const QString &serverName, serverList)
    {
        QString iconPath = smgr->getIconPath(serverName);
        QIcon serverIcon;
        if (QFile::exists(iconPath))
        {
            QIcon tempIcon = QIcon(iconPath);
            if (!tempIcon.isNull())
            {
                serverIcon = tempIcon;
            }
        }
        else
        {
            serverIcon = standardIcon;
        }
        QListWidgetItem *newItem = new QListWidgetItem(serverName);
        newItem->setIcon(serverIcon);
        ui->lwServer->addItem(newItem);
    }
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
        QMessageBox::warning(this,tr("New Server"),tr("Server Manager connection lost"));
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
        QMessageBox::warning(this,tr("Delete Server"),tr("Server Manager connection lost"));
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
                smgr->setStartCommand(serverName, serverStart);
            }
        }
        else
        {
            QMessageBox::information(this,tr("Choose Start"),tr("No server is selected"));
        }
    }
    else
    {
        QMessageBox::warning(this,tr("Choose Server"),tr("Server Manager connection lost"));
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
                smgr->setStopCommand(serverName, serverStop);
            }
        }
        else
        {
            QMessageBox::information(this,tr("Choose Stop"),tr("No server is selected"));
        }
    }
    else
    {
        QMessageBox::warning(this,tr("Choose Server"),tr("Server Manager connection lost"));
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
                smgr->setConfigCommand(serverName, serverConfig);
            }
        }
        else
        {
            QMessageBox::information(this,tr("Choose Config"),tr("No server is selected"));
        }
    }
    else
    {
        QMessageBox::warning(this,tr("Choose Config"),tr("Server Manager connection lost"));
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
                smgr->setUpdateCommand(serverName, serverUpdate);
            }
        }
        else
        {
            QMessageBox::information(this,tr("Choose Update"),tr("No server is selected"));
        }
    }
    else
    {
        QMessageBox::warning(this,tr("Choose Update"),tr("Server Manager connection lost"));
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
                smgr->setAttachCommand(serverName, serverAttach);
            }
        }
        else
        {
            QMessageBox::information(this,tr("Choose Attach"),tr("No server is selected"));
        }
    }
    else
    {
        QMessageBox::warning(this,tr("Choose Attach"),tr("Server Manager connection lost"));
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
            if (!smgr->startServer(serverName))
            {
                QMessageBox::information(this,tr("Start"),tr("No command registered for start"));
            }
        }
        else
        {
            QMessageBox::information(this,tr("Start"),tr("No server is selected"));
        }
    }
    else
    {
        QMessageBox::warning(this,tr("Start"),tr("Server Manager connection lost"));
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
            if (!smgr->stopServer(serverName))
            {
                QMessageBox::information(this,tr("Stop"),tr("No command registered for stop"));
            }
        }
        else
        {
            QMessageBox::information(this,tr("Stop"),tr("No server is selected"));
        }
    }
    else
    {
        QMessageBox::warning(this,tr("Stop"),tr("Server Manager connection lost"));
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
            if (!smgr->configServer(serverName))
            {
                QMessageBox::information(this,tr("Config"),tr("No command registered for config"));
            }
        }
        else
        {
            QMessageBox::information(this,tr("Config"),tr("No server is selected"));
        }
    }
    else
    {
        QMessageBox::warning(this,tr("Config"),tr("Server Manager connection lost"));
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
            if (!smgr->updateServer(serverName))
            {
                QMessageBox::information(this,tr("Update"),tr("No command registered for update"));
            }
        }
        else
        {
            QMessageBox::information(this,tr("Update"),tr("No server is selected"));
        }
    }
    else
    {
        QMessageBox::warning(this,tr("Update"),tr("Server Manager connection lost"));
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
            if (!smgr->attachServer(serverName))
            {
                QMessageBox::information(this,tr("Attach"),tr("No command registered for attach"));
            }
        }
        else
        {
            QMessageBox::information(this,tr("Attach"),tr("No server is selected"));
        }
    }
    else
    {
        QMessageBox::warning(this,tr("Attach"),tr("Server Manager connection lost"));
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
                QString pwInput = QInputDialog::getText(this,tr("Server Manager Admin"),tr("Type a password for the admin mode"),QLineEdit::Password,"",&ok);
                if (ok)
                {
                    smgr->setAdminPassword(pwInput);
                    setAdminMode(true);
                }
            }
            else
            {
                bool ok;
                QString pwInput = QInputDialog::getText(this,tr("Server Manager Admin"),tr("Please type the admin password"),QLineEdit::Password,"",&ok);
                if (ok)
                {
                    if (smgr->getPasswordHashFromString(pwInput) == passwordHash)
                    {
                        setAdminMode(true);
                    }
                    else
                    {
                        QMessageBox::warning(this,tr("Server Manager Admin"),tr("Incorrect password"));
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
        QMessageBox::warning(this,tr("Server Manager Admin"),tr("Server Manager connection lost"));
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
            QIcon tempIcon;
            if (QFile::exists(iconPath))
            {
                tempIcon = QIcon(iconPath);
                if (tempIcon.isNull())
                {
                    iconPath = ProductImg;
                    tempIcon = QIcon(iconPath);
                }
            }
            else
            {
                iconPath = ProductImg;
                tempIcon = QIcon(iconPath);
            }
            frmIcon *iconWindow = new frmIcon(this);
            iconWindow->setWindowIcon(this->windowIcon());
            iconWindow->setCurrentIcon(tempIcon,iconPath);
            iconWindow->show();
            iconWindow->exec();
            if (iconWindow->isIconChanged())
            {
                iconPath = iconWindow->getCurrentIconPath();
                tempIcon = iconWindow->getCurrentIcon();
                serverItem->setIcon(tempIcon);
                smgr->setIconPath(serverName, iconPath);
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
        QMessageBox::warning(this,tr("Choose Icon"),tr("Server Manager connection lost"));
    }
}

void frmServerManager::setWidgetDesign()
{
#ifdef QT4
    QStyle *KMStyle = QStyleFactory::create("cleanlooks");
#else
#ifdef QT5
    QStyle *KMStyle = QStyleFactory::create("fusion");
#else
    QCommonStyle *KMStyle = new QCommonStyle();
#endif
#endif
    ui->labDesignedLogin->setStyle(KMStyle);
}

void frmServerManager::on_timerLB_ticked()
{
    if (ui->labDesignedLogin->rect().contains(ui->labDesignedLogin->mapFromGlobal(QCursor::pos())))
    {
        if (!mouseOverLogin)
        {
            ui->labDesignedLogin->setStyleSheet("background-color: rgb(255, 255, 255); border-color: rgb(215, 120, 50); color: rgb(215, 70, 25);");
        }
        mouseOverLogin = true;
    }
    else
    {
        if (mouseOverLogin)
        {
            ui->labDesignedLogin->setStyleSheet("background-color: rgb(215, 70, 25); border-color: rgb(215, 120, 50);");
        }
        mouseOverLogin = false;
    }
    QTimer::singleShot(10,this,SLOT(on_timerLB_ticked()));
}

void frmServerManager::on_labDesignedLogin_mouseRelease(QMouseEvent *)
{
    if (ui->labDesignedLogin->rect().contains(ui->labDesignedLogin->mapFromGlobal(QCursor::pos())))
    {
        connectToServer();
    }
}

void frmServerManager::on_cmdDisconnect_clicked()
{
    autoLogin = false;
    smgr->disconnectFromServer();
    smgr->setAutologinDisabled();
    ui->swSM->setCurrentIndex(1);
    ui->lwServer->clear();
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
