#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>

#include "rssreader.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void load_rss();
    void rss_read();
    void open_news(const QModelIndex& index);

private:
    Ui::MainWindow *ui;
    QTimer timer_;
    RssReader rss_reader_;
};

#endif // MAINWINDOW_H
