/**
 * SPDX license identifier: MPL-2.0
 *
 * Copyright (C) 2015 GENIVI Alliance
 *
 * This file is part of GENIVI AudioManager PoC.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License (MPL), v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * For further information see http://www.genivi.org/.
 *
 * List of changes:
 * 29.01.2015, Holger Behrens, written based on template created by QtCreator
 *
 */

#include <QApplication>
#include <QQuickView>
#include <systemd/sd-journal.h>

#include "main.h"

#define QML_EXAMPLE_SURFACE_ID 3

int main(int argc, char *argv[])
{
    setenv("QT_QPA_PLATFORM", "wayland", 1); // force to use wayland plugin
    setenv("QT_WAYLAND_DISABLE_WINDOWDECORATION", "1", 1);

	sd_journal_print(LOG_DEBUG, "GDP: qml-example");

    QObject *object;
    QGuiApplication app(argc, argv);
    QQuickView view(QUrl(QStringLiteral("qrc:/main.qml")));
    object = (QObject *)view.rootObject();

    MyClass myClass;
    QObject::connect(object, SIGNAL(qmlSignal(QString)),
                     &myClass, SLOT(cppSlot(QString)));

    view.setProperty("IVI-Surface-ID", QML_EXAMPLE_SURFACE_ID);
    view.show();

    return app.exec();
}
