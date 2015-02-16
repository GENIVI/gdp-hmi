/**
 * SPDX license identifier: MPL-2.0
 *
 * Copyright (C) 2015, GENIVI Alliance
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
 * 11.Feb.2015, Holger Behrens, written
 * 16.Feb.2015, Holger Behrens, added generic identifier
 */

#ifndef GDP_HMI_APP_H
#define GDP_HMI_APP_H

#define GDP_PANEL                       0 	
#define GDP_PANEL_SURFACE_ID            0
#define GDP_PANEL_LAYER_ID              0
#define GDP_PANEL_UNIT                  "gdp-hmi-panel.service"

#define GDP_LAUNCHER                    1
#define GDP_LAUNCHER_SURFACE_ID         1
#define GDP_LAUNCHER_LAYER_ID           100
#define GDP_LAUNCHER_UNIT               "gdp-hmi-launcher.service"

#define GDP_BACKGROUND                  2
#define GDP_BACKGROUND_SURFACE_ID       2
#define GDP_BACKGROUND_LAYER_ID         200
#define GDP_BACKGROUND_UNIT             "gdp-hmi-background.service"

#define QML_EXAMPLE                     3
#define QML_EXAMPLE_SURFACE_ID          3
#define QML_EXAMPLE_LAYER_ID            300
#define QML_EXAMPLE_UNIT                "qml-example.service"

#define AM_DEMO                         4
#define AM_DEMO_SURFACE_ID              20
#define AM_DEMO_LAYER_ID                300
#define AM_DEMO_UNIT                    "AudioManager_PoC.service"

#define BROWSER_POC                     5
#define BROWSER_POC_SURFACE_ID          30
#define BROWSER_POC_LAYER_ID            400
#define BROWSER_POC_UNIT                "browser.service"

#define FSA                             6
#define FSA_SURFACE_ID                  40
#define FSA_LAYER_ID                    500
#define FSA_UNIT                        "fsa.service"

#define MOCK_NAVIGATION                 7
#define MOCK_NAVIGATION_SURFACE_ID      10
#define MOCK_NAVIGATION_LAYER_ID        600
#define MOCK_NAVIGATION_UNIT            "EGLWLMockNavigation.service"

#define INPUT_EVENT_EXAMPLE             8
#define INPUT_EVENT_EXAMPLE_SURFACE_ID  5100
#define INPUT_EVENT_EXAMPLE_LAYER_ID    700
#define INPUT_EVENT_EXAMPLE_UNIT        "EGLWLInputEventExample.service"

#define GDP_MAX_LAYER_ID                800
#define GDP_LAYER_ID_INCR               100

#endif // GDP_HMI_APP_H