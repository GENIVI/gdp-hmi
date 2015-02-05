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

BUILT_SOURCES = ivi-controller-client-protocol.h

bin_PROGRAMS = gdp-hmi-controller
gdp_hmi_controller_CPPFLAGS = $(CONTROL_FLAGS) $(AM_CPPFLAGS)
gdp_hmi_controller_LDADD = $(ILM_LIBS) $(CONTROL_LIBS)
gdp_hmi_controller_SOURCES = \
        ivi-controller-client-protocol.h \
        gdp-hmi-controller.c

%-protocol.c : ../protocol/%.xml
	$(AM_V_GEN)$(WAYLAND_SCANNER) code < $< > $@

%-server-protocol.h : ../protocol/%.xml
	$(AM_V_GEN)$(WAYLAND_SCANNER) server-header < $< > $@

%-client-protocol.h : ../protocol/%.xml
	$(AM_V_GEN)$(WAYLAND_SCANNER) client-header < $< > $@
