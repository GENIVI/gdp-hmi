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
 * 16.Mar.2015, Sebastien Baudouin, written
 */

/*! \file gdp-hmi-launcher2.cpp
 *  \brief HMI launcher class for the GENIVI Demo Platform
 *   
 *  This component implements the HMI launcher class of the
 *  GENIVI Demo Platform.
 */

#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#ifdef USE_DLT
#include <dlt/dlt.h>
DLT_IMPORT_CONTEXT(launcherTraceCtx);
#else
#include <systemd/sd-journal.h>
#endif


#include "gdp-hmi-launcher2.h"

static const char *GDP_HMI_PID_FILENAME  = "/var/run/gdp-hmi-controller.pid";
static const char *GDP_DBUS_SERVICE_NAME = "org.genivi.gdp.hmi.controller";
static const char *GDP_DBUS_SERVICE_PATH = "/org/genivi/gdp/hmi/controller";

#define GDP_LAUNCHER2_LAST_APP_UNIT_DB_ID 0xFF
#define GDP_LAUNCHER2_LAST_APP_UNIT_KEY "LastAppIndex"
#define GDP_LAUNCHER2_USER 3
#define GDP_LAUNCHER2_SEAT 2
#define GDP_LAUNCHER2_INDEX_MAX 0xFF

GDPLauncherClass::GDPLauncherClass()
: m_hmiControllerPid(-1)
{
    int pcl_return;
#ifdef USE_DLT
    DLT_LOG(launcherTraceCtx,DLT_LOG_INFO,DLT_STRING("Debug: GDPLauncherClass - dbus session.\n"));
#else
    sd_journal_print(LOG_DEBUG, "Debug: GDPLauncherClass - dbus session.\n");
#endif
	m_controller = new org::genivi::gdp::HMI_Controller(GDP_DBUS_SERVICE_NAME,
		GDP_DBUS_SERVICE_PATH, QDBusConnection::sessionBus(), this);
    m_timerId = startTimer(5000); // 5 second timer

    /* Initialize Persistence Client Library */
    pcl_return = pclInitLibrary("Launcher", PCL_SHUTDOWN_TYPE_NORMAL | PCL_SHUTDOWN_TYPE_FAST);
    if (pcl_return < 0) {
#ifdef USE_DLT
    DLT_LOG(launcherTraceCtx, DLT_LOG_INFO,
            DLT_STRING("Launcher2: Failed to initialize PCL.");
            DLT_STRING("Error: Unexpected PCL return.");
            DLT_STRING("Return:"); DLT_INT(pcl_return));
#else
    sd_journal_print(LOG_DEBUG, "Launcher2: Failed to initialize PCL\n");
#endif
    return;
    }
}

GDPLauncherClass::~GDPLauncherClass()
{
    int pcl_return;

    /* Deinitialize the PCL */
    pcl_return = pclDeinitLibrary();

    if (pcl_return < 0) {
#ifdef USE_DLT
        DLT_LOG(launcherTraceCtx, DLT_LOG_INFO,
                DLT_STRING("Launcher2: Failed to deinitialize PCL.");
                DLT_STRING("Error: Unexpected PCL return.");
                DLT_STRING("Return:"); DLT_INT(pcl_return));
#else
        sd_journal_print(LOG_DEBUG, "Launcher2: Failed to deinitialize PCL.\n");
#endif
    }
}

int GDPLauncherClass::readLastUserAppIndex()
{
    int pcl_return;
    unsigned char value[8];
    /* Get data from persistence */

    pcl_return = pclKeyReadData(GDP_LAUNCHER2_LAST_APP_UNIT_DB_ID,
            GDP_LAUNCHER2_LAST_APP_UNIT_KEY,
            GDP_LAUNCHER2_USER,
            GDP_LAUNCHER2_SEAT,
            value,
            sizeof(value));
    if (pcl_return != sizeof(value)) {
#ifdef USE_DLT
        DLT_LOG(launcherTraceCtx, DLT_LOG_INFO,
                DLT_STRING("Launcher2: Failed to read Last Application Unit.");
                DLT_STRING("Error: Unexpected PCL return.");
                DLT_STRING("Return:"); DLT_INT(pcl_return));
#else
        sd_journal_print(LOG_DEBUG, "Launcher2: Failed to read Last Application Unit.\n");
#endif
        return GDP_LAUNCHER2_INDEX_MAX;
    }

    return atoi((char*)value);
}


void GDPLauncherClass::writeLastUserAppIndex(int index)
{
    int pcl_return;
    unsigned char value[8];

    sprintf((char*)value, "%d", index);
    /* Set data to persistence */

    pcl_return = pclKeyWriteData(GDP_LAUNCHER2_LAST_APP_UNIT_DB_ID,
            GDP_LAUNCHER2_LAST_APP_UNIT_KEY,
            GDP_LAUNCHER2_USER,
            GDP_LAUNCHER2_SEAT,
            value,
            sizeof(value));
    if (pcl_return != sizeof(value)) {
#ifdef USE_DLT
        DLT_LOG(launcherTraceCtx, DLT_LOG_INFO,
                DLT_STRING("Launcher2: Failed to write Last Application Unit.");
                DLT_STRING("Error: Unexpected PCL return.");
                DLT_STRING("Return:"); DLT_INT(pcl_return));
#else
        sd_journal_print(LOG_DEBUG, "Launcher2: Failed to write Last Application Unit.\n");
#endif
    }
}

void GDPLauncherClass::timerEvent(QTimerEvent *event)
{
    Q_UNUSED(event);
    if ( !(m_controller->isValid()) ) {
#ifdef USE_DLT
        DLT_LOG(launcherTraceCtx,DLT_LOG_INFO,DLT_STRING("Info: HMI Controller - disconnected.\n"));
#else
        sd_journal_print(LOG_INFO, "Info: HMI Controller - disconnected.\n");
#endif
    }
}
