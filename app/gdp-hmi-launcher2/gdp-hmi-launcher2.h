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
 * 16.Mar.2015, Sebastien Baudouin, added supprot for dbus
 */

#ifndef GDP_HMI_LAUNCHER2_H
#define GDP_HMI_LAUNCHER2_H

#include <QDebug>
#include <QObject>
#include <QString>
#include <QtDBus>

#include <signal.h>
#include <sys/types.h>
#include <systemd/sd-journal.h>

#include <persistence_client_library.h>     /* Init/DeInit PCL                */
#include <persistence_client_library_key.h> /* Access persistent data         */

#include "gdp-hmi-introspect_interface.h"

class GDPLauncherClass : public QObject
{
    Q_OBJECT

public:
    GDPLauncherClass();
    ~GDPLauncherClass();
    int readLastUserAppIndex();
    void writeLastUserAppIndex(int index);

    Q_INVOKABLE int getLastAppIndex() {
        m_selectedAppIndex = readLastUserAppIndex();
        return m_selectedAppIndex;
    }

    Q_INVOKABLE void setLastAppIndex(int index) {
        if (index != m_selectedAppIndex) {
            m_selectedAppIndex = index;
        }
        writeLastUserAppIndex(m_selectedAppIndex);
    }


public slots:
    void hmiRequestOffSlot() {
        QString unit = QStringLiteral("PowerOff.service");
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
    int m_selectedAppIndex;
    org::genivi::gdp::HMI_Controller *m_controller;
};

#endif // GDP_HMI_LAUNCHER2_H
