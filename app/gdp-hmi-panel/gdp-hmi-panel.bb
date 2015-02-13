# Copyright (C) 2015 GENIVI Alliance
# Released under the MIT license (see COPYING.MIT for the terms)

SUMMARY  = "GENIVI Demo Platform HMI - Panel"
HOMEPAGE = "http://projects.genivi.org/genivi-demo-platform/"
LICENSE  = "MPL-2.0"
LIC_FILES_CHKSUM = "file://LICENSE;md5=815ca599c9df247a0c7f619bab123dad"

DEPENDS = "qtbase qtdeclarative"

inherit qmake5
inherit externalsrc
EXTERNALSRC = "/home/holger/Projects/GDP/stash.wrs.com/genivi-demo-platform-hmi/app/gdp-hmi-panel"

PV = "1.0"

FILES_${PN} += "\
    ${datadir}/gdp/* \
    "

do_install_append() {
	install -d ${D}${datadir}/gdp
	install -m 0444 ${S}/assets/GDP_Square.png \
					${D}${datadir}/gdp/GDP_Square.png
	install -m 0444 ${S}/assets/GDP_Hamburger_white.png \
	                ${D}${datadir}/gdp/GDP_Hamburger_white.png
	install -m 0444 ${S}/assets/GDP_Panel.png ${D}${datadir}/gdp/GDP_Panel.png
}
