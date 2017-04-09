#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtWidgets/QDesktopWidget>
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>
#include <QtCore/QJsonValue>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("SUPER TJ Client");
    worker = new HttpRequestWorker(this);
    api = new TJAPI();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::postAutenticate()
{
   api->verifyQR(ui->plainTextEdit->toPlainText());
   connect(api, SIGNAL(responseIsHere(QString)), this, SLOT(handle_autentification(QString)));
}

void MainWindow::handle_autentification(QString result)
{
    updateText(result);
}
void MainWindow::updateText(const QString& text_)
{
     ui->textEdit->append(QString("\r%1").arg(text_));
}
void MainWindow::on_textEdit_destroyed()
{

}

void MainWindow::on_pushButton_clicked()
{
    api->getInfo();
}

void MainWindow::on_pushButton_2_clicked()
{
    postAutenticate();
}

void MainWindow::on_pushButton_3_clicked()
{
    api->getUserInfo();
}
