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