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

#include "frmIcon.h"
#include "ui_frmIcon.h"
#include "config.h"
#include <QImageReader>
#include <QFileDialog>
#include <QMessageBox>
#include <QIcon>
#include <QFile>

frmIcon::frmIcon(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::frmIcon)
{
    setWindowFlags(windowFlags()^Qt::WindowContextHelpButtonHint);
    ui->setupUi(this);
    ui->cmdCancel->setDefault(true);
    configFile = new QSettings(ProductVendor,"Icon Manager");
    currentIcon = QIcon(ProductImg);
    iconChanged = false;
    iconList = configFile->value("iconList",QStringList(ProductImg)).toStringList();
    iconList.removeDuplicates();
    foreach (const QString &iconPath, iconList)
    {
        if (QFile::exists(iconPath))
        {
            QIcon tempIcon = QIcon(iconPath);
            if (!tempIcon.isNull())
            {
                QListWidgetItem *newItem = new QListWidgetItem(iconPath);
                newItem->setIcon(tempIcon);
                ui->lwIcons->addItem(newItem);
            }
            else
            {
                iconList.removeAll(iconPath);
            }
        }
        else
        {
            iconList.removeAll(iconPath);
        }
    }
    configFile->setValue("iconList",iconList);

}

void frmIcon::setCurrentIcon(QIcon currentIconA, QString iconPath)
{
    currentIconPath = iconPath;
    currentIcon = currentIconA;
    if (!iconList.contains(iconPath))
    {
        QListWidgetItem *newItem = new QListWidgetItem(iconPath);
        newItem->setIcon(currentIconA);
        ui->lwIcons->addItem(newItem);
        newItem->setSelected(true);
        iconList.append(iconPath);
        configFile->setValue("iconList",iconList);
    }
    else
    {
        QList<QListWidgetItem*> itemsToSelect = ui->lwIcons->findItems(iconPath,Qt::MatchExactly);
        if (itemsToSelect.count() == 1)
        {
            QListWidgetItem *itemToSelect = itemsToSelect.at(0);
            itemToSelect->setSelected(true);
        }
    }
    ui->imgCurrentIcon->setPixmap(currentIcon.pixmap(24,24));
}

frmIcon::~frmIcon()
{
    delete ui;
}

void frmIcon::on_cmdOkay_clicked()
{
    iconChanged = true;
    this->close();
}

void frmIcon::on_cmdCancel_clicked()
{
    this->close();
}

void frmIcon::on_cmdMinus_clicked()
{
    QList<QListWidgetItem*> selectedItems = ui->lwIcons->selectedItems();
    if (selectedItems.length() == 1)
    {
        QListWidgetItem *iconItem = selectedItems.at(0);
        QString iconPath = iconItem->text();
        if (iconPath == ProductImg)
        {
            QMessageBox::warning(this,tr("Remove Icon"),tr("Can't remove default icon"));
            return;
        }
        int retb = QMessageBox::question(this,tr("Remove Icon"),tr("Are you sure to remove %1 from the icon list?").arg(iconPath),QMessageBox::Yes | QMessageBox::No,QMessageBox::No);
        if (retb == QMessageBox::Yes)
        {
            iconList.removeAll(iconPath);
            configFile->setValue("iconList",iconList);
            delete iconItem;
        }
    }
    else
    {
        QMessageBox::information(this,tr("Remove Icon"),tr("No server is selected"));
    }
}

void frmIcon::on_cmdPlus_clicked()
{
    QString defaultFilter = "Images (";
    int countFormat = 0;
    foreach(QByteArray imgFormat, QImageReader::supportedImageFormats())
    {
        if (countFormat != 0)
        {
            defaultFilter += " ";
        }
        countFormat++;
        defaultFilter += "*." + QString::fromUtf8(imgFormat);
    }
    defaultFilter += ")";

    QString oldButtonName = ui->cmdCancel->text();
    QString fileName = QFileDialog::getOpenFileName(this,tr("Select Icon"),QDir::homePath(),defaultFilter + ";;Portable Network Graphics (*.png);;All files (**)");
    ui->cmdCancel->setText(oldButtonName);
    if (fileName != "")
    {
        if (QFile::exists(fileName))
        {
            QIcon tempIcon = QIcon(fileName);
            if (!tempIcon.isNull())
            {
                QListWidgetItem *newItem = new QListWidgetItem(fileName);
                newItem->setIcon(tempIcon);
                ui->lwIcons->addItem(newItem);
                iconList.append(fileName);
                configFile->setValue("iconList",iconList);
            }
            else
            {
                QMessageBox::warning(this,tr("Add Icon"),tr("Can't read icon file"));
            }
        }
        else
        {
            QMessageBox::warning(this,tr("Add Icon"),tr("Icon does not exists"));
        }
    }
}

void frmIcon::on_lwIcons_currentItemChanged(QListWidgetItem *currentItem, QListWidgetItem *previousItem)
{
    Q_UNUSED(previousItem);
    currentIcon = currentItem->icon();
    currentIconPath = currentItem->text();
    ui->imgCurrentIcon->setPixmap(currentIcon.pixmap(24,24));
}

bool frmIcon::isIconChanged()
{
    return iconChanged;
}

QIcon frmIcon::getCurrentIcon()
{
    return currentIcon;
}

QString frmIcon::getCurrentIconPath()
{
    return currentIconPath;
}

