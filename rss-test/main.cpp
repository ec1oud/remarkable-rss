#include <QCoreApplication>

#include "testrsshandler.h"

// Add a test forving a slow network
// sudo tc qdisc add dev  wlp2s0  root netem delay 1000ms

int main(int argc, char *argv[])
{
    // QCoreApplication a(argc, argv);

    TestRssHandler test;

    QStringList arguments = QCoreApplication::arguments();

    //return QTest::qExec(&test, arguments);
    return QTest::qExec(&test);
}
