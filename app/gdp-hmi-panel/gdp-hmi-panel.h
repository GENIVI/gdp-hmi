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
 * 16.feb.2015, Holger Behrens, cleanup signal handling
 */

#ifndef GDP_HMI_PANEL_H
#define GDP_HMI_PANEL_H

#include <signal.h>
#include <sys/types.h>
#include <systemd/sd-journal.h>

#include <QObject>
#include <QString>
#include <QDebug>

class GDPPanelClass : public QObject
{
    Q_OBJECT

public:
    GDPPanelClass();
    ~GDPPanelClass();

public slots:
    void homeSlot() {
    	int rtn = -1;
        rtn = kill(m_hmiControllerPid, SIGUSR1); // show launcher
        if (-1 == rtn) {
	        sd_journal_print(LOG_ERR,
	        	"Error: Signaling PID (%d) failed (SIGUSR1).\n",
	        	m_hmiControllerPid);
        }
    }

private:
	pid_t m_hmiControllerPid;
};

#endif // GDP_HMI_PANEL_H