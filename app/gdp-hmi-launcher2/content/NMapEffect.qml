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

/* Simple normal mapping shader */

ShaderEffect {
    id: root
    // Original image
    property string sourceImage
    // Normal mapped image
    property string normalsImage
    // Lightsource which defines the position of light
    property NMapLightSource lightSource
    // Boost diffuse effect of this item
    property real diffuseBoost: 0.0
    // Light intensity from source or alternatively custom intensity for this item
    property real lightIntensity: lightSource.lightIntensity
    // Switch x-coordinate of normal mapped image
    property bool switchX: false
    // Switch y-coordinate of normal mapped image
    property bool switchY: false
    // Set these in case this item isn't directly under main area
    property real elementPositionX: root.x
    property real elementPositionY: root.y
    // Optional 'colorize' effect to apply for the item, can be used for fog effect
    property color colorizeColor: "#404040"
    property real colorizeAmount: 0.0
    // Sizes of the original pixmap
    property real originalWidth: sourceImageItem.sourceSize.width
    property real originalHeight: sourceImageItem.sourceSize.height

    /* Private */
    property real _lightPosX: lightSource.lightPosX / lightSource.width * (lightSource.width/root.width) - elementPositionX/root.width
    property real _lightPosY: lightSource.lightPosY / lightSource.height * (lightSource.height/root.height) - elementPositionY/root.height
    property variant _source: ShaderEffectSource { sourceItem: sourceImageItem; hideSource: true }
    property variant _source2: ShaderEffectSource { sourceItem: normalsourceImageItem; hideSource: true }

    width: sourceImageItem.width
    height: sourceImageItem.height

    Image {
        id: sourceImageItem
        source: sourceImage
        visible: false
    }
    Image {
        id: normalsourceImageItem
        source: normalsImage
        visible: false
    }

    fragmentShader: "
    varying highp vec2 qt_TexCoord0;
    uniform highp float qt_Opacity;
    uniform sampler2D _source;
    uniform sampler2D _source2;
    uniform highp float _lightPosX;
    uniform highp float _lightPosY;
    uniform highp float diffuseBoost;
    uniform highp float lightIntensity;
    uniform highp float colorizeAmount;
    uniform highp vec4 colorizeColor;
    uniform bool switchX;
    uniform bool switchY;

    void main(void)
    {
        highp vec2 pixPos = qt_TexCoord0;
        highp vec4 pix = texture2D(_source, pixPos.st);
        highp vec4 pix2 = texture2D(_source2, pixPos.st);
        highp vec3 normal = normalize(pix2.rgb * 2.0 - 1.0);
        highp float xp = float(switchX) * (_lightPosX - pixPos.x) + float(!switchX) * (pixPos.x - _lightPosX);
        highp float yp = float(switchY) * (_lightPosY - pixPos.y) + float(!switchY) * (pixPos.y - _lightPosY);
        highp vec3 light_pos = normalize(vec3(xp, yp, lightIntensity));

        highp float diffuse = max(dot(normal, light_pos), 0.2);
        diffuse *= (1.0 + diffuseBoost);

        highp vec4 color = vec4(diffuse * pix.rgb, pix.a);
        color = mix(color, color.a * colorizeColor, colorizeAmount);

        gl_FragColor = color * qt_Opacity;
    }
    "
}
