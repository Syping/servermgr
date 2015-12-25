#/*****************************************************************************
#* servermgr Syping Gaming Team Server Manager
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

lessThan(QT_MAJOR_VERSION, 5): error("servermgr requires Qt 5.3 or higher")
isEqual(QT_MAJOR_VERSION, 5): lessThan(QT_MINOR_VERSION, 3): error("servermgr requires Qt 5.3 or higher")

TEMPLATE = subdirs
CONFIG += ordered

SUBDIRS = src

OTHER_FILES += configure \
    LICENSE \
    README.md \
    sm_version
