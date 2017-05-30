#include "rss.h"

Rss::Rss()
{
}

Rss::Rss(const Rss & rss)
    : Rss(rss.title_,
          rss.description_,
          rss.link_,
          rss.image_link_,
          rss.date_,
          rss.language_,
          rss.ttl_)
{
}

Rss::Rss(Rss && rss)
    : title_(std::move(rss.title_))
    , description_(std::move(rss.description_))
    , link_(std::move(rss.link_))
    , image_link_(std::move(rss.image_link_))
    , date_(std::move(rss.date_))
    , language_(std::move(rss.language_))
    , ttl_(std::move(rss.ttl_))
{
}

Rss::Rss(const QString & title,
         const QString & description,
         const QUrl & link,
         const QUrl & image_link,
         const QDateTime & date,
         const QString & language,
         int32_t ttl)
    : title_(title)
    , description_(description)
    , link_(link)
    , image_link_(image_link)
    , date_(date)
    , language_(language)
    , ttl_(ttl)
{
}

Rss & Rss::operator=(const Rss & rss) {
    *this = std::move(Rss(rss));
    return *this;
}

Rss & Rss::operator=(Rss && rss) {
    title_ = std::move(rss.title_);
    description_ = std::move(rss.description_);
    link_ = std::move(rss.link_);
    image_link_ = std::move(rss.image_link_);
    date_ = std::move(rss.date_);
    language_ = std::move(rss.language_);
    ttl_ = std::move(rss.ttl_);
    return *this;
}


QString Rss::title() const
{
    return title_;
}

void Rss::set_title(const QString &title)
{
    title_ = title;
}

QString Rss::description() const
{
    return description_;
}

void Rss::set_description(const QString &description)
{
    description_ = description;
}

QUrl Rss::link() const
{
    return link_;
}

void Rss::set_link(const QUrl &link)
{
    link_ = link;
}

QUrl Rss::image_link() const
{
    return image_link_;
}

void Rss::set_image_link(const QUrl &image_link)
{
    image_link_ = image_link;
}

QDateTime Rss::date() const
{
    return date_;
}

void Rss::set_date(const QDateTime &date)
{
    date_ = date;
}

QString Rss::language() const
{
    return language_;
}

void Rss::setLanguage(const QString &language)
{
    language_ = language;
}

int32_t Rss::ttl() const
{
    return ttl_;
}

void Rss::set_ttl(const int32_t &ttl)
{
    ttl_ = ttl;
}
