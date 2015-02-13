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
# 06.Feb.2015, Holger Behrens, created using QtCreator
# 09.Feb.2015, Holger Behrens, added gdp-hmi-launcher sources and headers
# 10.Feb.2015, Holger Behrens, added support for dbus

TEMPLATE = app

QT += dbus qml quick widgets

DBUS_INTERFACES += \
	protocol/dbus/gdp-hmi-introspect.xml

HEADERS += \
	gdp-hmi-launcher.h

SOURCES += \
	main.cpp \
	gdp-hmi-launcher.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# enable C standard as published in 2011 as ISO/IEC 9899:2011 (known as C11)
CONFIG     += c++11

LIBS       += -lsystemd

target.path = /usr/bin

# Default rules for deployment.
include(deployment.pri)

DISTFILES +=
