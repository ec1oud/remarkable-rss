#ifndef NEWS_H
#define NEWS_H

#include <QString>
#include <QUrl>
#include <QDateTime>

class News
{
public:
    News();
    News(const News & news);
    News(News && news);
    News(const QString & title,
        const QString & description,
        const QUrl & link,
        const QUrl & image_link=QUrl(),
        const QDateTime & date=QDateTime());

    News & operator=(const News & news);
    News & operator=(News && news);

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

private:
    QString title_;
    QString description_;
    QUrl link_;
    QUrl image_link_;
    QDateTime date_;
};

#endif // NEWS_H
