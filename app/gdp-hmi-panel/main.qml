/**
 * SPDX license identifier: MPL-2.0
 *
 * Copyright (C) 2016 GENIVI Alliance
 *
 * This file is part of GENIVI Development Platform HMI.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License (MPL), v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * For further information see http://www.genivi.org/.
 *
 * List of changes:
 * 06.Feb.2015, Holger Behrens, written based on template created by QtCreator
 * 16.Feb.2015, Holger Behrens, cleanup signal and image handling
 */

import QtQuick 2.3
import QtQuick.Controls 1.2

Rectangle {
    id: gdp_panel
    visible: true
    width: 1024
    height: 68

    signal homeSignal()

    Image {
        id: background_image
        anchors.fill: parent
        source: "file:///usr/share/gdp/GDP_Panel.png"
    }

    ToolButton {
        id: home_button
        width: 50
        height: 50
        visible: true
        anchors.left: parent.left
        anchors.leftMargin: 14
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 0
        anchors.top: parent.top
        anchors.topMargin: 0
        onClicked: gdp_panel.homeSignal()
        Image {
            id: home_image
            fillMode: Image.PreserveAspectFit
            anchors.fill: parent
            source: "file:///usr/share/gdp/GDP_Quadrat.png"
        }
    }

    Text {
        id: notice
        color: "#ffffff"
        style: Text.Normal
        font.pixelSize: 12
        text: qsTr("Copyright © 2016, GENIVI Alliance")
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 8
        anchors.right: parent.right
        anchors.rightMargin: 8
    }
}
