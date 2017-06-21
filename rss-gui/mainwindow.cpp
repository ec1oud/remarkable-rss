#include <QMessageBox>
#include <QUrl>
#include <QDialog>
#include <QDebug>
#include <QTimer>
#include <QDesktopServices>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "decoratednewsmodel.h"
#include "newsmodel.h"
#include "util.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->imageLabel->hide();

    connect(ui->urlEdit, &QLineEdit::returnPressed, this, &MainWindow::load_rss);
    connect(ui->goButton, &QPushButton::clicked, this, &MainWindow::load_rss);

    timer_.setInterval(1000);
    connect(&timer_, &QTimer::timeout, [this] {qDebug() << "timer"; });
    timer_.start();

    auto news_model = new NewsModel(this);

    auto decor_model = new DecoratedNewsModel(this);
    decor_model->setSourceModel(news_model);
    decor_model->setSortRole(NewsModel::Roles::kDate);
    // decor_model->setDynamicSortFilter(true);

    QItemSelectionModel* news_selection_model = new QItemSelectionModel(decor_model, this);

    rss_reader_.set_model(news_model);
    connect(&rss_reader_, &RssReader::finished, this, &MainWindow::rss_read);
    connect(&rss_reader_, &RssReader::finished, decor_model, &DecoratedNewsModel::load_images);

    ui->newsListView->setModel(decor_model);
    ui->newsListView->setSelectionModel(news_selection_model);

    ui->newsListView->setSpacing(5);
    ui->newsListView->setResizeMode(QListView::Adjust);
    ui->newsListView->setFlow(QListView::TopToBottom);

    //ui->newsListView->setWrapping(true);

    connect(ui->newsListView, &QListView::doubleClicked, this, &MainWindow::open_news);

    ui->urlEdit->setText("http://feeds.bbci.co.uk/news/rss.xml?edition=uk");
}

MainWindow::~MainWindow()
{
    timer_.stop();
    delete ui;
}

void MainWindow::load_rss() {
    qDebug() << "loading RSS from '" << ui->urlEdit->text() << "'.";

    QUrl url = QUrl::fromUserInput(ui->urlEdit->text());
    // QUrl url("http://feeds.bbci.co.uk/news/rss.xml?edition=uk");
    //QUrl url("http://revistaepoca.globo.com/Revista/Epoca/Rss/0,,DMI0-15210,00.xml");

    if (!url.isValid()) {
        qWarning() << "Invalid URL '" << ui->urlEdit->text() << "'.";
        Util::show_errror("Error", "Invalid URL.");
        // ui->urlEdit->undo();
        return;
    }

    rss_reader_.load(url);
}

void MainWindow::rss_read() {
    auto rss = rss_reader_.rss();
    ui->titleLabel->setText(rss.title());
    ui->descriptionLabel->setText(rss.description());
    // ui->newsListView->model()->sort(-1, Qt::DescendingOrder);
}

void MainWindow::open_news(const QModelIndex& index) {
    auto link = index.model()->data(index, NewsModel::Roles::kLink).toUrl();
    bool ok = QDesktopServices::openUrl(link);
    if (!ok) {
        Util::show_errror("Error", "Failed to open external browser.");
    }
}
