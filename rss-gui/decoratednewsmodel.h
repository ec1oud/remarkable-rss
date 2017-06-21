#ifndef DECORATEDNEWSMODEL_H
#define DECORATEDNEWSMODEL_H

#include <QSortFilterProxyModel>
#include <QString>
#include <memory>
#include <unordered_map>

class NewsModel;
class QPixmap;
class ImageCache;

class DecoratedNewsModel : public QSortFilterProxyModel
{
public:
    DecoratedNewsModel(QObject * parent = NULL);
    virtual ~DecoratedNewsModel();

    QVariant data(const QModelIndex & index, int role) const override;
    void setSourceModel(QAbstractItemModel * source_model) override;

    NewsModel * news_model();

public slots:
    void load_images();

private slots:
    void image_loaded(QList<int> rows);

private:
    ImageCache * image_cache_;
};

#endif // DECORATEDNEWSMODEL_H
