/**
 * Source codes are licensed under a Creative Commons Attribution 3.0 Unported
 * License. http://creativecommons.org/licenses/by/3.0/
 *
 * SPDX license identifier: CC-BY-3.0
 *
 * Initial code writters can be contacted at: info@quitcoding.com
 *
 * Copyright (C) 2015 GENIVI Alliance
 *
 * List of changes:
 * 16.Mar.2015, Sebastien Baudouin, modify the file to take in account the path
 *                                  where icons are stored and add unit field for
 *                                  launching application
 *
 */
import QtQuick 2.0

ListModel {
    ListElement {
        name: "Fuel Stop Advisor"
        sourceIcon: "file://usr/share/gdp/hmi_icons_033115-1.png"
        normalsIcon: "file://usr/share/gdp/hmi_icons_033115-1n.png"
        shadowIcon: "file://usr/share/gdp/hmi_icons_033115-1s.png"
        unit: "fsa.service"
    }
    ListElement {
        name: "Navigation"
        sourceIcon: "file://usr/share/gdp/hmi_icons_033115-2.png"
        normalsIcon: "file://usr/share/gdp/hmi_icons_033115-2n.png"
        shadowIcon: "file://usr/share/gdp/hmi_icons_033115-2s.png"
        unit: "EGLWLMockNavigation.service"
    }
    ListElement {
        name: "Audio Manager"
        sourceIcon: "file://usr/share/gdp/hmi_icons_033115-3.png"
        normalsIcon: "file://usr/share/gdp/hmi_icons_033115-3n.png"
        shadowIcon: "file://usr/share/gdp/hmi_icons_033115-3s.png"
        unit: "AudioManager_PoC.service"
    }
    ListElement {
        name: "Web Browsing"
        sourceIcon: "file://usr/share/gdp/hmi_icons_033115-4.png"
        normalsIcon: "file://usr/share/gdp/hmi_icons_033115-4n.png"
        shadowIcon: "file://usr/share/gdp/hmi_icons_033115-4s.png"
        unit: "demoui.service"
    }
    ListElement {
        name: "Application"
        sourceIcon: "file://usr/share/gdp/hmi_icons_033115-5.png"
        normalsIcon: "file://usr/share/gdp/hmi_icons_033115-5n.png"
        shadowIcon: "file://usr/share/gdp/hmi_icons_033115-5s.png"
        unit: "qml-example.service"
    }
    ListElement {
        name: "Media Player"
        sourceIcon: "file://usr/share/gdp/hmi_icons_033115-6.png"
        normalsIcon: "file://usr/share/gdp/hmi_icons_033115-6n.png"
        shadowIcon: "file://usr/share/gdp/hmi_icons_033115-6s.png"
        unit: "EGLWLInputEventExample.service"
    }
}
