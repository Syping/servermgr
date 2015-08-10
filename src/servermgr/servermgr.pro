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

DEPLOYMENT.display_name = "Server Manager"

TEMPLATE = app

SOURCES += main.cpp \
    ConnectThread.cpp \
    IconThread.cpp \
    KPTLabel.cpp \
    frmConfig.cpp \
    frmIcon.cpp \
    frmServerManager.cpp

HEADERS  += ConnectThread.h \
    IconThread.h \
    KPTLabel.h \
    frmConfig.h \
    frmIcon.h \
    frmServerManager.h
    SMStyleTweaks.h

FORMS    += \
    frmConfig.ui \
    frmIcon.ui \
    frmServerManager.ui \

RESOURCES += \
    res/res.qrc

TRANSLATIONS += \
    res/sm_de.ts

INCLUDEPATH += ../servermgrfw ../

OTHER_FILES += \
    app.rc \
    sm.ico

# UNIX EXTRAS
unix: !macx: TARGET = servermgr
unix: !macx: DEFINES += SM_UNIX
unix: !macx: target.path = $$(SM_INSTALL_PATH)/bin
unix: !macx: appfiles.path = $$(SM_INSTALL_PATH)/share/applications
unix: !macx: appfiles.files = $$PWD/res/install/servermgr.desktop
unix: !macx: pixfiles.path = $$(SM_INSTALL_PATH)/share/pixmaps
unix: !macx: pixfiles.files = $$PWD/res/install/servermgr.xpm
unix: !macx: INSTALLS += target appfiles pixfiles

# ANDROID EXTRAS
android: CONFIG += mobility
android: DEFINES += SM_ANDROID
android: TARGET = ServerManager
ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android-src

# SERVER MANAGER LIBS
win32: LIBS += -L$$OUT_PWD/../servermgrfw/release -L$$OUT_PWD/../servermgrfw/debug -lservermgrfw
unix: LIBS += -L$$OUT_PWD/../servermgrfw -lservermgrfw

DISTFILES += \
    android-src/AndroidManifest.xml \
    android-src/gradle/wrapper/gradle-wrapper.jar \
    android-src/gradlew \
    android-src/res/values/libs.xml \
    android-src/build.gradle \
    android-src/gradle/wrapper/gradle-wrapper.properties \
    android-src/gradlew.bat

CONFIG(DisableSSL): DEFINES += DISABLE_SSL
