#include <string>

#include <QByteArray>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QLocale>
#include <QXmlParseException>

#include "rssreader.h"
#include "util.h"
#include "newsmodel.h"

RssReader::RssReader()
    : reply_(nullptr)
    , model_(nullptr)
{
    connect(&manager_, &QNetworkAccessManager::finished, this, &RssReader::reply_finished);
}

void RssReader::set_model(NewsModel * model) {
    model_ = model;
    rss_handler_.set_model(model);
}

void RssReader::load(const QUrl & url) {
    if (reply_) reply_->abort();

    // todo (edgard.lima): use Etag and if-modified

    rss_handler_.clear();
    reader_.clear();
    model_->removeRows(0, model_->rowCount(), QModelIndex());
    QNetworkRequest request(url);
    reply_ = manager_.get(QNetworkRequest(request));
    connect(reply_, &QIODevice::readyRead, this, &RssReader::ready_read);
}

Rss RssReader::rss() const {
    return rss_handler_.rss();
}

void RssReader::reply_finished(QNetworkReply* reply) {
    //int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    if (reply->error() != QNetworkReply::NoError && reply->error() != QNetworkReply::OperationCanceledError) {
        Util::show_errror("Http error", reply->errorString());
    }
    qDebug() << "Read finished. Error code:" << reply->errorString();

    // qDebug() << "original request" << reply_->request().url();

    reply->deleteLater();
    reply_ = nullptr;

    emit finished();
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
            parse_ok = rss_handler_.start_element(reader_.namespaceUri(),
                                             QStringRef(),
                                             reader_.name(),
                                             reader_.attributes());
        } else if (reader_.isEndElement()) {
            parse_ok = rss_handler_.end_element(reader_.namespaceUri(),
                                             QStringRef(),
                                             reader_.name());
        } else if (reader_.isCharacters() && !reader_.isWhitespace()) {
            parse_ok = rss_handler_.characters(reader_.text());
        }
    }

    if (!parse_ok) {
        QXmlParseException ex(reader_.errorString(), reader_.columnNumber(), reader_.lineNumber());
        rss_handler_.fatal_error(ex);
        if (reply_) {
            reply_->abort();
        }
    } else if (reader_.error() && reader_.error() != QXmlStreamReader::PrematureEndOfDocumentError) {
        QXmlParseException ex(reader_.errorString(), reader_.columnNumber(), reader_.lineNumber());
        rss_handler_.fatal_error(ex);
        if (reply_) {
            reply_->abort();
        }
    }
}
