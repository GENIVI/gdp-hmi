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
 * 10.Feb.2014, Holger Behrens, written
 */

/*! \file gdp-dbus-service.cpp
 *  \brief HMI controller service API on D-Bus for the GENIVI Demo Platform
 *   
 *   This component implements the HMI controller service API and makes it
 *   available on the D-Bus session bus.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <systemd/sd-journal.h>

#include "gdp-hmi-surfaces.h"
#include "gdp-dbus-service.h"
#include "gdp-dbus-systemd.h"

extern SystemdService *gSystemdSession;		// systemd on session bus (d-bus)
extern SystemdService *gSystemd;			// systemd on system  bus (d-bus)

HmiService::HmiService(DBus::Connection &connection)
  : DBus::ObjectAdaptor(connection, GDP_DBUS_SERVICE_PATH)
{
    sd_journal_print(LOG_INFO, "HmiService - constructor (path= %s)\n",
    	GDP_DBUS_SERVICE_PATH);
}

int64_t HmiService::GetId()
{
    sd_journal_print(LOG_DEBUG, "HmiService::GetId() - %d\n", getpid());
	return getpid();
}

std::string HmiService::Show(const std::string &unit)
{
	if (0 == unit.compare(0, 15, "poweroff.target")) {
	    sd_journal_print(LOG_DEBUG, "HmiService::Show() - %s (match)\n",
	    	unit.c_str());
		std::string path = gSystemd->StartUnit(unit, "replace");
	} else {
	    sd_journal_print(LOG_DEBUG, "HmiService::Show() - %s\n",
	    	unit.c_str());
	    for (int count = 0;
	    	count < sizeof gdp_surfaces / sizeof gdp_surfaces[0]; count++) {
	    	if (0 == unit.compare(gdp_surfaces[count].unit)) {
	    		if (ILM_TRUE == gdp_surfaces[count].created) {
	    			// FIXME
	    			// bring gdp_surfaces[count].id_surface to front
				    sd_journal_print(LOG_DEBUG,
				    	"HmiService::Show() - %s surface (%d) exists.\n",
				    	unit.c_str(), gdp_surfaces[count].id_surface);
	    		} else {
	    			// request systemd to start the unit
					std::string path = gSystemdSession->StartUnit(unit, "replace");
				    sd_journal_print(LOG_DEBUG,
				    	"systemd(session)::StartUnit() - %s\n", path.c_str());
	    		}
	    		break; // for-loop
	    	}
	    	else {
			    sd_journal_print(LOG_DEBUG, "DEBUG: compare \"%s\" with \"%s\"",
			    	unit.c_str(), gdp_surfaces[count].unit.c_str());
	    	}
	    } // for-loop
	}
	return "Show unit \"" + unit + "\"!";
}
