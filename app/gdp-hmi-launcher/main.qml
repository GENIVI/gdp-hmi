/**
 * SPDX license identifier: MPL-2.0
 *
 * Copyright (C) 2015 GENIVI Alliance
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
 */

import QtQuick 2.3
import QtQuick.Controls 1.2

Rectangle {
    id: gdp_launcher
    visible: true
    width: 1024
    height: 768
    color: "steelblue"
    opacity: 1.0

    signal appSelectSignal(string unit)
    signal requestOffSignal()

    Rectangle {
        id: grid_rectangle
        visible: true
        anchors.bottom: panel.top
        anchors.bottomMargin: 0
        anchors.right: parent.right
        anchors.rightMargin: 0
        anchors.left: parent.left
        anchors.leftMargin: 0
        anchors.top: parent.top
        anchors.topMargin: 0

        // handle clicks on empty area
        MouseArea {
            anchors.fill: parent
            onClicked: grid.currentIndex = -1
        }

        // grid with a list (model) of runable applications
        GridView {
            id: grid
            width: 600
            height: 450
            flickableDirection: Flickable.AutoFlickDirection
            keyNavigationWraps: true
            cellHeight: 225
            cellWidth: 200

            anchors.horizontalCenterOffset: 0
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: parent.top
            anchors.topMargin: 30

            Component {
                id: applicationDelegate
                Item {
                    width: grid.cellWidth; height: grid.cellHeight
                    Column {
                        anchors.fill: parent
                        Image {
                            source: icon;
                            anchors.horizontalCenter: parent.horizontalCenter;
                            width: 200;
                            height: 200;
                        }
                        Text {
                            text: name;
                            anchors.horizontalCenter: parent.horizontalCenter
                        }
                    } // Column
                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            grid.currentIndex = index
                            //console.log("grid clicked", index, " ", model.unit)
                            gdp_launcher.appSelectSignal(model.unit)
                        }
                    }
                } // Item
            } // Component

            model: ApplicationsModel {}
            delegate: applicationDelegate
            highlight: Rectangle { color: "lightsteelblue"; radius: 5 }
            focus: true

            // unselect any item
            Component.onCompleted: currentIndex = -1
        } // GridView - grid
    } // Rectangle - grid_rectangle

    // panel with power-off button
    Image {
        id: panel
        x: 0
        y: 686
        height: 68
        opacity: 1.0
        anchors.right: parent.right
        anchors.rightMargin: 0
        anchors.left: parent.left
        anchors.leftMargin: 0
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 0
        source: "file://usr/share/gdp/GDP_Panel.png"

        ToolButton {
            id: powerButton
            width: 60
            height: 60
            anchors.left: parent.left
            anchors.leftMargin: 10
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 4
            onClicked: {
                console.log("powerButton clicked")
                gdp_launcher.requestOffSignal()
            }
            Image {
                anchors.rightMargin: 5
                anchors.leftMargin: 5
                anchors.bottomMargin: 5
                anchors.topMargin: 5
                anchors.fill: parent
                source: "file://usr/share/gdp/power-icon_red.png"
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
    } // Image - panel
} // Rectangle - gdp_launcher
