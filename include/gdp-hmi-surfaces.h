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
 * 11.Feb.2014, Holger Behrens, written
 */

#ifndef GDP_HMI_SURFACES_H
#define GDP_HMI_SURFACES_H

#include <string>
#include <ilm/ilm_types.h>

#include "gdp-hmi-apps.h"

struct gdp_surface_context {
	t_ilm_bool created;
    const t_ilm_uint id_surface;
    const t_ilm_uint id_layer;
    const std::string unit;
};

static struct gdp_surface_context gdp_surfaces[] = {
	{
		ILM_FALSE,
		GDP_PANEL_SURFACE_ID,
		GDP_PANEL_LAYER_ID,
		GDP_PANEL_UNIT
	},
	{
		ILM_FALSE,
		GDP_LAUNCHER_SURFACE_ID,
		GDP_LAUNCHER_LAYER_ID,
		GDP_LAUNCHER_UNIT
	},
	{
		ILM_FALSE,
		GDP_BACKGROUND_SURFACE_ID,
		GDP_BACKGROUND_LAYER_ID,
		GDP_BACKGROUND_UNIT
	},
	{
		ILM_FALSE,
		QML_EXAMPLE_SURFACE_ID,
		QML_EXAMPLE_LAYER_ID,
		QML_EXAMPLE_UNIT
	},
	{
		ILM_FALSE,
		AM_DEMO_SURFACE_ID,
		AM_DEMO_LAYER_ID,
		AM_DEMO_UNIT
	},
	{
		ILM_FALSE,
		BROWSER_POC_SURFACE_ID,
		BROWSER_POC_LAYER_ID,
		BROWSER_POC_UNIT
	},
	{
		ILM_FALSE,
		FSA_SURFACE_ID,
		FSA_LAYER_ID,
		FSA_UNIT
	},
	{
		ILM_FALSE,
		MOCK_NAVIGATION_SURFACE_ID,
		MOCK_NAVIGATION_LAYER_ID,
		MOCK_NAVIGATION_UNIT
	},
	{
		ILM_FALSE,
		INPUT_EVENT_EXAMPLE_SURFACE_ID,
		INPUT_EVENT_EXAMPLE_LAYER_ID,
		INPUT_EVENT_EXAMPLE_UNIT
	},
};

#endif // GDP_HMI_SURFACES_H