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
 * 06.Feb.2015, Holger Behrens, written based on template created by QtCreator
 *
 */

import QtQuick 2.3
import QtQuick.Controls 1.2

Rectangle {
    id: gdp_panel
    visible: true
    width: 1024
    height: 68

    signal qmlSignal(string msg)

    Image {
        id: background_image
        anchors.fill: parent
        source: "file:///usr/share/gdp/GDP_Panel.png"
    }

    ToolButton {
        id: home_button
        width: 50
        height: 50
        anchors.left: parent.left
        anchors.leftMargin: 10
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 0
        anchors.top: parent.top
        anchors.topMargin: 0
        iconName: "HomeIcon"
        iconSource: "file:///usr/share/gdp/GDP_Square.png"
        onClicked: {
            console.log("homeButton clicked")
            gdp_panel.qmlSignal("homeButton")
        }
    }

    Text {
        id: notice
        color: "#ffffff"
        style: Text.Normal
        font.pixelSize: 12
        text: qsTr("Copyright © 2015, GENIVI Alliance")
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 8
        anchors.right: parent.right
        anchors.rightMargin: 8
    }
}

