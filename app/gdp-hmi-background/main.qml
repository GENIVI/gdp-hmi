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

Rectangle {
    id: background
    visible: true
    width: 1024
    height: 768

    Image {
        id: background_image
        fillMode: Image.PreserveAspectFit
        smooth: false
        enabled: false
        anchors.fill: parent
        // sourceSize.height: 768
        // sourceSize.width: 1024
        source: "file:///usr/share/gdp/GDP_Background.png"
    }
}
