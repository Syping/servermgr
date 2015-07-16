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
#include <QInputDialog>
#include <QMessageBox>
#include <QSettings>
#include <QProcess>
#include <QDebug>
#include <QFile>
#include <QIcon>

frmServerManager::frmServerManager(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::frmServerManager)
{
    ui->setupUi(this);
    smgr = new ServerManager(this);
    standardIcon = QIcon(ProductImg);
    this->setWindowIcon(QIcon(ProductIcon));
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
    setAdminMode(false);
}

frmServerManager::~frmServerManager()
{
    delete ui;
}

void frmServerManager::on_cmdNewServer_clicked()
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

void frmServerManager::on_cmdDeleteServer_clicked()
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

void frmServerManager::on_cmdCStart_clicked()
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

void frmServerManager::on_cmdCStop_clicked()
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

void frmServerManager::on_cmdCConfig_clicked()
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

void frmServerManager::on_cmdCUpdate_clicked()
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

void frmServerManager::on_cmdCAttach_clicked()
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

void frmServerManager::on_cmdStart_clicked()
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

void frmServerManager::on_cmdStop_clicked()
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

void frmServerManager::on_cmdConfig_clicked()
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

void frmServerManager::on_cmdUpdate_clicked()
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

void frmServerManager::on_cmdAttach_clicked()
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
        ui->cmdCIcon->setVisible(true);
        ui->cmdCUpdate->setVisible(true);
        ui->cmdNewServer->setVisible(true);
        ui->cmdDeleteServer->setVisible(true);
        ui->cmdAdmin->setVisible(false);
        ui->cmdAttach->setVisible(false);
        ui->cmdStart->setVisible(false);
        ui->cmdStop->setVisible(false);
        ui->cmdConfig->setVisible(false);
        ui->cmdUpdate->setVisible(false);
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
    }
}

void frmServerManager::on_cmdAdmin_clicked()
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

void frmServerManager::on_cmdDAdmin_clicked()
{
    setAdminMode(false);
}

void frmServerManager::on_cmdCIcon_clicked()
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
