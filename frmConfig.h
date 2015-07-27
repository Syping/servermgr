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

#ifndef FRMCONFIG_H
#define FRMCONFIG_H

#include <QDialog>
#include <QMap>

namespace Ui {
    class frmConfig;
}

class frmConfig : public QDialog
{
    Q_OBJECT

public:
    explicit frmConfig(QWidget *parent = 0, QString languagePath = "", bool designedMode = true);
    ~frmConfig();

private slots:
    void on_cmdCancel_clicked();
    void on_cmdOK_clicked();
    void on_cmdChangeAdminPassword_clicked();

private:
    QString languagePath;
    bool designedMode;
    QMap <QString,QString> langWrapper;
    QStringList putStringToStringList(QString string);
    bool designedModeActivated();
    void applySettings();
    Ui::frmConfig *ui;
};

#endif // FRMCONFIG_H
