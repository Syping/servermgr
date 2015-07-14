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
    configFile = new QSettings(ProductVendor,ProductName);
    QStringList serverList = configFile->value("serverList",QStringList()).toStringList();
    standardIcon = QIcon(ProductImg);
    this->setWindowIcon(QIcon(ProductIcon));
    foreach (const QString &serverName, serverList)
    {
        configFile->beginGroup("server " + serverName);
        QString iconPath = configFile->value("icon",ProductImg).toString();
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
        configFile->endGroup();
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
        QStringList serverList = configFile->value("serverList",QStringList()).toStringList();
        if (!serverList.contains(serverName))
        {
            QListWidgetItem *newItem = new QListWidgetItem(serverName);
            newItem->setIcon(standardIcon);
            ui->lwServer->addItem(newItem);
            serverList.append(serverName);
            configFile->setValue("serverList",serverList);
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
        configFile->beginGroup("server " + serverName);
        QString serverStart = configFile->value("start").toString();
        serverStart = QInputDialog::getText(this,tr("Choose Start"),tr("Please type in start command"),QLineEdit::Normal,serverStart,&snok);
        if (snok)
        {
            configFile->setValue("start",serverStart);
        }
        configFile->endGroup();
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
        configFile->beginGroup("server " + serverName);
        QString serverStop = configFile->value("stop").toString();
        serverStop = QInputDialog::getText(this,tr("Choose Stop"),tr("Please type in stop command"),QLineEdit::Normal,serverStop,&snok);
        if (snok)
        {
            configFile->setValue("stop",serverStop);
        }
        configFile->endGroup();
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
        configFile->beginGroup("server " + serverName);
        QString serverConfig = configFile->value("config").toString();
        serverConfig = QInputDialog::getText(this,tr("Choose Config"),tr("Please type in config command"),QLineEdit::Normal,serverConfig,&snok);
        if (snok)
        {
            configFile->setValue("config",serverConfig);
        }
        configFile->endGroup();
    }
    else
    {
        QMessageBox::information(this,tr("Choose Config"),tr("No server is selected"));
    }
}

void frmServerManager::on_cmdChooseUpdate_clicked()
{
    QList<QListWidgetItem*> selectedItems = ui->lwServer->selectedItems();
    if (selectedItems.length() == 1)
    {
        bool snok;
        QListWidgetItem *serverItem = selectedItems.at(0);
        QString serverName = serverItem->text();
        configFile->beginGroup("server " + serverName);
        QString serverUpdate = configFile->value("update").toString();
        serverUpdate = QInputDialog::getText(this,tr("Choose Update"),tr("Please type in update command"),QLineEdit::Normal,serverUpdate,&snok);
        if (snok)
        {
            configFile->setValue("update",serverUpdate);
        }
        configFile->endGroup();
    }
    else
    {
        QMessageBox::information(this,tr("Choose Update"),tr("No server is selected"));
    }
}

void frmServerManager::on_cmdStart_clicked()
{
    QList<QListWidgetItem*> selectedItems = ui->lwServer->selectedItems();
    if (selectedItems.length() == 1)
    {
        QListWidgetItem *serverItem = selectedItems.at(0);
        QString serverName = serverItem->text();
        configFile->beginGroup("server " + serverName);
        QString serverStart = configFile->value("start","").toString();
        if (serverStart != "")
        {
            QProcess serverProcess;
            serverProcess.startDetached(serverStart);
        }
        else
        {
            QMessageBox::information(this,tr("Start"),tr("No command registered for start"));
        }
        configFile->endGroup();
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
        configFile->beginGroup("server " + serverName);
        QString serverStop = configFile->value("stop","").toString();
        if (serverStop != "")
        {
            QProcess serverProcess;
            serverProcess.startDetached(serverStop);
        }
        else
        {
            QMessageBox::information(this,tr("Stop"),tr("No command registered for stop"));
        }
        configFile->endGroup();
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
        configFile->beginGroup("server " + serverName);
        QString serverConfig = configFile->value("config","").toString();
        if (serverConfig != "")
        {
            QProcess serverProcess;
            serverProcess.startDetached(serverConfig);
        }
        else
        {
            QMessageBox::information(this,tr("Config"),tr("No command registered for config"));
        }
        configFile->endGroup();
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
        configFile->beginGroup("server " + serverName);
        QString serverUpdate = configFile->value("update","").toString();
        if (serverUpdate != "")
        {
            QProcess serverProcess;
            serverProcess.startDetached(serverUpdate);
        }
        else
        {
            QMessageBox::information(this,tr("Update"),tr("No command registered for update"));
        }
        configFile->endGroup();
    }
    else
    {
        QMessageBox::information(this,tr("Update"),tr("No server is selected"));
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
        ui->cmdChooseUpdate->setVisible(true);
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
        ui->cmdChooseUpdate->setVisible(false);
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
    configFile->beginGroup("auth");
    QString password = configFile->value("password","").toString();
    if (password == "")
    {
        bool ok;
        QString pwInput = QInputDialog::getText(this,tr("Server Manager Admin"),tr("Type a password for the admin mode"),QLineEdit::Password,"",&ok);
        if (ok)
        {
            QCryptographicHash *pwHash = new QCryptographicHash(QCryptographicHash::Md4);
            pwHash->addData(pwInput.toUtf8());
            QString hashString = QString::fromUtf8(pwHash->result().toBase64());
            configFile->setValue("password",hashString);
            setAdminMode(true);
        }
    }
    else
    {
        bool ok;
        QString pwInput = QInputDialog::getText(this,tr("Server Manager Admin"),tr("Please type the admin password"),QLineEdit::Password,"",&ok);
        if (ok)
        {
            QCryptographicHash *pwHash = new QCryptographicHash(QCryptographicHash::Md4);
            pwHash->addData(pwInput.toUtf8());
            QString hashString = QString::fromUtf8(pwHash->result().toBase64());
            if (password == hashString)
            {
                setAdminMode(true);
            }
            else
            {
                QMessageBox::warning(this,tr("Server Manager Admin"),tr("Incorrect password"));
            }
        }
    }
    configFile->endGroup();
}

void frmServerManager::on_cmdDAdmin_clicked()
{
    setAdminMode(false);
}

void frmServerManager::on_cmdAttach_clicked()
{
    QList<QListWidgetItem*> selectedItems = ui->lwServer->selectedItems();
    if (selectedItems.length() == 1)
    {
        QListWidgetItem *serverItem = selectedItems.at(0);
        QString serverName = serverItem->text();
        configFile->beginGroup("server " + serverName);
        QString serverStart = configFile->value("attach","").toString();
        if (serverStart != "")
        {
            QProcess serverProcess;
            serverProcess.startDetached(serverStart);
        }
        else
        {
            QMessageBox::information(this,tr("Attach"),tr("No command registered for attach"));
        }
        configFile->endGroup();
    }
    else
    {
        QMessageBox::information(this,tr("Attach"),tr("No server is selected"));
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
        configFile->beginGroup("server " + serverName);
        QString serverStart = configFile->value("attach").toString();
        serverStart = QInputDialog::getText(this,tr("Choose Attach"),tr("Please type in attach command"),QLineEdit::Normal,serverStart,&snok);
        if (snok)
        {
            configFile->setValue("attach",serverStart);
        }
        configFile->endGroup();
    }
    else
    {
        QMessageBox::information(this,tr("Choose Attach"),tr("No server is selected"));
    }
}

void frmServerManager::on_cmdCIcon_clicked()
{
    QList<QListWidgetItem*> selectedItems = ui->lwServer->selectedItems();
    if (selectedItems.length() == 1)
    {
        QListWidgetItem *serverItem = selectedItems.at(0);
        QString serverName = serverItem->text();
        configFile->beginGroup("server " + serverName);
        QString iconPath = configFile->value("icon",ProductImg).toString();
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
            configFile->setValue("icon",iconPath);
        }
        configFile->endGroup();
        iconWindow->deleteLater();
        delete iconWindow;
    }
    else
    {
        QMessageBox::information(this,tr("Choose Icon"),tr("No server is selected"));
    }
}
