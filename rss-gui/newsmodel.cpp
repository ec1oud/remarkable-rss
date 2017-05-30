#include "newsmodel.h"

NewsModel::NewsModel(QObject* parent)
    : QAbstractListModel(parent)
{
}

QModelIndex NewsModel::add_news(const News& news) {
    int rows = rowCount();

    beginInsertRows(QModelIndex(), rows, rows);
    news_.push_back(news);
    endInsertRows();

    return index(rows);
}

int NewsModel::rowCount(const QModelIndex& /* parent */) const {
    return news_.size();
}

QVariant NewsModel::data(const QModelIndex& index, int role) const {
    if (!isIndexValid(index)) {
        return {};
    }

    const auto & news = news_[index.row()];

    switch (role) {
    case Qt::DisplayRole:
    case Roles::kTitle:
        return news.title();
        break;
    case Qt::ToolTipRole:
    case Roles::kDescription:
        return news.description();
        break;
    case Roles::kLink:
        return news.link();
        break;
    case Roles::kImageLink:
        return news.image_link();
        break;
    case Roles::kDate:
        return news.date();
        break;
    default:
        return {};
        break;
    }
}

bool NewsModel::removeRows(int row, int count, const QModelIndex& parent) {
    if (row < 0 || row >= rowCount() || count <= 0 || (row + count) > rowCount()) {
        return false;
    }

    beginRemoveRows(parent, row, row + count - 1);
    news_.erase(news_.begin() + row, news_.begin() + row + count);
    endRemoveRows();

    return true;
}

QHash<int, QByteArray> NewsModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[Roles::kTitle] = "title";
    roles[Roles::kDescription] = "description";
    roles[Roles::kLink] = "link";
    roles[Roles::kImageLink] = "image";
    roles[Roles::kDate] = "date";

    return roles;
}

bool NewsModel::isIndexValid(const QModelIndex& index) const {
    return index.isValid() && index.row() >= 0 && index.row() < rowCount();
}
