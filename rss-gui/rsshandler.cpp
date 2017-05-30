#include <QtXml>
#include <QtGui>
#include <QDebug>
#include <QMessageBox>
#include <QXmlStreamReader>
#include <QXmlParseException>

#include "rsshandler.h"
#include "decoratednewsmodel.h"
#include "util.h"

RssHandler::RssHandler(QXmlStreamReader & reader)
    : reader_(reader)
    , model_(nullptr)
{
}

void RssHandler::set_model(DecoratedNewsModel * model) {
    model_ = model;
    if (model_) {
        model_->setHeaderData(0, Qt::Horizontal, QObject::tr("Title"));
        model_->setHeaderData(1, Qt::Horizontal, QObject::tr("Date"));
    }
}

void RssHandler::clear() {
    model_->clear();
    rss_build_date_.clear();
    rss_ = Rss();
    current_news_ = News();
    current_preview_.clear();
    stack_.clear();
}

Rss RssHandler::rss() const
{
    return rss_;
}

void RssHandler::setRss(const Rss &rss)
{
    rss_ = rss;
}

bool RssHandler::start_element(const QStringRef & ns_uri,
                               const QStringRef & local_name,
                               const QStringRef & name,
                               const QXmlStreamAttributes &attributes )
{
    stack_.push_back(name.toString().toStdString());

    const std::string & parent = stack_.size() <= 1 ? std::string("") : stack_[stack_.size()-2];
    const std::string & pparent = stack_.size() <= 2 ? std::string("") : stack_[stack_.size()-3];

    bool is_img_thumnail_elem = pparent == "image" && parent == "thumbnail";

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
        if (version.isEmpty() || version != "2.0") {
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
        model_->insertRow(model_->rowCount());
        QModelIndex idx = model_->index(model_->rowCount(),0);

        model_->setData(idx, current_text_, Qt::ToolTipRole);
        model_->setData(idx, current_text_, Qt::UserRole);

        current_preview_.clear();
        current_news_ = News();
        rss_build_date_.clear();
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
            QLocale loc(rss_.language());
            current_news_.set_date(loc.toDateTime(current_text_));
        }
    } else if ( name == "description" ) {
        if (is_item_elem) {
            if (current_text_.length() >= 300 )
                current_preview_ = current_text_.left(300)+"...";  // todo (edgard.lima): use a better cut
            else
                current_preview_ = current_text_;
            current_news_.set_description(current_text_);
        } else if (is_channel_elem) {
            rss_.set_description(current_text_);
        }
    } else if ( name == "lastBuildDate" ) {
        if (is_channel_elem) {
            rss_build_date_ = current_text_;
            QLocale loc(rss_.language());
            rss_.set_date(loc.toDateTime(rss_build_date_));
        }
    } else if (name == "language") {
        if (is_channel_elem) {
            rss_.setLanguage(current_text_);
            if (!rss_build_date_.isEmpty()) {
                QLocale loc(rss_.language());
                rss_.set_date(loc.toDateTime(rss_build_date_));
            }
        }
    } else if (name == "url") {
        if (is_channel_img_elem) {
            rss_.set_image_link(current_text_);
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
