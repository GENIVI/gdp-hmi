# Copyright (C) 2015 GENIVI Alliance
# Released under the MIT license (see COPYING.MIT for the terms)

SUMMARY = "Simple QML Application"
HOMEPAGE = "http://projects.genivi.org/genivi-demo-platform/"
LICENSE = "MPL-2.0"
LIC_FILES_CHKSUM = "file://LICENSE;md5=815ca599c9df247a0c7f619bab123dad"

DEPENDS = "qtbase qtdeclarative"

inherit qmake5
inherit externalsrc
EXTERNALSRC = "/home/holger/Projects/GDP/stash.wrs.com/genivi-demo-platform-hmi/app/qml-example"

PV = "1.0"

FILES_${PN} += "\
    "
FILES_${PN}-dev += "\
    "
FILES_${PN}-dbg += "\
    "
