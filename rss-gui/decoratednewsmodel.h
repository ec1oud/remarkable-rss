#ifndef DECORATEDNEWSMODEL_H
#define DECORATEDNEWSMODEL_H

#include <QSortFilterProxyModel>
#include <QString>
#include <memory>
#include <unordered_map>

class NewsModel;
class QPixmap;

class DecoratedNewsModel : public QSortFilterProxyModel
{
public:
    DecoratedNewsModel(QObject * parent = NULL);

    QVariant data(const QModelIndex & index, int role) const override;
    void setSourceModel(QAbstractItemModel * source_model) override;

    NewsModel * news_model();

private:
    void generate_thumbnails(const QModelIndex & start_index, int count);

    void reload_thumnails();

private:
    std::unordered_map<std::string, std::unique_ptr<QPixmap>> thumbnails_;
};

#endif // DECORATEDNEWSMODEL_H
