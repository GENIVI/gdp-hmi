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

#ifndef GDP_SYSTEMD_SERVICE_H
#define GDP_SYSTEMD_SERVICE_H

#include <dbus-c++/dbus.h>
#include <dbus-c++/glib-integration.h>
#include <systemd/sd-journal.h>

#include "systemd1-client-glue.h"

static const char *SYSTEMD_DBUS_SERVICE_NAME = "org.freedesktop.systemd1";
static const char *SYSTEMD_DBUS_SERVICE_PATH = "/org/freedesktop/systemd1";

class SystemdService : public org::freedesktop::systemd1::Manager_proxy,
  public DBus::IntrospectableProxy,
  public DBus::ObjectProxy
{
public:
	SystemdService(DBus::Connection &connection,
		const char *path, const char *service);

private:
	// make virtual functions dirty no-op's
	void UnitNew(const std::string& argin0, const ::DBus::Path& argin1){}
	void UnitRemoved(const std::string& argin0, const ::DBus::Path& argin1) {}
	void JobNew(const uint32_t& argin0, const ::DBus::Path& argin1,
		const std::string& argin2) {}
	void JobRemoved(const uint32_t& argin0, const ::DBus::Path& argin1,
		const std::string& argin2, const std::string& argin3) {}
	void StartupFinished(const uint64_t& argin0, const uint64_t& argin1,
		const uint64_t& argin2, const uint64_t& argin3,
		const uint64_t& argin4, const uint64_t& argin5) {}
	void UnitFilesChanged() {}
	void Reloading(const bool& argin0) {}
};

#endif // GDP_SYSTEMD_SERVICE_H