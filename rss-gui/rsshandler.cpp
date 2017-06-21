#include <QtXml>
#include <QtGui>
#include <QDebug>
#include <QMessageBox>
#include <QXmlStreamReader>
#include <QXmlParseException>

#include "rsshandler.h"
#include "util.h"
#include "newsmodel.h"

RssHandler::RssHandler()
    : model_(nullptr)
{
}

void RssHandler::set_model(NewsModel * model) {
    model_ = model;
}

void RssHandler::clear() {
    if (model_) model_->removeRows(0, model_->rowCount(), QModelIndex());
    rss_ = Rss();
    current_news_ = News();
    stack_.clear();
}

Rss RssHandler::rss() const
{
    return rss_;
}

void RssHandler::set_rss(const Rss &rss)
{
    rss_ = rss;
}

bool RssHandler::start_element(const QStringRef & /* ns_uri */,
                               const QStringRef & /* local_name */,
                               const QStringRef & name,
                               const QXmlStreamAttributes &attributes )
{
    stack_.push_back(name.toString().toStdString());

    const std::string & parent = stack_.size() <= 1 ? std::string("") : stack_[stack_.size()-2];
    const std::string & pparent = stack_.size() <= 2 ? std::string("") : stack_[stack_.size()-3];

    qDebug() << name;

    bool is_img_thumnail_elem = pparent == "channel" && parent == "item";

    if (stack_.front() != "rss") {
        err_string_ = QObject::tr("This file is not an RSS source.");
        return false;
    }
    if (name == "rss") {
        if (stack_.size() > 1) {
            err_string_ = QObject::tr("This file contains two RSS elements.");
            return false;
        }
        auto version = attributes.value("version");
        if (version.isEmpty() /* || version != "2.0" */) {
            err_string_ = QObject::tr("Can only handle RSS 2.0.");
            return false;
        }
    } else if (name == "channel") {
        if (parent != "rss") {
            err_string_ = QObject::tr("channel is not rss's child.");
            return false;
        }
    } else if (name == "item") {
        if (parent != "channel") {
            err_string_ = QObject::tr("item is not channel's child.");
            return false;
        }
    } else if (name == "thumbnail") {
        if (is_img_thumnail_elem) {
            auto url = attributes.value("url").toString();
            current_news_.set_image_link(url);
        }
    } else {

    }
    current_text_ = "";
    return true;
}

bool RssHandler::characters( const QStringRef &str )
{
    current_text_ += str;
    return true;
}

// rssreader/rsshandler.cpp (continued)
bool RssHandler::end_element( const QStringRef & /* ns_uri */,
                              const QStringRef & /* local_name */,
                              const QStringRef &name )
{
    const std::string & parent = stack_.size() <= 1 ? std::string("") : stack_[stack_.size()-2];
    const std::string & pparent = stack_.size() <= 2 ? std::string("") : stack_[stack_.size()-3];

    bool is_item_elem = parent == "item";
    bool is_channel_elem = parent == "channel";
    bool is_channel_img_elem = pparent == "channel" && parent == "image";

    // todo (edgard.lima): replace if-else with hash with lambdas

    if ( name == "channel" ) {
    } else if ( name == "item" ) {
        if (model_) model_->add_news(current_news_);
        current_news_ = News();
    } else if ( name == "title" ) {
        if (is_item_elem) {
            current_news_.set_title(current_text_);
        } else if (is_channel_elem) {
            rss_.set_title(current_text_);
        }
    } else if ( name == "link" ) {
        if (is_item_elem) {
            current_news_.set_link(current_text_);
        } else if (is_channel_elem) {
            rss_.set_link(current_text_);
        }
    } else if ( name == "pubDate" ) {
        if (is_item_elem) {
            // ddd, dd MMM yyyy HH:mm:ss TZ
            auto aux = QDateTime::fromString(current_text_, Qt::RFC2822Date);
            current_news_.set_date(aux);
        }
    } else if ( name == "description" ) {
        if (is_item_elem) {
            current_news_.set_description(current_text_);
        } else if (is_channel_elem) {
            rss_.set_description(current_text_);
        }
    } else if ( name == "lastBuildDate" ) {
        if (is_channel_elem) {
            // ddd, dd MMM yyyy HH:mm:ss TZ
            auto aux = QDateTime::fromString(current_text_, Qt::RFC2822Date);
            rss_.set_date(aux);
        }
    } else if (name == "language") {
        if (is_channel_elem) {
            rss_.setLanguage(current_text_);
        }
    } else if (name == "url") {
        if (is_channel_img_elem) {
            rss_.set_image_link(current_text_);
        }
    } else if (name == "ttl") {
        if (is_channel_elem) {
            rss_.set_ttl(current_text_.toInt());
        }
    }

    stack_.pop_back();
    current_text_.clear();
    return true;
}


QString RssHandler::errorString() const
{
    return err_string_;
}

bool RssHandler::fatal_error( const QXmlParseException &exception )
{
    Util::show_errror(QObject::tr( "RSS-Reader" ),
                      QObject::tr( "Parse error in line %1, columne %2: \n %3" )
                      .arg( exception.lineNumber() )
                      .arg( exception.columnNumber() )
                      .arg( exception.message() ) );
    return false;
}
