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

#ifndef GDP_HMI_SERVICE_H
#define GDP_HMI_SERVICE_H

#include <dbus-c++/dbus.h>
#include <dbus-c++/glib-integration.h>
#include <systemd/sd-journal.h>

#include "gdp-hmi-server-glue.h"

static const char *GDP_DBUS_SERVICE_NAME = "org.genivi.gdp.hmi.controller";
static const char *GDP_DBUS_SERVICE_PATH = "/org/genivi/gdp/hmi/controller";

class HmiService : public org::genivi::gdp::HMI_Controller_adaptor,
  public DBus::IntrospectableAdaptor,
  public DBus::ObjectAdaptor
{
public:
	HmiService(DBus::Connection &connection);

	int64_t GetId();

	std::string Show(const std::string &name);
};

#endif // GDP_HMI_SERVICE_H