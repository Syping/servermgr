#/*****************************************************************************
#* servermgrfw Syping Gaming Team Server Manager Framework
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

QT       += core gui network widgets

isEqual(QT_MAJOR_VERSION, 5): DEFINES += "QT5"

TARGET = servermgrfw
TEMPLATE = lib
CONFIG += staticlib

SOURCES += ServerManager.cpp \
    PixmapEdit.cpp

HEADERS += ServerManager.h \
    PixmapEdit.h

INCLUDEPATH += ../
