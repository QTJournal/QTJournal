#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtWidgets/QDesktopWidget>
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>
#include <QtCore/QJsonValue>

#include <QFileDialog>

#include <QStringListModel>
#include <QDebug>

#include <QStandardItemModel>

#include <QHBoxLayout>
#include <QVBoxLayout>

#include <QListWidgetItem>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("SUPER TJ Client");

    //api = new TJAPI();
    //connect(api, SIGNAL(responseIsHere(QString)), this, SLOT(updateText(QString)));
    //connect(api,SIGNAL(updateTextSignal(QString)),this,SLOT(updateText(QString)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateText(const QString& text_)
{
    ui->textEdit->append(QString("\r%1").arg(text_));
}

void MainWindow::updatePostsList(QList<Post *> *posts)
{
    QStringList postsNames;

    for (int i = 0; i < posts->size(); i++) {
        postsNames << posts->at(i)->getTitle();

        QLabel* postTitle = new QLabel(posts->at(i)->getTitle());
        postTitle->setWordWrap(true);
        QLabel* postAuthor = new QLabel(posts->at(i)->getAuthor()->getName());

        QVBoxLayout* layout = new QVBoxLayout();
        QHBoxLayout* layout2 = new QHBoxLayout();

        QListWidgetItem* item = new QListWidgetItem();

        QWidget* widget = new QWidget();
        QWidget* widget2 = new QWidget();

        layout->addWidget(postAuthor);
        layout->addWidget(postTitle);
        layout->addWidget(widget2);

        widget->setLayout(layout);
        widget2->setLayout(layout2);

        item->setSizeHint(QSize(item->sizeHint().height(),ui->listWidget->sizeHintForRow(0)));
        layout->setSizeConstraint(QLayout::SetMinimumSize);

        ui->listWidget->addItem(item);
        ui->listWidget->setItemWidget(item,widget);

        posts->at(i)->deleteLater();
    }

    delete posts;
}

void MainWindow::updateQRString(const QString& str_)
{
    ui->plainTextEdit->appendPlainText(str_);
}
void MainWindow::on_textEdit_destroyed()
{

}

void MainWindow::on_pushButton_clicked()
{
    //api->getInfo();
    emit getInfoButtonClicked();
}

void MainWindow::on_pushButton_2_clicked()
{
    //api->verifyQR(ui->plainTextEdit->toPlainText());
    emit verifyQRButtonClicked(ui->plainTextEdit->toPlainText());
}

void MainWindow::on_pushButton_3_clicked()
{
    //api->getUserInfo();
    emit getUserInfoButtonClicked();
}
void MainWindow::on_pushButton_4_clicked()
{
    QFileDialog dialog(this);
    QStringList FileNames;
    dialog.setFileMode(QFileDialog::AnyFile);
    dialog.setNameFilter(tr("Images (*.png *.xpm *.jpg)"));
    if (dialog.exec()){
        FileNames = dialog.selectedFiles();
    }
    emit QRtoDecode(FileNames);
}
