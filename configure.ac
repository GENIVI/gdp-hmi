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
# 05.Feb.2015, Holger Behrens, added support for 'doc' making

AC_PREREQ([2.69])
AC_INIT([genivi-demo-platform-hmi], [0.2],
        [http://bugs.genivi.org/], [genivi-demo-platform-hmi],
        [http://projects.genivi.org/genivi-demo-platform/])

AM_INIT_AUTOMAKE([1.11 foreign no-dist-gzip dist-xz])

AM_SILENT_RULES([yes])

# Check for programs
AC_PROG_CC

# Initialize libtool
LT_PREREQ([2.4])
LT_INIT([disable-static])

PKG_PROG_PKG_CONFIG()

PKG_CHECK_MODULES([CONTROL], [
	weston >= 1.5.0
	libsystemd])
PKG_CHECK_MODULES([PLUGIN], [weston >= 1.5.0 cairo xkbcommon])
PKG_CHECK_MODULES([CLIENT], [
	wayland-client >= 1.5.0
	wayland-cursor
	cairo >= 1.10.0
	xkbcommon
	pixman-1
	libpng
	libsystemd
	])

# Check for wayland-ivi-extension
AC_CHECK_HEADERS([ilm/ilm_types.h])
AC_CHECK_LIB(ilmClient, ilm_UpdateInputEventAcceptanceOn,
	[ILM_LIBS="-lilmCommon -lilmClient"],
	[AC_MSG_ERROR([Can't find correct ilmClient library.])])
AC_CHECK_LIB(ilmControl, ilm_SetKeyboardFocusOn,
	[ILM_LIBS="$ILM_LIBS -lilmControl"],
	[AC_MSG_ERROR([Can't find correct ilmControl library.])])
AC_SUBST(ILM_LIBS)

WAYLAND_PREFIX=`$PKG_CONFIG --variable=prefix wayland-client`
AC_PATH_PROG([WAYLAND_SCANNER], [wayland-scanner],,
	[${WAYLAND_PREFIX}/bin$PATH_SEPARATOR$PATH])

AM_CONDITIONAL(BUILD_DOCS, [test x$enable_documentation = xyes])
if test "x$enable_documentation" = "xyes"; then
	AC_PATH_PROG(DOXYGEN, doxygen)

	if test "x$DOXYGEN" = "x"; then
		AC_MSG_ERROR([Documentation build requested but doxygen not found. Install doxygen or disable the documentation using --disable-documentation])
	fi

	AC_CONFIG_FILES([doc/doxygen/genivi-demo-platform-hmi.doxygen])
fi

AC_CONFIG_FILES([Makefile])
AC_CONFIG_FILES([doc/Makefile])
AC_CONFIG_FILES([doc/doxygen/Makefile])
AC_CONFIG_FILES([protocol/Makefile])
AC_CONFIG_FILES([src/Makefile])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_SRCDIR(src/gdp-hmi-controller.c)

# enable C standard as published in 2011 as ISO/IEC 9899:2011 (known as C11)
CFLAGS="$CFLAGS -std=c11"

AC_OUTPUT
