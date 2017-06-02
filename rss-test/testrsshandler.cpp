#include <QXmlStreamAttributes>
#include <QStringRef>

#include "testrsshandler.h"

TestRssHandler::TestRssHandler()
{

}

void TestRssHandler::cleanup()
{
    rss_handler_.clear();
}

void TestRssHandler::parse_ok()
{
    QString e("");
    QString rss("rss");

    QStringRef a (&e);
    QStringRef b (&rss);

    auto ret = rss_handler_.start_element(a, a, b, QXmlStreamAttributes());

    QCOMPARE(ret, false);
    QVERIFY(ret == false);
}

void TestRssHandler::parse_fail()
{

}
