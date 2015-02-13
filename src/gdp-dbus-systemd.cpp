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

/*! \file gdp-dbus-systemd.cpp
 *  \brief HMI controller interface with systemd via D-Bus
 *   
 *   This component implements the HMI controller interface with systemd via
 *   D-Bus. Using the session bus to query application status and using the
 *   system bus for calling the systemd 'PowerOff' method.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <systemd/sd-journal.h>

#include "gdp-dbus-systemd.h"

SystemdService::SystemdService(DBus::Connection &connection,
	const char *path, const char *service)
  : DBus::ObjectProxy(connection, path, service)
{
    sd_journal_print(LOG_INFO, "SystemdService - constructor (path= %s)\n",
    	path);
}
