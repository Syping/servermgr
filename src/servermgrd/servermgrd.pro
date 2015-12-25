#/*****************************************************************************
#* servermgrd Syping Gaming Team Server Manager Server
#* Copyright (C) 2015-2016 Syping Gaming Team
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

QT       += core network gui widgets

isEqual(QT_MAJOR_VERSION, 5): DEFINES += "QT5"

CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

TARGET = servermgrd

SOURCES += main.cpp \
    core.cpp \
    http.cpp \
    server.cpp

HEADERS  += core.h \
    http.h \
    server.h

INCLUDEPATH += ../servermgrfw ../

# UNIX EXTRAS
unix: !macx: DEFINES += SM_UNIX
unix: !macx: target.path = $$(SM_INSTALL_PATH)/bin
unix: !macx: INSTALLS += target

# SERVER MANAGER LIBS
win32: LIBS += -L$$OUT_PWD/../servermgrfw/release -L$$OUT_PWD/../servermgrfw/debug -lservermgrfw
unix: LIBS += -L$$OUT_PWD/../servermgrfw -lservermgrfw

CONFIG(DisableSSL): DEFINES += DISABLE_SSL
