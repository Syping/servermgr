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
#include "ServerManager.h"
#include "config.h"

#include <QApplication>
#include <QTranslator>
#include <QLocale>
#include <QDebug>
#include <QFile>
#include <QDir>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    a.setApplicationName(ProductName);
    a.setApplicationVersion(ProductVersion);

    QSettings settings(ProductVendor,ProductName);

    settings.beginGroup("Interface");
    QString language = settings.value("Language","System").toString();
    bool designedMode = settings.value("designedMode",true).toBool();
    settings.endGroup();

    // Start external translate loading
    QString trplugspath = TranslationPath;
    QDir dirTr;
    trplugspath = trplugspath.replace("$ED",QFileInfo(a.applicationFilePath()).absoluteDir().absolutePath());
    trplugspath = trplugspath.replace("$WD",dirTr.absolutePath());
    qDebug() << "Translation path" << trplugspath;
    bool trsf = false;
    bool svlp = false;
    QTranslator EappTranslator;
    if (language == "System" || language.trimmed() == "")
    {
        QString languageName = QLocale::system().name();
        QStringList langList = languageName.split("_");
        if (langList.length() >= 1)
        {
            if (QFile::exists(trplugspath + "/sm_" + langList.at(0) + ".qm"))
            {
                EappTranslator.load(trplugspath + "/sm_" + langList.at(0) + ".qm");
            }
        }
    }
    else
    {
        QString languageName = language;
        QStringList langList = languageName.split("_");
        if (langList.length() >= 1)
        {
            if (QFile::exists(trplugspath + "/sm_" + langList.at(0) + ".qm"))
            {
                if (!EappTranslator.load(trplugspath + "/sm_" + langList.at(0) + ".qm"))
                {
                    if (langList.at(0) != "en")
                    {
                        trsf = true;
                    }
                }
                else
                {
                    svlp = true;
                }
            }
            else
            {
                if (langList.at(0) != "en")
                {
                    trsf = true;
                }
            }
        }
    }
    if (trsf)
    {
        QString languageName = QLocale::system().name();
        QStringList langList = languageName.split("_");
        if (langList.length() >= 1)
        {
            if (QFile::exists(trplugspath + "/sm_" + langList.at(0) + ".qm"))
            {
                EappTranslator.load(trplugspath + "/sm_" + langList.at(0) + ".qm");
            }
        }
    }
    a.installTranslator(&EappTranslator);
#ifdef QT4
    QTranslator EqtTranslator;
    if (language == "System" || language.trimmed() == "")
    {
        QString languageName = QLocale::system().name();
        QStringList langList = languageName.split("_");
        if (langList.length() >= 1)
        {
            if (QFile::exists(trplugspath + "/qt_" + langList.at(0) + ".qm"))
            {
                EqtTranslator.load(trplugspath + "/qt_" + langList.at(0) + ".qm");
            }
        }
    }
    else
    {
        QString languageName = language;
        QStringList langList = languageName.split("_");
        if (langList.length() >= 1)
        {
            if (QFile::exists(trplugspath + "/qt_" + langList.at(0) + ".qm"))
            {
                EqtTranslator.load(trplugspath + "/qt_" + langList.at(0) + ".qm");
            }
        }
    }
    if (trsf)
    {
        QString languageName = QLocale::system().name();
        QStringList langList = languageName.split("_");
        if (langList.length() >= 1)
        {
            if (QFile::exists(trplugspath + "/qt_" + langList.at(0) + ".qm"))
            {
                EqtTranslator.load(trplugspath + "/qt_" + langList.at(0) + ".qm");
            }
        }
    }
    a.installTranslator(&EqtTranslator);
#else
#ifdef QT5
    QTranslator EqtTranslator1;
    if (language == "System" || language.trimmed() == "")
    {
        QString languageName = QLocale::system().name();
        QStringList langList = languageName.split("_");
        if (langList.length() >= 1)
        {
            if (QFile::exists(trplugspath + "/qtbase_" + langList.at(0) + ".qm"))
            {
                EqtTranslator1.load(trplugspath + "/qtbase_" + langList.at(0) + ".qm");
            }
        }
    }
    else
    {
        QString languageName = language;
        QStringList langList = languageName.split("_");
        if (langList.length() >= 1)
        {
            if (QFile::exists(trplugspath + "/qtbase_" + langList.at(0) + ".qm"))
            {
                EqtTranslator1.load(trplugspath + "/qtbase_" + langList.at(0) + ".qm");
            }
        }
    }
    if (trsf)
    {
        QString languageName = QLocale::system().name();
        QStringList langList = languageName.split("_");
        if (langList.length() >= 1)
        {
            if (QFile::exists(trplugspath + "/qtbase_" + langList.at(0) + ".qm"))
            {
                EqtTranslator1.load(trplugspath + "/qtbase_" + langList.at(0) + ".qm");
            }
        }
    }
    a.installTranslator(&EqtTranslator1);
#endif
#endif
    // End external translate loading
    // Start internal translate loading
    QTranslator appTranslator;
    if (language == "System" || language.trimmed() == "")
    {
        QString languageName = QLocale::system().name();
        QStringList langList = languageName.split("_");
        if (langList.length() >= 1)
        {
            if (QFile::exists(":/tr/sm_" + langList.at(0) + ".qm"))
            {
                if (!appTranslator.load(":/tr/sm_" + langList.at(0) + ".qm"))
                {
                    if (langList.at(0) != "en")
                    {
                        if (svlp) {trsf = true;}
                    }
                }
            }
            else
            {
                if (langList.at(0) != "en")
                {
                    if (svlp) {trsf = true;}
                }
            }
        }
    }
    else
    {
        QString languageName = language;
        QStringList langList = languageName.split("_");
        if (langList.length() >= 1)
        {
            if (QFile::exists(":/tr/sm_" + langList.at(0) + ".qm"))
            {
                appTranslator.load(":/tr/sm_" + langList.at(0) + ".qm");
            }
        }
    }
    if (trsf)
    {
        QString languageName = QLocale::system().name();
        QStringList langList = languageName.split("_");
        if (langList.length() >= 1)
        {
            if (QFile::exists(":/tr/sm_" + langList.at(0) + ".qm"))
            {
                appTranslator.load(":/tr/sm_" + langList.at(0) + ".qm");
            }
        }
    }
    a.installTranslator(&appTranslator);
#ifdef QT4
    QTranslator qtTranslator;
    if (language == "System" || language.trimmed() == "")
    {
        QString languageName = QLocale::system().name();
        QStringList langList = languageName.split("_");
        if (langList.length() >= 1)
        {
            if (QFile::exists(":/tr/qt_" + langList.at(0) + ".qm"))
            {
                qtTranslator.load(":/tr/qt_" + langList.at(0) + ".qm");
            }
        }
    }
    else
    {
        QString languageName = language;
        QStringList langList = languageName.split("_");
        if (langList.length() >= 1)
        {
            if (QFile::exists(":/tr/qt_" + langList.at(0) + ".qm"))
            {
                qtTranslator.load(":/tr/qt_" + langList.at(0) + ".qm");
            }
        }
    }
    if (trsf)
    {
        QString languageName = QLocale::system().name();
        QStringList langList = languageName.split("_");
        if (langList.length() >= 1)
        {
            if (QFile::exists(":/tr/qt_" + langList.at(0) + ".qm"))
            {
                qtTranslator.load(":/tr/qt_" + langList.at(0) + ".qm");
            }
        }
    }
    a.installTranslator(&qtTranslator);
#else
#ifdef QT5
    QTranslator qtTranslator1;
    if (language == "System" || language.trimmed() == "")
    {
        QString languageName = QLocale::system().name();
        QStringList langList = languageName.split("_");
        if (langList.length() >= 1)
        {
            if (QFile::exists(":/tr/qtbase_" + langList.at(0) + ".qm"))
            {
                qtTranslator1.load(":/tr/qtbase_" + langList.at(0) + ".qm");
            }
        }
    }
    else
    {
        QString languageName = language;
        QStringList langList = languageName.split("_");
        if (langList.length() >= 1)
        {
            if (QFile::exists(":/tr/qtbase_" + langList.at(0) + ".qm"))
            {
                qtTranslator1.load(":/tr/qtbase_" + langList.at(0) + ".qm");
            }
        }
    }
    if (trsf)
    {
        QString languageName = QLocale::system().name();
        QStringList langList = languageName.split("_");
        if (langList.length() >= 1)
        {
            if (QFile::exists(":/tr/qtbase_" + langList.at(0) + ".qm"))
            {
                qtTranslator1.load(":/tr/qtbase_" + langList.at(0) + ".qm");
            }
        }
    }
    a.installTranslator(&qtTranslator1);
#endif
#endif
    // End internal translate loading

    // Start loading style
    settings.beginGroup("Style");
    bool loadCustom = settings.value("Custom",false).toBool();
    if (loadCustom)
    {
        QString style = settings.value("Name","System").toString();
        if (style != "System")
        {
            if (style.trimmed() != "")
            {
                a.setStyle(style);
            }
        }
    }
    settings.endGroup();
    // End loading style

    frmServerManager w(trplugspath, designedMode);
    w.show();

    return a.exec();
}
