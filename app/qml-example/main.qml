/**
 * SPDX license identifier: MPL-2.0
 *
 * Copyright (C) 2015, GENIVI Alliance
 *
 * This file is part of GENIVI AudioManager PoC.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License (MPL), v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * For further information see http://www.genivi.org/.
 *
 * List of changes:
 * 29.01.2014, Holger Behrens, written based on template created by QtCreator
 */

import QtQuick 2.3
import QtQuick.Controls 1.2

Rectangle {
    id: window
    visible: true
    width: 600
    height: 480

    signal qmlSignal(string msg)

    Column {
        id: column1
        height: 200
        anchors.right: parent.right
        anchors.rightMargin: 0
        anchors.left: parent.left
        anchors.leftMargin: 0
        anchors.top: parent.top
        anchors.topMargin: 0

        Button {
            id: buttonOne
            text: qsTr("No")
            anchors.top: parent.top
            anchors.topMargin: 8
            anchors.right: parent.right
            anchors.rightMargin: 20
            onClicked: {
                console.log("buttonOne clicked");
                window.qmlSignal("Hello from QML (buttonOne)")
            }
        }
        Text {
            x: 0
            y: 0
            width: 120
            text: qsTr("Hello GENIVI World")
            anchors.left: parent.left
            anchors.leftMargin: 10
            anchors.top: parent.top
            anchors.topMargin: 8
            horizontalAlignment: Text.AlignLeft
        }
        TextInput {
            id: textInputOne
            height: 20
            text: qsTr("Text Input")
            horizontalAlignment: Text.AlignLeft
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.left
            anchors.leftMargin: 24
            anchors.right: parent.right
            anchors.rightMargin: 260
            font.pixelSize: 12
        }
        TextField {
            id: textFieldOne
            x: 0
            y: 0
            width: 120
            transformOrigin: Item.Center
            anchors.top: textInputOne.bottom
            anchors.topMargin: 8
            anchors.left: parent.left
            anchors.leftMargin: 24
            placeholderText: qsTr("Text Field")
        }
    }

    MouseArea {
        id: mouseArea
        y: 0
        height: 280
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 0
        anchors.right: parent.right
        anchors.left: parent.left
        anchors.rightMargin: 0
        anchors.leftMargin: 0
        onClicked: {
            console.log("mouseArea clicked - at", mouse.x, mouse.y)
        }
        onPressedChanged: {
            console.log("Mouse area is pressed?", pressed)
        }
    }
}
