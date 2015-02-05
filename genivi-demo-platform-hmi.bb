# Copyright (C) 2015 GENIVI Alliance
# Released under the MIT license (see COPYING.MIT for the terms)

SUMMARY = "Simple HMI for the GENIVI Demo Platform (GDP)"
HOMEPAGE = "http://projects.genivi.org/genivi-demo-platform/"
LICENSE = "MPL-2.0"
LIC_FILES_CHKSUM = "file://LICENSE;md5=815ca599c9df247a0c7f619bab123dad"

DEPENDS = "wayland-ivi-extension weston"

# SRC_URI = "git://git.projects.genivi.org/genivi-demo-platform-hmi.git"
# SRCREV = ""
# S = "${WORKDIR}/git"

inherit autotools pkgconfig
inherit externalsrc
EXTERNALSRC = "/home/holger/Projects/GDP/stash.wrs.com/genivi-demo-platform-hmi"

# make sure this matches the version given in configure.ac
PV = "0.2"

# by default the plugin should end up as:
#    /usr/lib/weston/genivi-demo-platform-hmi.so
# to load this plugin you need to add this (.so) to your weston.ini ivi-module
FILES_${PN} += "\
    ${datadir}/weston/* \
    ${libdir}/weston/genivi-demo-platform-hmi.so \
    "
FILES_${PN}-dev += "\
    ${libdir}/weston/genivi-demo-platform-hmi.la \
    "
FILES_${PN}-dbg += "\
    ${libdir}/weston/.debug/* \
    "

do_install_append() {
	install -d ${D}${datadir}/weston
	install -m 0444 ${S}/assets/GDP_Panel.png ${D}${datadir}/weston/GDP_Panel.png
}
