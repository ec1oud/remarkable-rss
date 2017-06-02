#ifndef TESTRSSHANDLER_H
#define TESTRSSHANDLER_H

#include <QtTest/QTest>

#include "rsshandler.h"

class TestRssHandler : public QObject
{
    Q_OBJECT
public:
    TestRssHandler();

private slots:
    //void initTestCase();
    void init();
    void cleanup();
    //void cleanupTestCase();

    void parse_ok();
    void parse_fail();


private:
    RssHandler rss_handler_;
};

#endif // TESTRSSHANDLER_H
