#include <QPixmap>

#include "decoratednewsmodel.h"
#include "newsmodel.h"
#include "imagecache.h"

DecoratedNewsModel::DecoratedNewsModel(QObject * parent)
    : QSortFilterProxyModel(parent)
    , image_cache_(new ImageCache(this, this))
{
    connect(image_cache_, &ImageCache::image_loaded, this, &DecoratedNewsModel::image_loaded);
}

DecoratedNewsModel::~DecoratedNewsModel()
{
}

QVariant DecoratedNewsModel::data(const QModelIndex & idx, int role) const {

    auto index = mapToSource(idx);

    if (role == Qt::ToolTipRole) {
        auto date = sourceModel()->data(index, NewsModel::Roles::kDate).toDateTime().toLocalTime().toString(Qt::DefaultLocaleShortDate);
        auto desc = sourceModel()->data(index, NewsModel::Roles::kDescription).toString();

        auto r = date + " - " + desc;

        return r;
    } else if (role == Qt::DecorationRole) {
        auto img_url = sourceModel()->data(index, NewsModel::Roles::kImageLink).toUrl();
        auto image = (*image_cache_)[img_url];
        if (!image.isNull()) {
            return image;
        }
        return {};
    }
    return QSortFilterProxyModel::data(idx, role);
}

void DecoratedNewsModel::setSourceModel(QAbstractItemModel * source_model) {
    QSortFilterProxyModel::setSourceModel(source_model);
    if (!source_model) return;

}

NewsModel * DecoratedNewsModel::news_model() {
    return static_cast<NewsModel *>(sourceModel());
}

void DecoratedNewsModel::load_images()
{
    image_cache_->load_images();
}

void DecoratedNewsModel::image_loaded(QList<int> rows)
{
    for (auto row : rows) {
        QVector<int> roles = { Qt::DecorationRole};
        emit dataChanged(index(row, 0), index(row, 0), roles);
    }
}
