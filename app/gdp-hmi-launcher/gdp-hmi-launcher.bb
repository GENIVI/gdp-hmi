# Copyright (C) 2015 GENIVI Alliance
# Released under the MIT license (see COPYING.MIT for the terms)

SUMMARY  = "GENIVI Demo Platform HMI - Launcher"
HOMEPAGE = "http://projects.genivi.org/genivi-demo-platform/"
LICENSE  = "MPL-2.0"
LIC_FILES_CHKSUM = "file://LICENSE;md5=815ca599c9df247a0c7f619bab123dad"

DEPENDS = "qtbase qtdeclarative gdp-hmi-panel"

inherit qmake5
inherit externalsrc
EXTERNALSRC = "/home/holger/Projects/GDP/stash.wrs.com/genivi-demo-platform-hmi/app/gdp-hmi-launcher"

PV = "1.0"

FILES_${PN} += "\
    ${datadir}/gdp/* \
    "

do_install_append() {
	install -d ${D}${datadir}/gdp
	install -m 0444 ${S}/assets/car-purple-icon.png \
		${D}${datadir}/gdp/car-purple-icon.png
	install -m 0444 ${S}/assets/power-icon_red.png \
		${D}${datadir}/gdp/power-icon_red.png
	install -m 0444 ${S}/assets/power-icon_white.png \
		${D}${datadir}/gdp/power-icon_white.png
}
