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
 * 16.Mar.2015, Sebastien Baudouin, written based on template created by QtCreator
 *
 */
import QtQuick 2.0

Item {
    id: root

    width: 96
    height: 108

    Image {
        id: powerOffImage
        x: -78
        y: -82
        source: "file://usr/share/gdp/powerOff.png"
        scale:0.3
        MouseArea {
            anchors.fill: parent
            onClicked: {
                    mainView.requestOffSignal();
            }
        }

    }

    Text {
        x: 10
        height: 16
        anchors.right: parent.right
        anchors.verticalCenter: powerOffImage.verticalCenter
        color: "#ffffff"
        style: Text.Outline
        styleColor: "#606060"
        font.pixelSize: 19
        text: "PowerOff"
        anchors.verticalCenterOffset: 46
        anchors.rightMargin: 8
    }
}
