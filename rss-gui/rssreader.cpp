#include <string>

#include <QByteArray>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QLocale>
#include <QXmlParseException>

#include "rssreader.h"
#include "decoratednewsmodel.h"

RssReader::RssReader()
    : reply_(nullptr)
    , rss_hanler_(reader_)
    , model_(nullptr)
{
    connect(&manager_, &QNetworkAccessManager::finished, this, &RssReader::reply_finished);
}

void RssReader::set_model(DecoratedNewsModel * model) {
    model_ = model;
    rss_hanler_.set_model(model);
}

void RssReader::load(const QUrl & url) {
    if (reply_) reply_->abort();

    QNetworkRequest request(url);
    reply_ = manager_.get(QNetworkRequest(request));
    connect(reply_, &QIODevice::readyRead, this, &RssReader::ready_read);
}

void RssReader::reply_finished(QNetworkReply* reply) {
    int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    qDebug() << "Reponse network error" << reply->error();
    qDebug() << "Reponse HTTP status code" << statusCode;
    qDebug() << "Reply content:" << reply->readAll();

    if (reply_ != reply) {
        qWarning() << "reply differs";
    }
    reply->deleteLater();
    reply_ = nullptr;
}

void RssReader::ready_read() {
    reader_.addData(reply_->readAll());
    parse();
}

void RssReader::parse() {

    bool parse_ok = true;

    while (!reader_.atEnd() && parse_ok) {
        reader_.readNext();
        if (reader_.isStartElement()) {
            parse_ok = rss_hanler_.start_element(reader_.namespaceUri(),
                                             QStringRef(),
                                             reader_.name(),
                                             reader_.attributes());
        } else if (reader_.isEndElement()) {
            parse_ok = rss_hanler_.end_element(reader_.namespaceUri(),
                                             QStringRef(),
                                             reader_.name());
        } else if (reader_.isCharacters() && !reader_.isWhitespace()) {
            parse_ok = rss_hanler_.characters(reader_.text());
        }
    }

    if (!parse_ok) {
        QXmlParseException ex(reader_.errorString(), reader_.columnNumber(), reader_.lineNumber());
        rss_hanler_.fatalError(ex);
        if (reply_) reply_->abort();
    } else if (reader_.error() && reader_.error() != QXmlStreamReader::PrematureEndOfDocumentError) {
        QXmlParseException ex(reader_.errorString(), reader_.columnNumber(), reader_.lineNumber());
        rss_hanler_.fatalError(ex);
        if (reply_) reply_->abort();
    }
}
