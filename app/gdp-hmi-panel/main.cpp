/**
 * SPDX license identifier: MPL-2.0
 *
 * Copyright (C) 2015 GENIVI Alliance
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
 * 06.Feb.2015, Holger Behrens, written based on template created by QtCreator
 * 16.feb.2015, Holger Behrens, cleanup signal handling
 */

#include <QApplication>
#include <QQuickView>
#include <systemd/sd-journal.h>

#include "gdp-hmi-panel.h"

#define GDP_PANEL_SURFACE_ID 0

int main(int argc, char *argv[])
{
    setenv("QT_QPA_PLATFORM", "wayland", 1); // force to use wayland plugin
    setenv("QT_WAYLAND_DISABLE_WINDOWDECORATION", "1", 1);

    sd_journal_print(LOG_DEBUG, "GDP: panel surface (id=%u)",
        GDP_PANEL_SURFACE_ID);

    QObject *object;
    QGuiApplication app(argc, argv);
    QQuickView view(QUrl(QStringLiteral("qrc:/main.qml")));
    object = (QObject *)view.rootObject();

    GDPPanelClass panel;
    QObject::connect(object, SIGNAL(homeSignal()), &panel, SLOT(homeSlot()));

    view.setProperty("IVI-Surface-ID", GDP_PANEL_SURFACE_ID);
    view.show();
    return app.exec();
}
