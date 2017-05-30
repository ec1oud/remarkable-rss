#include <QApplication>
#include <QMessageBox>
#include <QString>

#include "util.h"

Util::Util()
{

}

void Util::show_errror(const QString & title, const QString & message) {
    auto msg_box = new QMessageBox(QApplication::topLevelWidgets().front());
    msg_box->setIcon(QMessageBox::Critical);
    msg_box->setAttribute( Qt::WA_DeleteOnClose );
    msg_box->setStandardButtons( QMessageBox::Close );
    msg_box->setWindowTitle(title);
    msg_box->setText(message);
    // msg_box->setModal(false);
    msg_box->show();
}
