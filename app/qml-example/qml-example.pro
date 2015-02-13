# SPDX license identifier: MPL-2.0
#
# Copyright (C) 2015, GENIVI Alliance
#
# This file is part of GENIVI Demo Platform HMI.
#
# This Source Code Form is subject to the terms of the Mozilla Public
# License (MPL), v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.
#
# For further information see http://www.genivi.org/.
#
# List of changes:
# 29.01.2015, Holger Behrens, created using QtCreator
#

TEMPLATE = app

QT += qml quick widgets

SOURCES   += main.cpp
HEADERS   += main.h
RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

LIBS      += -lsystemd

target.path = /usr/bin

# enable C standard as published in 2011 as ISO/IEC 9899:2011 (known as C11)
CONFIG     += c++11

# Default rules for deployment.
include(deployment.pri)
