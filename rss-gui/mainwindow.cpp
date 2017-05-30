#include <QMessageBox>
#include <QUrl>
#include <QDialog>
#include <QDebug>
#include <QTimer>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "decoratednewsmodel.h"
#include "newsmodel.h"

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

    rss_reader_.set_model(decor_model);
    //decor_model->setSortRole(NewsModel::Roles::kDate);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::load_rss() {
    qDebug() << "loading RSS from '" << ui->urlEdit->text() << "'.";

    //QUrl url = QUrl::fromUserInput(ui->urlEdit->text());
    QUrl url("http://feeds.bbci.co.uk/news/rss.xml?edition=uk");

    if (!url.isValid()) {
        qWarning() << "Invalid URL '" << ui->urlEdit->text() << "'.";
        show_errror("Error", "Invalid URL.");
        return;
    }

    rss_reader_.load(url);
}

void MainWindow::show_errror(const QString & title, const QString & message) {
    auto msg_box = new QMessageBox( this );
    msg_box->setIcon(QMessageBox::Critical);
    msg_box->setAttribute( Qt::WA_DeleteOnClose );
    msg_box->setStandardButtons( QMessageBox::Close );
    msg_box->setWindowTitle(title);
    msg_box->setText(message);
    // msg_box->setModal(false);
    msg_box->show();
}
