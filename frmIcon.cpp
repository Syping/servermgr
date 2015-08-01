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

#include "SMStyleTweaks.h"
#include "PixmapEdit.h"
#include "ui_frmIcon.h"
#include "frmIcon.h"
#include "config.h"
#include <QImageReader>
#include <QFileDialog>
#include <QMessageBox>
#include <QIcon>
#include <QFile>

frmIcon::frmIcon(QWidget *parent, bool designedMode) :
    QDialog(parent), designedMode(designedMode), ui(new Ui::frmIcon)
{
    setWindowFlags(windowFlags()^Qt::WindowContextHelpButtonHint);
    ui->setupUi(this);
    configFile = new QSettings(ProductVendor,"Icon Manager");
    currentIcon = QIcon(ProductImg);
    iconChanged = false;
    firstChange = false;
    squareSize = 24;

    if (!designedMode)
    {
#ifndef SM_UNIX
        ui->cmdPlus->setIcon(QIcon(":/icon/plus-icon_c.png"));
        ui->cmdMinus->setIcon(QIcon(":/icon/minus-icon_c.png"));
#endif
        ui->cmdPlus->setStyleSheet("");
        ui->cmdMinus->setStyleSheet("");
        ui->iconWidget->setStyleSheet("");
    }
    else
    {
#ifdef SM_UNIX
        ui->cmdPlus->setIcon(QIcon());
        ui->cmdMinus->setIcon(QIcon());
#endif
        // Change Designed Mode Style
        SMStyleTweaks *SMStyle = new SMStyleTweaks;
        ui->cmdOkay->setStyle(SMStyle);
        ui->cmdPlus->setStyle(SMStyle);
        ui->cmdMinus->setStyle(SMStyle);
        ui->cmdCancel->setStyle(SMStyle);
        ui->lwIcons->setStyle(SMStyle);
    }

    ui->iconWidgetLayout->setSpacing(6);
    ui->cmdCancel->setDefault(true);
    ui->lwIcons->setFocus();
}

void frmIcon::loadIcons()
{
    iconList = configFile->value("iconList",QStringList(ProductImg)).toStringList();
    iconList.removeDuplicates();
    foreach (const QString &iconPath, iconList)
    {
        if (QFile::exists(iconPath))
        {
            QPixmap tempPixmap = QPixmap(iconPath);
            if (!tempPixmap.isNull())
            {
                if (tempPixmap.size() != QSize(squareSize, squareSize))
                {
                    PixmapEdit sPixmapEdit;
                    sPixmapEdit.setPixmap(tempPixmap);
                    sPixmapEdit.centerPixmapAtSquare(squareSize, true);
                    tempPixmap = sPixmapEdit.getPixmap();
                }
                QIcon tempIcon = QIcon(tempPixmap);
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
    ui->imgCurrentIcon->setPixmap(currentIcon.pixmap(squareSize, squareSize));
}

frmIcon::~frmIcon()
{
    delete ui;
}

void frmIcon::on_cmdOkay_clicked()
{
    if (firstChange)
    {
        iconChanged = true;
    }
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
            QPixmap tempPixmap = QPixmap(fileName);
            if (!tempPixmap.isNull())
            {
                if (tempPixmap.size() != QSize(squareSize, squareSize))
                {
                    PixmapEdit sPixmapEdit;
                    sPixmapEdit.setPixmap(tempPixmap);
                    sPixmapEdit.centerPixmapAtSquare(squareSize, true);
                    tempPixmap = sPixmapEdit.getPixmap();
                }
                QIcon tempIcon = QIcon(tempPixmap);
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
    Q_UNUSED(currentItem);
    Q_UNUSED(previousItem);
    if (firstChange)
    {
        currentIcon = ui->lwIcons->currentItem()->icon();
        currentIconPath = ui->lwIcons->currentItem()->text();
        ui->imgCurrentIcon->setPixmap(currentIcon.pixmap(squareSize, squareSize));
    }
    firstChange = true;
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

void frmIcon::setSquareSize(int _squareSize)
{
    squareSize = _squareSize;
    ui->lwIcons->setIconSize(QSize(squareSize, squareSize));
    ui->imgCurrentIcon->setMinimumSize(QSize(squareSize, squareSize));
    ui->imgCurrentIcon->setMaximumSize(QSize(squareSize, squareSize));
    ui->imgCurrentIcon->setGeometry(ui->imgCurrentIcon->geometry().x(), ui->imgCurrentIcon->y(), squareSize, squareSize);
}

