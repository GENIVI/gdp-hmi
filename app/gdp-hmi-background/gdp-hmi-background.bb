# Copyright (C) 2015 GENIVI Alliance
# Released under the MIT license (see COPYING.MIT for the terms)

SUMMARY  = "GENIVI Demo Platform Background Image"
HOMEPAGE = "http://projects.genivi.org/genivi-demo-platform/"
LICENSE  = "MPL-2.0"
LIC_FILES_CHKSUM = "file://LICENSE;md5=815ca599c9df247a0c7f619bab123dad"

DEPENDS = "qtbase qtdeclarative"

inherit qmake5
inherit externalsrc
EXTERNALSRC = "/home/holger/Projects/GDP/stash.wrs.com/genivi-demo-platform-hmi/app/gdp-hmi-background"

PV = "1.0"

FILES_${PN} += "\
    ${datadir}/gdp/* \
    "

do_install_append() {
	install -d ${D}${datadir}/gdp
	install -m 0444 ${S}/assets/GDP_Background.png \
	                ${D}${datadir}/gdp/GDP_Background.png
}
