#ifndef RSS_H
#define RSS_H

#include <QString>
#include <QUrl>
#include <QDateTime>

#include <cstdint>

class Rss
{
public:
    Rss();
    Rss(const Rss & rss);
    Rss(Rss && rss);
    Rss(const QString & title,
        const QString & description,
        const QUrl & link,
        const QUrl & image_link=QUrl(),
        const QDateTime & date=QDateTime(),
        const QString & language = "en",
        int32_t ttl=60);

    Rss & operator=(const Rss & rss);
    Rss & operator=(Rss && rss);

    QString title() const;
    void set_title(const QString &title);

    QString description() const;
    void set_description(const QString &description);

    QUrl link() const;
    void set_link(const QUrl &link);

    QUrl image_link() const;
    void set_image_link(const QUrl &image_link);

    QDateTime date() const;
    void set_date(const QDateTime &date);

    QString language() const;
    void setLanguage(const QString &language);

    int32_t ttl() const;
    void set_ttl(const int32_t &ttl);

private:
    QString title_;
    QString description_;
    QUrl link_;
    QUrl image_link_;
    QDateTime date_;
    QString language_;
    int32_t ttl_;
};

#endif // RSS_H
