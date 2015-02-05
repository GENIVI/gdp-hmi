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
# 05.Feb.2015, Holger Behrens, written

dist_noinst_DATA = genivi-demo-platform-hmi.doxygen.in

scanned_src_files = \
    $(top_srcdir)/src/gdp-hmi-controller.c

html: $(scanned_src_files) genivi-demo-platform-hmi.doxygen
	$(AM_V_GEN)$(MKDIR_P) html && \
	(cat genivi-demo-platform-hmi.doxygen; \
		echo "GENERATE_HTML=YES"; \
        echo "HTML_OUTPUT=html"; \
        echo "INPUT= $(scanned_src_files)"; \
        ) | doxygen -
