#ifndef IMAGECACHE_H
#define IMAGECACHE_H

#include <unordered_map>

#include <QObject>
#include <QImage>
#include <QPixmap>
#include <QList>
#include <QUrl>
#include <QHash>
#include <QtNetwork/QNetworkAccessManager>

class QNetworkReply;

class QAbstractItemModel;

class ImageCache : public QObject
{
    Q_OBJECT
public:
    ImageCache(QAbstractItemModel * model, QObject* parent = nullptr);
    ~ImageCache();

    const QPixmap & operator[](const QUrl & url);

public slots:
    void load_images();

private slots:
    void reply_finished(QNetworkReply* reply);

signals:
    void image_loaded(QList<int>);

private:
    QNetworkAccessManager manager_;
    QHash <QUrl, QPixmap> images_;  // TODO (edgard.lima): Move to /tmp
    QHash <QUrl, QNetworkReply *> replies_;
    QHash <QUrl, QList<int>*> rows_;

    QAbstractItemModel * model_;

    void remove_downloading(const QUrl & url);
};

#endif // IMAGECACHE_H
