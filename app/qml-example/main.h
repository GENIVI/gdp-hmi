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
 * 29.Jan.2014, Holger Behrens, written based on Qt5 documentation
 *          http://doc.qt.io/qt-5/qtqml-cppintegration-interactqmlfromcpp.html
 */

#ifndef MAIN_H
#define MAIN_H

#include <QString>
#include <QDebug>

class MyClass : public QObject
{
    Q_OBJECT

public slots:
    void cppSlot(const QString &msg) {
        qDebug() << "Called the C++ slot with message:" << msg;
    }
};

#endif // MAIN_H