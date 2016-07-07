# SPDX license identifier: MPL-2.0
#
# Copyright (C) 2015, GENIVI Alliance
#
# This file is part of the GENIVI Demo Platform HMI.
#
# This Source Code Form is subject to the terms of the Mozilla Public
# License (MPL), v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.
#
# For further information see http://www.genivi.org/.
#
# List of changes:
# 23.Jan.2015, Holger Behrens, written
# 06.Feb.2015, Holger Behrens, define include directory location
# 09.Feb.2015, Holger Behrens, add support for dbus/dbus-c++/glib
# 10.Feb.2015, Hogler Behrens, add support for systemd1/login1 dbus-c++ client
#

AM_CPPFLAGS   = -I$(top_srcdir)/include
BUILT_SOURCES = ivi-controller-client-protocol.h \
                gdp-hmi-server-glue.h  \
                gdp-hmi-client-glue.h  \
                systemd1-client-glue.h \
                login1-client-glue.h

bin_PROGRAMS = gdp-hmi-controller
gdp_hmi_controller_CPPFLAGS = $(CONTROL_CFLAGS) $(DBUS_CFLAGS) $(AM_CPPFLAGS)
gdp_hmi_controller_LDADD = $(ILM_LIBS) $(CONTROL_LIBS) $(DBUS_LIBS)
gdp_hmi_controller_SOURCES = \
    gdp-dbus-systemd.cpp \
    systemd1-client-glue.h \
    gdp-dbus-service.cpp \
    gdp-hmi-server-glue.h \
    ivi-controller-client-protocol.h \
    gdp-hmi-controller.cpp

%-protocol.c : ../protocol/%.xml
	$(AM_V_GEN)$(WAYLAND_SCANNER) code < $< > $@

%-server-protocol.h : ../protocol/%.xml
	$(AM_V_GEN)$(WAYLAND_SCANNER) server-header < $< > $@

%-client-protocol.h : ../protocol/%.xml
	$(AM_V_GEN)$(WAYLAND_SCANNER) client-header < $< > $@

gdp-hmi-client-glue.h: $(top_srcdir)/protocol/dbus/gdp-hmi-introspect.xml
	$(XML2CPP) $< --proxy=$@

gdp-hmi-server-glue.h: $(top_srcdir)/protocol/dbus/gdp-hmi-introspect.xml
	$(XML2CPP) $< --adaptor=$@

systemd1-client-glue.h: $(top_srcdir)/protocol/dbus/systemd-systemd1.xml
	$(XML2CPP) $< --proxy=$@

login1-client-glue.h: $(top_srcdir)/protocol/dbus/systemd-login1.xml
	$(XML2CPP) $< --proxy=$@
