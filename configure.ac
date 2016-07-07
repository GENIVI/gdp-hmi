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
# 06.Feb.2015, Holger Behrens, added support for C++ source
#                              updated CONTROL flag
# 09.Feb.2015, Holger Behrens, add support for dbus/dbus-c++/glib

AC_PREREQ([2.69])
AC_INIT([genivi-demo-platform-hmi], [0.4],
        [http://bugs.genivi.org/], [genivi-demo-platform-hmi],
        [http://projects.genivi.org/genivi-demo-platform/])

AM_INIT_AUTOMAKE([1.11 foreign no-dist-gzip dist-xz])
AC_CONFIG_MACRO_DIR([m4])
AM_SILENT_RULES([yes])

# Check for programs
AC_PROG_CC
AC_PROG_CXX

# Initialize libtool
LT_PREREQ([2.4])
LT_INIT([disable-static])

PKG_PROG_PKG_CONFIG()

PKG_CHECK_MODULES([DBUS], [
	dbus-1
	dbus-c++-1
	dbus-c++-glib-1
	])
PKG_CHECK_MODULES([CONTROL], [
	wayland-client >= 1.5.0
	weston >= 1.5.0
	libsystemd
	glib-2.0])
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
AC_CHECK_LIB(ilmInput, ilm_setInputAcceptanceOn, [],
	[AC_MSG_ERROR([Can't find correct ilmInput library.])])
AC_CHECK_LIB(ilmInput, ilm_setInputFocus, [],
	[AC_MSG_ERROR([Can't find correct ilmInput library.])])
ILM_LIBS="-lilmCommon -lilmControl -lilmInput"
AC_SUBST(ILM_LIBS)

WAYLAND_PREFIX=`$PKG_CONFIG --variable=prefix wayland-client`
AC_PATH_PROG([WAYLAND_SCANNER], [wayland-scanner],,
	[${WAYLAND_PREFIX}/bin$PATH_SEPARATOR$PATH])

# check for dbusxx-xml2cpp
AC_PATH_PROG([XML2CPP], [dbusxx-xml2cpp])

# handle arguments
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
AC_CONFIG_FILES([protocol/dbus/Makefile])
AC_CONFIG_FILES([src/Makefile])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_SRCDIR(src/gdp-hmi-controller.cpp)

# enable C standard as published in 2011 as ISO/IEC 9899:2011 (known as C11)
CFLAGS="$CFLAGS -std=c11"

AC_OUTPUT
