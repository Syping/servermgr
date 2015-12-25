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
#include "ServerManager.h"
#include "ui_frmConfig.h"
#include "frmConfig.h"
#include "config.h"
#include <QStyleFactory>
#include <QInputDialog>
#include <QTranslator>
#include <QMessageBox>
#include <QSettings>
#include <QProcess>
#include <QLocale>
#include <QDebug>
#include <QFile>
#include <QDir>

frmConfig::frmConfig(QWidget *parent, QString languagePath, bool designedMode) :
    QDialog(parent), languagePath(languagePath), designedMode(designedMode), ui(new Ui::frmConfig)
{
    setWindowFlags(windowFlags()^Qt::WindowContextHelpButtonHint);
    ui->setupUi(this);
    QSettings settings(ProductVendor,ProductName);

    //// Interface
    // Font
    settings.beginGroup("Interface");
    bool classicMode = false;
    if (!designedMode)
    {
        classicMode = true;
    }
    else
    {
        SMStyleTweaks *SMStyle = new SMStyleTweaks;
        ui->cmdChangeAdminPassword->setStyle(SMStyle);
        ui->cmdCancel->setStyle(SMStyle);
        ui->cmdOK->setStyle(SMStyle);

        QFile *CWStyleFile = new QFile(":/smstyle/configWidget.qss");
        if (CWStyleFile->open(QFile::ReadOnly))
        {
            ui->configWidget->setStyleSheet(QString::fromUtf8(CWStyleFile->readAll()));
            CWStyleFile->close();
        }
    }
    ui->cbSMStyle->setChecked(classicMode);

    // Language
    langWrapper["System"] = "System (System)";
    langWrapper["System (System)"] = "System";
    langWrapper["English (English)"] = "en";
    langWrapper["en"] = "English (English)";
    ui->cbLanguage->addItem(QIcon::fromTheme("flag-us",QIcon(":/tr/us.png")),"English (English)");
    QDir trDir;
    trDir.cd(":/tr");
    foreach(const QString& key, trDir.entryList(putStringToStringList("sm_*.qm")))
    {
        QTranslator trFile;
        bool ok = trFile.load(QString(":/tr/" + key));
        if (ok)
        {
            QString trName = key;
            trName = trName.remove("sm_").remove(".qm");
            QLocale trLocale(trName);
#if QT_VERSION >= 0x040800
            QString trNative = trLocale.nativeLanguageName();
#else
            QString trNative = QLocale::languageToString(trLocale.language());
#endif
            QString trInter = QLocale::languageToString(trLocale.language());
            if (trNative != "")
            {
                langWrapper[trName] = trNative + " (" + trInter + ")";
                langWrapper[trNative + " (" + trInter + ")"] = trName;
                if (QFile::exists(QString(":/tr/" + trName + ".png")))
                {
                    ui->cbLanguage->addItem(QIcon::fromTheme("flag-" + trName,QIcon(":/tr/" + trName + ".png")),trNative + " (" + trInter + ")");
                }
                else
                {
                    ui->cbLanguage->addItem(QIcon::fromTheme("flag-" + trName),trNative + " (" + trInter + ")");
                }
            }
        }
    }
    QDir trPlugsDir;
    trPlugsDir.cd(languagePath);
    foreach(const QString& key, trPlugsDir.entryList(putStringToStringList("sm_*.qm")))
    {
        QTranslator trFile;
        bool ok = trFile.load(QString(languagePath + "/" + key));
        if (ok)
        {
            QString trName = key;
            trName = trName.remove("sm_").remove(".qm");
            QLocale trLocale(trName);
#if QT_VERSION >= 0x040800
            QString trNative = trLocale.nativeLanguageName();
#else
            QString trNative = QLocale::languageToString(trLocale.language());
#endif
            QString trInter = QLocale::languageToString(trLocale.language());
            if (trNative != "")
            {
                langWrapper[trName] = trNative + " (" + trInter + ")";
                langWrapper[trNative + " (" + trInter + ")"] = trName;
                if (QFile::exists(QString(":/tr/" + trName + ".png")))
                {
                    ui->cbLanguage->addItem(QIcon::fromTheme("flag-" + trName,QIcon(":/tr/" + trName + ".png")),trNative + " (" + trInter + ")");
                }
                else
                {
                    ui->cbLanguage->addItem(QIcon::fromTheme("flag-" + trName),trNative + " (" + trInter + ")");
                }
            }
        }
    }
    QString langVar = settings.value("Language","System").toString();
    if (langWrapper.contains(langVar))
    {
        QString langName = langWrapper[langVar];
        ui->cbLanguage->setCurrentIndex(ui->cbLanguage->findText(langName));
    }
    settings.endGroup();
    //// Interface

    // Style
    ui->cbStyle->addItems(QStyleFactory::keys());
    QStringList styles;
    styles << "System" << QStyleFactory::keys();

    settings.beginGroup("Style");
#ifdef SM_UNIX
    bool loadCustom = settings.value("Custom", false).toBool();
    if (loadCustom)
    {
        QString style = settings.value("Name", "System").toString();
        if (style != "System")
        {
            if (style.trimmed() != "")
            {
                if (styles.contains(style))
                {
                    ui->cbStyle->setCurrentIndex(ui->cbStyle->findText(style));
                }
            }
        }
    }
#else
    bool loadCustom = settings.value("Custom", true).toBool();
    if (loadCustom)
    {
        QString style = settings.value("Name", "Fusion").toString();
        if (style != "System")
        {
            if (style.trimmed() != "")
            {
                if (styles.contains(style))
                {
                    ui->cbStyle->setCurrentIndex(ui->cbStyle->findText(style));
                }
            }
        }
    }
#endif
    settings.endGroup();

    ui->configWidgetLayout->setSpacing(6);
    ui->cmdCancel->setDefault(true);
    ui->cbStyle->setFocus();
}

QStringList frmConfig::putStringToStringList(QString string)
{
    QStringList strlist;
    strlist << string;
    return strlist;
}

frmConfig::~frmConfig()
{
    delete ui;
}

void frmConfig::on_cmdCancel_clicked()
{
    this->close();
}

void frmConfig::on_cmdOK_clicked()
{
    applySettings();
    this->close();
}

void frmConfig::applySettings()
{
    QSettings settings(ProductVendor,ProductName);

    bool restartSM = false;

    //// Interface
    // Font
    settings.beginGroup("Interface");
    bool classicMode = ui->cbSMStyle->isChecked();
    if (classicMode == designedMode) restartSM = true;
    if (classicMode)
    {
        designedMode = false;
    }
    else
    {
        designedMode = true;
    }
    settings.setValue("designedMode",designedMode);

    // Language
    QString oldLang = settings.value("Language","System").toString();
    QString newLang = langWrapper[ui->cbLanguage->currentText()];
    if (oldLang != newLang)
    {
        settings.setValue("Language",newLang);
        restartSM = true;
    }
    settings.endGroup();

    //// Interface

    // Style
    settings.beginGroup("Style");
    if (ui->cbStyle->currentIndex() == 0)
    {
        settings.setValue("Custom",false);
        restartSM = true;
    }
    else
    {
        QString newStyle = ui->cbStyle->currentText();
        settings.setValue("Custom",true);
        settings.setValue("showBug",true);
        settings.setValue("Name",newStyle);
        restartSM = true;
    }
    settings.endGroup();

    if (restartSM)
    {
        QProcess *newSM = new QProcess;
        newSM->startDetached("\"" + qApp->applicationFilePath() + "\"");
        newSM->deleteLater();
        qApp->exit(0);
    }
}

bool frmConfig::designedModeActivated()
{
    return designedMode;
}

void frmConfig::on_cmdChangeAdminPassword_clicked()
{
    ServerManager *psmgr = new ServerManager();
    psmgr->ServerManagerMode = ServerManager::LocalMode;
    QString passwordHash = psmgr->getAdminPasswordHash();
    QInputDialog inputDialog;
    if (passwordHash == "")
    {
        bool ok;
        QString pwInput = inputDialog.getText(this, tr("Admin"), tr("Type a password for the admin mode and host mode"), QLineEdit::Password, "", &ok, inputDialog.windowFlags()^Qt::WindowContextHelpButtonHint);
        if (ok)
        {
            psmgr->setAdminPassword(pwInput);
        }
    }
    else
    {
        bool ok;
        QString pwInput = inputDialog.getText(this, tr("Admin"), tr("Please type the admin/host password"), QLineEdit::Password, "", &ok, inputDialog.windowFlags()^Qt::WindowContextHelpButtonHint);
        if (ok)
        {
            if (psmgr->getPasswordHashFromString(pwInput) == passwordHash)
            {
                pwInput = inputDialog.getText(this, tr("Admin"), tr("Type a new password for the admin mode and host mode"), QLineEdit::Password, "", &ok, inputDialog.windowFlags()^Qt::WindowContextHelpButtonHint);
                if (ok)
                {
                    psmgr->setAdminPassword(pwInput);
                }
                else
                {
                    QMessageBox::warning(this, tr("Admin"), tr("Password not changed"));
                }
            }
            else
            {
                QMessageBox::warning(this, tr("Admin"), tr("Incorrect password"));
            }
        }
    }
    psmgr->deleteLater();
}
