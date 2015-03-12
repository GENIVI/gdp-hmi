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
 *
 */
import QtQuick 2.0

/* Light source for the normal mapping shader */

Item {
    id: root

    // The position of light, read-only to keep translate
    property real lightPosX: priv.lPosX + lightTranslateX
    property real lightPosY: priv.lPosY + lightTranslateY

    // Translate position, for e.g. animating light slightly
    property real lightTranslateX: 0
    property real lightTranslateY: 0

    // The intensity of light
    property real lightIntensity: 0.4;

    // Set the position for light, forces inside the area
    function setLightPos(xpos, ypos) {
        priv.lPosX = Math.min(root.width, Math.max(0, xpos));
        priv.lPosY = Math.min(root.height, Math.max(0, ypos));
    }

    // This item should fill the whole area where light is used
    anchors.fill: parent

    Item {
        id: priv
        property real lPosX: root.width/2;
        property real lPosY: root.height/2;
    }
}
