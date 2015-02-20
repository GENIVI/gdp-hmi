/**
 * SPDX license identifier: MPL-2.0
 *
 * Copyright (C) 2015 GENIVI Alliance
 *
 * This file is part of GENIVI Demo Platform HMI.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License (MPL), v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * For further information see http://www.genivi.org/.
 *
 * List of changes:
 * 06.Feb.2015, Holger Behrens, written
 */

import QtQuick 2.3

ListModel {
    ListElement {
        name: "Audio Manager Demo"
        icon: "file://usr/share/gdp/GDP_AM_Button.png"
        unit: "AudioManager_PoC.service"
        description: "Audio Manager Demo Description (TBD)"
    }
    ListElement {
        name: "Browser PoC"
        icon: "file://usr/share/gdp/GDP_Browser_Button.png"
        unit: "browser.service"
        description: "Browser PoC Description (TBD)"
    }
    ListElement {
        name: "Fuel Stop Adviser PoC"
        icon: "file://usr/share/gdp/1424451154_fuel-128.png"
        unit: "fsa.service"
        description: "Fuel Stop Adviser PoC Description (TBD)"
    }
    ListElement {
        name: "Navigation Mock Up"
        icon: "file://usr/share/gdp/maps-icon.png"
        unit: "EGLWLMockNavigation.service"
        description: "Navigation Mock Up Description (TBD)"
    }
    ListElement {
        name: "Input Example"
        icon: "file://usr/share/gdp/App-xeyes-icon.png"
        unit: "EGLWLInputEventExample.service"
        description: "Input Example Description (TBD)"
    }
    ListElement {
        name: "QML Example"
        icon: "file://usr/share/gdp/car-purple-icon.png"
        unit: "qml-example.service"
        description: "QML Example Description (TBD)"
    }
}
