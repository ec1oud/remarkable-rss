#include <QDebug>
#include <QString>

#include "util.h"

Util::Util()
{

}

void Util::show_error(const QString & title, const QString & message) {
    qWarning() << title << ":" << message;
    // TODO how should the QML UI show it?
}
