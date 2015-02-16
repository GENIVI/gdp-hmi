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
 * 16.Feb.2015, Holger Behrens, cleanup
 */

#ifndef GDP_HMI_SURFACES_H
#define GDP_HMI_SURFACES_H

#include <string>
#include <ilm/ilm_types.h>

#include "gdp-hmi-apps.h"

struct gdp_surface_context {
	t_ilm_bool visible;
	t_ilm_bool created;
    const t_ilm_uint id_surface;
    const t_ilm_uint id_layer;
    const std::string unit;
};

extern struct gdp_surface_context gdp_surfaces[];
extern const int gdp_surfaces_num;

#endif // GDP_HMI_SURFACES_H