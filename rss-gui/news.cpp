#include "news.h"

News::News()
{
}

News::News(const News & news)
    : News(news.title_,
          news.description_,
          news.link_,
          news.image_link_,
          news.date_)
{
}

News::News(News && news)
    : title_(std::move(news.title_))
    , description_(std::move(news.description_))
    , link_(std::move(news.link_))
    , image_link_(std::move(news.image_link_))
    , date_(std::move(news.date_))
{
}

News::News(const QString & title,
         const QString & description,
         const QUrl & link,
         const QUrl & image_link,
         const QDateTime & date)
    : title_(title)
    , description_(description)
    , link_(link)
    , image_link_(image_link)
    , date_(date)
{
}

News & News::operator=(const News & news) {
    *this = std::move(News(news));
    return *this;
}

News & News::operator=(News && news) {
    title_ = std::move(news.title_);
    description_ = std::move(news.description_);
    link_ = std::move(news.link_);
    image_link_ = std::move(news.image_link_);
    date_ = std::move(news.date_);
    return *this;
}


QString News::title() const
{
    return title_;
}

void News::set_title(const QString &title)
{
    title_ = title;
}

QString News::description() const
{
    return description_;
}

void News::set_description(const QString &description)
{
    description_ = description;
}

QUrl News::link() const
{
    return link_;
}

void News::set_link(const QUrl &link)
{
    link_ = link;
}

QUrl News::image_link() const
{
    return image_link_;
}

void News::set_image_link(const QUrl &image_link)
{
    image_link_ = image_link;
}

QDateTime News::date() const
{
    return date_;
}

void News::set_date(const QDateTime &date)
{
    date_ = date;
}
