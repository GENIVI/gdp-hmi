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
 * 16.Mar.2015, Sebastien Baudouin, modify the file to take in account our screen
 *                                  aspect ratio and the path where icons are
 *                                  stored and add a new item for powerOff
 *
 */
import QtQuick 2.0
import QtGraphicalEffects 1.0
import "content"

Item {
    id: mainView

    width: 1024
    height: 768

    signal appSelectSignal(string unit)
    signal requestOffSignal()

    QtObject {
        id: settings
        property real spotAnimationPosition: 0.0
        SequentialAnimation on spotAnimationPosition {
            loops: Animation.Infinite
            NumberAnimation { to: 30; duration: 2000; easing.type: Easing.InOutQuad }
            NumberAnimation { to: 0; duration: 2000; easing.type: Easing.InOutQuad }
        }
    }

    Image {
        anchors.fill: parent
        source: "file://usr/share/gdp/background_lab.jpg"
    }

    ContentModel {
        id: listModel
    }

    NMapLightSource {
        id: lightSourceItem
        z: 10
        lightPosX: mainView.width * 0.5
        lightPosY: mainView.height * 0.8 - settings.spotAnimationPosition * 5
        lightIntensity: 0.3
    }

    PathView {
        id: pathView
        anchors.fill: parent
        model: listModel
        delegate: ListItem {
            onIsSelectedChanged: {
                if (isSelected) {
                    nameTextItem.text = model.name
                }
            }
        }
        path: Path {
            startX: mainView.width*0.5; startY: mainView.height*0.68
            PathAttribute { name: "iconScale"; value: 0.8 }
            PathAttribute { name: "iconZ"; value: 100.0 }
            PathAttribute { name: "iconFog"; value: 0.0 }
            PathQuad { x: mainView.width*0.5; y: mainView.height*0.2; controlX: mainView.width*1.1; controlY: mainView.height*0.2 }
            PathAttribute { name: "iconScale"; value: 0.46 }
            PathAttribute { name: "iconZ"; value: 1.0 }
            PathAttribute { name: "iconFog"; value: 0.5 }
            PathQuad { x: mainView.width*0.5; y: mainView.height*0.68; controlX: -mainView.width*0.1; controlY: mainView.height*0.2 }
        }
        focus: true
        Keys.onPressed: {
            if (event.key === Qt.Key_Left) {
                pathView.decrementCurrentIndex();
                event.accepted = true;
            }
            if (event.key === Qt.Key_Right) {
                pathView.incrementCurrentIndex();
                event.accepted = true;
            }
        }
    }

    Text {
        id: nameTextItem
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.bottom
        anchors.bottomMargin: parent.height * 0.08
        font.pixelSize: 68
        color: "#d0d0d0"
        style: Text.Outline
        styleColor: "#404040"
        visible: false
    }

    DropShadow {
        anchors.fill: nameTextItem
        horizontalOffset: 0
        verticalOffset: settings.spotAnimationPosition * 0.5 - 8
        radius: 14.0
        samples: 16
        color: "#000000"
        source: nameTextItem
        opacity: 0.6
    }

    Image {
        anchors.bottom: parent.bottom
        anchors.bottomMargin: parent.height * 0.01
        anchors.right: parent.right
        anchors.rightMargin: Math.max(8, parent.width * 0.15 - 100)
        source: "file://usr/share/gdp/arrow-right.png"
        MouseArea {
            anchors.fill: parent
            onClicked: pathView.incrementCurrentIndex()
        }
    }

    Image {
        anchors.bottom: parent.bottom
        anchors.bottomMargin: parent.height * 0.01
        anchors.left: parent.left
        anchors.leftMargin: Math.max(8, parent.width * 0.15 - 100)
        mirror: true
        source: "file://usr/share/gdp/arrow-right.png"
        MouseArea {
            anchors.fill: parent
            onClicked: pathView.decrementCurrentIndex()
        }
    }

    Image {
        anchors.bottom: parent.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.horizontalCenterOffset: 150
        source: "file://usr/share/gdp/spot.png"
        transformOrigin: Item.BottomRight
        width: 500
        height: 200
        rotation: settings.spotAnimationPosition * 0.8
        opacity: 0.2
    }

    Image {
        anchors.bottom: parent.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.horizontalCenterOffset: -150
        source: "file://usr/share/gdp/spot.png"
        mirror: true
        transformOrigin: Item.BottomLeft
        width: 500
        height: 200
        rotation: -settings.spotAnimationPosition * 0.8
        opacity: 0.2
    }

    PowerOffItem {
        id: powerOffItem
    }
}
