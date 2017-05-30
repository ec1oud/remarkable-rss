#ifndef RSSHANDLER_H
#define RSSHANDLER_H

#include <vector>
#include <string>
#include <QString>
#include <QModelIndex>

#include "rss.h"
#include "news.h"

class QXmlStreamReader;
class QXmlStreamAttributes;
class QXmlParseException;
class DecoratedNewsModel;

class RssHandler
{
public:
    RssHandler(QXmlStreamReader & reader);
    void set_model(DecoratedNewsModel * model);

    bool start_element(const QStringRef & ns_uri,
                      const QStringRef & local_name, const QStringRef & name,
                      const QXmlStreamAttributes &attributes);
    bool end_element(const QStringRef & ns_uri,
                    const QStringRef &local_name, const QStringRef & name);
    bool characters(const QStringRef &str);
    bool fatal_error(const QXmlParseException &exception);
    QString errorString() const;

    void clear();

    Rss rss() const;
    void setRss(const Rss &rss);

private:
    QXmlStreamReader & reader_;
    QString err_string_;
    QString current_text_;
    QString rss_build_date_;
    Rss rss_;
    News current_news_;
    QString current_preview_;
    std::vector<std::string> stack_;
    DecoratedNewsModel * model_;
};

#endif // RSSHANDLER_H
