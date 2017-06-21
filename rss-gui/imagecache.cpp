#include <QtNetwork/QNetworkReply>

#include "imagecache.h"

#include "newsmodel.h"
#include "util.h"

const unsigned int THUMBNAIL_SIZE = 64;

ImageCache::ImageCache(QAbstractItemModel * model, QObject* parent)
    : QObject(parent)
    , model_(model)
{
    connect(&manager_, &QNetworkAccessManager::finished, this, &ImageCache::reply_finished);
}

ImageCache::~ImageCache() {
    for (auto it = replies_.begin(); it != replies_.end(); it++) {
        it.value()->abort();
        it.value()->deleteLater();
        replies_[it.key()] = nullptr;
    }
    qDeleteAll(rows_);
    rows_.clear();
}

const QPixmap &ImageCache::operator[](const QUrl &url)
{
    return images_[url];
}

void ImageCache::load_images()
{
    QHash <QUrl, bool> images;
    QList <QUrl> to_remove;
    qDeleteAll(rows_);
    rows_.clear();

    // load list of current images

    for (auto row = 0 ; row < model_->rowCount(); row++) {
        auto url = model_->data(model_->index(row, 0), NewsModel::Roles::kImageLink).toUrl();
        images.insert(url, true);
        if (rows_[url] == nullptr) {
            rows_[url] = new QList<int>;
        }
        rows_[url]->append(row);
    }

    // list images to be removed

    for (auto it_url = images_.keyBegin(); it_url != images_.keyEnd(); it_url++) {
        if (! images.contains(*it_url)) {
            to_remove.insert(0, *it_url);
        }
    }

    // remove image not present anymore

    for (auto url : to_remove) {
        images_.remove(url);
        remove_downloading(url);
    }

    // insert new images

    for (auto it_url = images.keyBegin(); it_url != images.keyEnd(); it_url++) {
        if (!images_.contains(*it_url)) {
            images_.insert(*it_url, QPixmap());
        }
    }

    // queue images to download

    for (auto it=images_.begin(); it != images_.end(); it++) {
        if (it->isNull()) {  // QImage is null
            auto itr = replies_.find(it.key());
            auto need_download = false;
            if (itr == replies_.end()) {
                need_download = true;
            } else if (itr.value() != nullptr && itr.value()->isFinished()) {
                itr.value()->deleteLater();
                need_download = true;
            }
            if (need_download) {
                replies_.insert(it.key(), manager_.get(QNetworkRequest(it.key())));
            }
        }
    }

}

void ImageCache::reply_finished(QNetworkReply *reply)
{
    if (reply->error() != QNetworkReply::NoError && reply->error() != QNetworkReply::OperationCanceledError) {
        qDebug() << "Image download error for '" << reply->request().url() << "' : " << reply->errorString();
    } else {
        qDebug() << "Image download '" << reply->request().url() << "'";

        auto image = QImage::fromData(reply->readAll());

        QPixmap pixmap = QPixmap::fromImage(image);
        images_[reply->url()] = QPixmap(pixmap
                                     .scaled(THUMBNAIL_SIZE, THUMBNAIL_SIZE,
                                             Qt::KeepAspectRatio,
                                             Qt::SmoothTransformation));
        if (images_[reply->url()].isNull()) {
            qDebug() << "null image for " << reply->url();
        }

        if (rows_[reply->url()] != nullptr) {
            emit image_loaded(*rows_[reply->url()]);
        }

    }

    remove_downloading(reply->request().url());

    // emit finished();

}

void ImageCache::remove_downloading(const QUrl &url)
{
    auto ptr = replies_[url];
    if (ptr != nullptr) {
        if (! ptr->isFinished()) ptr->abort();
        ptr->deleteLater();
        replies_[url] = nullptr;
    }
    replies_.remove(url);
}
