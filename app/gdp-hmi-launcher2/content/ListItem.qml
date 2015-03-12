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
 * 16.Mar.2015, Sebastien Baudouin, modify the file to start the application
 *                                  associated to the selected item
 *
 */
import QtQuick 2.0

Item {
    id: root
    width: 256
    height: 256

    property string name: model.name
    property bool isSelected: pathView.currentIndex === model.index
    property real fogAmount: PathView.iconFog

    z: PathView.iconZ
    scale: PathView.iconScale

    Image {
        id: shadowSource
        source: model.shadowIcon
        width: nmapItem.width
        height: nmapItem.height
        anchors.centerIn: nmapItem
        anchors.horizontalCenterOffset: ((root.x + root.width/2) - (lightSourceItem.width/2)) * 0.05
        anchors.verticalCenterOffset: ((root.y + root.height/2) - (lightSourceItem.height/2)) * 0.05 + (settings.spotAnimationPosition - 25) * (0.8 - fogAmount)
        opacity: 0.2 + (fogAmount*0.8)
        scale: 1.2 - (fogAmount*0.4)
    }

    NMapEffect {
        id: nmapItem
        sourceImage: model.sourceIcon
        normalsImage: model.normalsIcon
        lightSource: lightSourceItem
        switchX: true
        switchY: true
        elementPositionX: root.x
        elementPositionY: root.y
        colorizeAmount: fogAmount
        diffuseBoost: 0.5
    }

    MouseArea {
        anchors.fill: parent
        onClicked: {
            if (isSelected) {
                // Open the item
                mainView.appSelectSignal(model.unit)
            } else {
                pathView.currentIndex = model.index;
            }
        }
    }
}
