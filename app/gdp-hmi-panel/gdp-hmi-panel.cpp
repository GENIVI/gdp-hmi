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
 * 06.Feb.2014, Holger Behrens, written, inspired by Qt5 documentation
 *          http://doc.qt.io/qt-5/qtqml-cppintegration-interactqmlfromcpp.html
 */

/*! \file gdp-hmi-panel.cpp
 *  \brief HMI panel class for the GENIVI Demo Platform
 *   
 *  This component implements the HMI panel class of the GENIVI Demo Platform.
 */

#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <systemd/sd-journal.h>

#include "gdp-hmi-panel.h"

#define GDP_HMI_PID_FILENAME "/var/run/gdp-hmi-controller.pid"

GDPPanelClass::GDPPanelClass()
: m_hmiControllerPid(-1)
{
	int fd;
	char stringBuffer[256];

	fd = open(GDP_HMI_PID_FILENAME, O_RDONLY | O_CLOEXEC, 0);
	if (-1 == fd) {
		strerror_r(errno, stringBuffer, 256);
        sd_journal_print(LOG_ERR,
        	"Error: pid_file (open) - %s. Exiting.\n", stringBuffer);
		exit(EXIT_FAILURE);
	}

	if (-1 == read(fd, stringBuffer, 256)) {
        sd_journal_print(LOG_ERR,
        	"Error: Reading from PID file failed. Exiting.\n");
		exit(EXIT_FAILURE);
	}

	m_hmiControllerPid = std::stoi(stringBuffer);
    sd_journal_print(LOG_DEBUG, "Debug: GDPPanelClass() - pid read = %d\n",
    	m_hmiControllerPid);
}

GDPPanelClass::~GDPPanelClass()
{
}
