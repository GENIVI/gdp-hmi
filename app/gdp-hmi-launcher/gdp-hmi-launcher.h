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
 * 09.Feb.2015, Holger Behrens, written
 * 10.Feb.2015, Holger Behrens, added supprot for dbus
 */

#ifndef GDP_HMI_LAUNCHER_H
#define GDP_HMI_LAUNCHER_H

#include <QDebug>
#include <QObject>
#include <QString>
#include <QtDBus>

#include <signal.h>
#include <sys/types.h>
#include <systemd/sd-journal.h>

#include "gdp-hmi-introspect_interface.h"

class GDPLauncherClass : public QObject
{
    Q_OBJECT

public:
    GDPLauncherClass();
    ~GDPLauncherClass();

public slots:
    void hmiRequestOffSlot() {
        QString unit = QStringLiteral("poweroff.target");
        QString status = m_controller->Show(unit);
        sd_journal_print(LOG_DEBUG,
            "DEBUG: controller->Show(\"%s\") -> %s\n",
            unit.toLatin1().data(), status.toLatin1().data());
    }
    void hmiAppLaunchSlot(const QString &unit) {
        QString status = m_controller->Show(unit);
        sd_journal_print(LOG_DEBUG,
            "DEBUG: controller->Show(\"%s\") -> %s\n",
            unit.toLatin1().data(), status.toLatin1().data());
    }

protected:
    void timerEvent(QTimerEvent *event);

private:
	pid_t m_hmiControllerPid;
    int m_timerId;
    org::genivi::gdp::HMI_Controller *m_controller;
};

#endif // GDP_HMI_LAUNCHER_H