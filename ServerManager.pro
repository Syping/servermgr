#/*****************************************************************************
#* servermgr Syping Gaming Team Server Manager
#* Copyright (C) 2015 Syping Gaming Team
#*
#* Licensed under the Apache License, Version 2.0 (the "License");
#* you may not use this file except in compliance with the License.
#* You may obtain a copy of the License at
#*
#*     http://www.apache.org/licenses/LICENSE-2.0
#*
#* Unless required by applicable law or agreed to in writing, software
#* distributed under the License is distributed on an "AS IS" BASIS,
#* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#* See the License for the specific language governing permissions and
#* limitations under the License.
#*****************************************************************************/

QT       += core gui network
isEqual(QT_MAJOR_VERSION, 5): QT += widgets

isEqual(QT_MAJOR_VERSION, 4): DEFINES += "QT4"
isEqual(QT_MAJOR_VERSION, 5): DEFINES += "QT5"

win32: RC_FILE = app.rc

TARGET = ServerManager
TEMPLATE = app

SOURCES += main.cpp \
    ConnectThread.cpp \
    IconThread.cpp \
    KPTLabel.cpp \
    frmConfig.cpp \
    frmIcon.cpp \
    frmServerManager.cpp \
    PixmapEdit.cpp \
    ServerManager.cpp

HEADERS  += config.h \
    ConnectThread.h \
    IconThread.h \
    KPTLabel.h \
    frmConfig.h \
    frmIcon.h \
    frmServerManager.h \
    PixmapEdit.h \
    ServerManager.h \
    SMStyleTweaks.h

FORMS    += \
    frmConfig.ui \
    frmIcon.ui \
    frmServerManager.ui \

RESOURCES += \
    res/res.qrc

TRANSLATIONS += \
    res/sm_de.ts

OTHER_FILES += \
    app.rc \
    sm.ico

# UNIX EXTRAS
unix: !macx: DEFINES += SM_UNIX
unix: !macx: target.path = $$(SM_INSTALL_PATH)/bin
unix: !macx: INSTALLS += target

CONFIG(DisableSSL): DEFINES += DISABLE_SSL
