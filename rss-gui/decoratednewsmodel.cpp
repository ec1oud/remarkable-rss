#include <QPixmap>

#include "decoratednewsmodel.h"

#include "newsmodel.h"

DecoratedNewsModel::DecoratedNewsModel(QObject * parent)
    : QSortFilterProxyModel(parent)
{

}

QVariant DecoratedNewsModel::data(const QModelIndex & index, int role) const {
    if (role != Qt::DecorationRole) {
        return QSortFilterProxyModel::data(index, role);
    }
    auto img_url = sourceModel()->data(index, NewsModel::Roles::kImageLink).toString();
    auto it = thumbnails_.find(img_url.toStdString());
    if (it != thumbnails_.end()) {
        return *it->second;
    }
    return {};
}

void DecoratedNewsModel::setSourceModel(QAbstractItemModel * source_model) {
    QSortFilterProxyModel::setSourceModel(source_model);
    if (!source_model) return;

    connect(source_model, &QAbstractItemModel::modelReset, [this] () {
        reload_thumnails();
    } );

    connect(source_model, &QAbstractItemModel::rowsInserted,
            [this] (const QModelIndex& /* parent */, int first, int last) {
        generate_thumbnails(index(first, 0), last - first + 1);
    });
}

NewsModel * DecoratedNewsModel::news_model() {
    return static_cast<NewsModel *>(sourceModel());
}

void DecoratedNewsModel::generate_thumbnails(const QModelIndex & start_index, int count) {
    if (!start_index.isValid()) return;

    auto model = start_index.model();

    int last_index = start_index.row() + count;
    for (auto row = start_index.row(); row < last_index; row++) {
        auto filepath = model->data(model->index(row, 0), NewsModel::Roles::kImageLink).toString();

        //QPixmap pixmap(filepath);
        //auto thumbnail = new QPixmap(pixmap
        //                             .scaled(THUMBNAIL_SIZE, THUMBNAIL_SIZE,
        //                                     Qt::KeepAspectRatio,
        //                                     Qt::SmoothTransformation));
        //mThumbnails.insert(filepath, thumbnail);
    }
}

void DecoratedNewsModel::reload_thumnails() {
    thumbnails_.clear();
    generate_thumbnails(index(0,0), rowCount());
}
