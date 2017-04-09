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

void MainWindow::getInfo()
{
    QString urlStr = "https://api.tjournal.ru/2.3/club";

    HttpRequestInput input(urlStr, "GET");

   // HttpRequestWorker *worker = new HttpRequestWorker(this);
    connect(worker, SIGNAL(executionFinished(HttpRequestWorker*)), this, SLOT(handleResult(HttpRequestWorker*)));

    worker->execute(&input);
}

void MainWindow::postAutenticate()
{
   api->verifyQR(ui->plainTextEdit->toPlainText());
   connect(api, SIGNAL(responseIsHere(QString)), this, SLOT(handle_autentification(QString)));
}

void MainWindow::getUserInfo()
{
    QString urlStr = "https://api.tjournal.ru/2.3/account/info";

    HttpRequestInput input(urlStr, "GET");
    //QString authorizationValueString = QString("Bearer %1").arg(token);
    QByteArray authorizationValue;
    authorizationValue.append(token);

    input.addHeader("X-Auth-Session", authorizationValue);

   // HttpRequestWorker *worker = new HttpRequestWorker(this);
    connect(worker, SIGNAL(executionFinished(HttpRequestWorker*)), this, SLOT(handleResult(HttpRequestWorker*)));

    worker->execute(&input);
}

void MainWindow::handleResult(HttpRequestWorker * worker_)
{
    worker_->deleteLater();
    if (worker_->errorType != QNetworkReply::NoError) {
        qDebug() << worker_->errorStr;
        updateText(worker_->errorStr);
        //delete worker_;
        return;
    }
    QString result = worker_->response;
    worker_->deleteLater();
    //delete worker_;

    qDebug() << result;
    updateText(result);
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
    getInfo();
}

void MainWindow::on_pushButton_2_clicked()
{
    postAutenticate();
}

void MainWindow::on_pushButton_3_clicked()
{
    getUserInfo();
}
