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
 * 16.Mar.2015, Sebastien Baudouin, written based on template created by QtCreator
 */
#include <QGuiApplication>
#include <QQuickView>
#include <QtDBus>
#include <systemd/sd-journal.h>

#include "gdp-hmi-launcher2.h"
#define GDP_LAUNCHER2_SURFACE_ID 1

int main(int argc, char* argv[])
{
    setenv("QT_QPA_PLATFORM", "wayland", 1); // force to use wayland plugin
    setenv("QT_WAYLAND_DISABLE_WINDOWDECORATION", "1", 1);

    sd_journal_print(LOG_DEBUG, "GDP: launcher2 surface (id=%u)",
            GDP_LAUNCHER2_SURFACE_ID);

    QObject *object;
    QGuiApplication app(argc,argv);

    QQuickView view(QUrl(QStringLiteral("qrc:/gdp-hmi-launcher2.qml")));

    view.setResizeMode(QQuickView::SizeRootObjectToView);
    object = (QObject *)view.rootObject();

    GDPLauncherClass launcher2;
    QObject::connect(object, SIGNAL(appSelectSignal(QString)),
            &launcher2, SLOT(hmiAppLaunchSlot(QString)));
    QObject::connect(object, SIGNAL(requestOffSignal()),
            &launcher2, SLOT(hmiRequestOffSlot()));

    view.setProperty("IVI-Surface-ID", GDP_LAUNCHER2_SURFACE_ID);
    view.showFullScreen();

    return app.exec();
}
