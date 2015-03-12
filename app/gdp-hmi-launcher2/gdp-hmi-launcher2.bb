# Copyright (C) 2015 GENIVI Alliance
# Released under the MIT license (see COPYING.MIT for the terms)

SUMMARY  = "GENIVI Demo Platform HMI - Launcher2"
HOMEPAGE = "http://projects.genivi.org/genivi-demo-platform/"
LICENSE  = "MPL-2.0"
LIC_FILES_CHKSUM = "file://LICENSE;md5=815ca599c9df247a0c7f619bab123dad"

DEPENDS = "qtbase qtdeclarative gdp-hmi-panel"

inherit qmake5
inherit externalsrc
EXTERNALSRC = ""

PV = "1.0"

FILES_${PN} += "\
    ${datadir}/gdp/* \
    "

do_install_append() {
	install -d ${D}${datadir}/gdp
	install -m 0444 ${S}/content/images/hmi_icons_033115-1.png \
		${D}${datadir}/gdp/hmi_icons_033115-1.png
	install -m 0444 ${S}/content/images/hmi_icons_033115-2.png \
		${D}${datadir}/gdp/hmi_icons_033115-2.png
	install -m 0444 ${S}/content/images/hmi_icons_033115-3.png \
		${D}${datadir}/gdp/hmi_icons_033115-3.png
	install -m 0444 ${S}/content/images/hmi_icons_033115-4.png \
		${D}${datadir}/gdp/hmi_icons_033115-4.png
	install -m 0444 ${S}/content/images/hmi_icons_033115-5.png \
		${D}${datadir}/gdp/hmi_icons_033115-5.png
	install -m 0444 ${S}/content/images/hmi_icons_033115-6.png \
		${D}${datadir}/gdp/hmi_icons_033115-6.png

	install -m 0444 ${S}/content/images/hmi_icons_033115-1n.png \
		${D}${datadir}/gdp/hmi_icons_033115-1n.png
	install -m 0444 ${S}/content/images/hmi_icons_033115-2n.png \
		${D}${datadir}/gdp/hmi_icons_033115-2n.png
	install -m 0444 ${S}/content/images/hmi_icons_033115-3n.png \
		${D}${datadir}/gdp/hmi_icons_033115-3n.png
	install -m 0444 ${S}/content/images/hmi_icons_033115-4n.png \
		${D}${datadir}/gdp/hmi_icons_033115-4n.png
	install -m 0444 ${S}/content/images/hmi_icons_033115-5n.png \
		${D}${datadir}/gdp/hmi_icons_033115-5n.png
	install -m 0444 ${S}/content/images/hmi_icons_033115-6n.png \
		${D}${datadir}/gdp/hmi_icons_033115-6n.png

	install -m 0444 ${S}/content/images/hmi_icons_033115-1s.png \
		${D}${datadir}/gdp/hmi_icons_033115-1s.png
	install -m 0444 ${S}/content/images/hmi_icons_033115-2s.png \
		${D}${datadir}/gdp/hmi_icons_033115-2s.png
	install -m 0444 ${S}/content/images/hmi_icons_033115-3s.png \
		${D}${datadir}/gdp/hmi_icons_033115-3s.png
	install -m 0444 ${S}/content/images/hmi_icons_033115-4s.png \
		${D}${datadir}/gdp/hmi_icons_033115-4s.png
	install -m 0444 ${S}/content/images/hmi_icons_033115-5s.png \
		${D}${datadir}/gdp/hmi_icons_033115-5s.png
	install -m 0444 ${S}/content/images/hmi_icons_033115-6s.png \
		${D}${datadir}/gdp/hmi_icons_033115-6s.png

	install -m 0444 ${S}/content/images/arrow-right.png \
		${D}${datadir}/gdp/arrow-right.png
	install -m 0444 ${S}/content/images/background_lab.jpg \
		${D}${datadir}/gdp/background_lab.jpg
	install -m 0444 ${S}/content/images/spot.png \
		${D}${datadir}/gdp/spot.png
	install -m 0444 ${S}/content/images/powerOff.png \
		${D}${datadir}/gdp/powerOff.png
}
