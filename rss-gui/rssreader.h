#ifndef RSSPARSER_H
#define RSSPARSER_H

#include <QObject>
#include <QtNetwork/QNetworkAccessManager>
#include <QXmlStreamReader>

#include "rss.h"
#include "news.h"
#include "rsshandler.h"

class QByteArray;
class QNetworkReply;
class NewsModel;

class RssReader : public QObject
{
    Q_OBJECT
public:
    RssReader();
    void set_model(NewsModel * model);
    void load(const QUrl & url);

    Rss rss() const;

signals:
    void finished();

private slots:
    void reply_finished(QNetworkReply* reply);
    void ready_read();

private:
    QNetworkAccessManager manager_;  // TODO (edgard.lima): pass as argument (dependecy injection) for unit test
    QNetworkReply * reply_;
    QXmlStreamReader reader_;
    RssHandler rss_handler_;  // TODO (edgard.lima): pass as argument (dependecy injection) for unit test
    NewsModel * model_;

    void parse();
};

#endif // RSSPARSER_H
