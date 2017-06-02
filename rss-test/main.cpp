#include <QCoreApplication>

#include "testrsshandler.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    TestRssHandler test;

    QStringList arguments = QCoreApplication::arguments();

    return QTest::qExec(&test, arguments);
}
