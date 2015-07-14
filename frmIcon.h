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

#ifndef FRMICON_H
#define FRMICON_H

#include <QListWidgetItem>
#include <QStringList>
#include <QSettings>
#include <QDialog>
#include <QString>
#include <QIcon>

namespace Ui {
class frmIcon;
}

class frmIcon : public QDialog
{
    Q_OBJECT

public:
    explicit frmIcon(QWidget *parent = 0);
    QIcon getCurrentIcon();
    QString getCurrentIconPath();
    void setCurrentIcon(QIcon currentIcon, QString iconPath);
    bool isIconChanged();
    ~frmIcon();

private slots:
    void on_cmdOkay_clicked();
    void on_cmdCancel_clicked();
    void on_cmdMinus_clicked();
    void on_cmdPlus_clicked();
    void on_lwIcons_currentItemChanged(QListWidgetItem *currentItem, QListWidgetItem *previousItem);

private:
    QSettings *configFile;
    QString currentIconPath;
    QIcon currentIcon;
    QStringList iconList;
    bool iconChanged;
    Ui::frmIcon *ui;
};

#endif // FRMICON_H
