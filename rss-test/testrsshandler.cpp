#include <QXmlStreamAttributes>
#include <QStringRef>

#include "testrsshandler.h"

TestRssHandler::TestRssHandler()
{

}

void TestRssHandler::init()
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

    QXmlStreamAttributes attr;
    attr.append("version", "2.0");

    auto ret = rss_handler_.start_element(a, a, b, attr);

    QCOMPARE(ret, true);
}

void TestRssHandler::parse_fail()
{
    QString e("");
    QString rss("another");

    QStringRef a (&e);
    QStringRef b (&rss);

    QXmlStreamAttributes attr;
    attr.append("version", "2.0");

    auto ret = rss_handler_.start_element(a, a, b, attr);

    QCOMPARE(ret, false);
}
