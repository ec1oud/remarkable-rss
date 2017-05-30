#ifndef NEWSMODEL_H
#define NEWSMODEL_H

#include <string>
#include <memory>
#include <vector>

#include <QAbstractListModel>

#include "news.h"

class Album;
class DatabaseManager;
class AlbumModel;

class NewsModel : public QAbstractListModel
{
    Q_OBJECT
public:

    enum Roles {
        kTitle = Qt::UserRole + 1,
        kDescription,
        kLink,
        kImageLink,
        kDate
    };

    NewsModel(QObject* parent = 0);

    QModelIndex add_news(const News& news);

    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role) const override;
    bool removeRows(int row, int count, const QModelIndex& parent) override;
    virtual QHash<int, QByteArray> roleNames() const override;

private:
    void loadNewss(int albumId);
    bool isIndexValid(const QModelIndex& index) const;

private:
    std::vector<News> news_;
};

#endif // NEWSMODEL_H
