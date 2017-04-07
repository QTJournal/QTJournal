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
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::getInfo()
{
    QString urlStr = "https://api.tjournal.ru/2.3/club";

    HttpRequestInput input(urlStr, "GET");

    HttpRequestWorker *worker = new HttpRequestWorker(this);
    connect(worker, SIGNAL(executionFinished(HttpRequestWorker*)), this, SLOT(handleResult(HttpRequestWorker*)));

    worker->execute(&input);
}

void MainWindow::postAutenticate()
{
    QString urlStr = "https://api.tjournal.ru/2.3/account/verifyQR";

    HttpRequestInput input(urlStr, "POST");
    QString tokenQR = ui->plainTextEdit->toPlainText();
    if(tokenQR.size())
    {
        QString fknhash=tokenQR+"hDv#L9Om>iHfAdT5^6uIy?&";
        QString encodedPass = QString(QCryptographicHash::hash((fknhash.toLocal8Bit()),QCryptographicHash::Md5).toHex());
        qDebug()<<encodedPass;
        input.addVar("hash", encodedPass);
        input.addVar("token", tokenQR);
         HttpRequestWorker *worker = new HttpRequestWorker(this);
        connect(worker, SIGNAL(executionFinished(HttpRequestWorker*)), this, SLOT(handle_autentification(HttpRequestWorker*)));
        worker->execute(&input);
    }
}

void MainWindow::getUserInfo()
{
    QString urlStr = "https://api.tjournal.ru/2.3/account/info";

    HttpRequestInput input(urlStr, "GET");
    //QString authorizationValueString = QString("Bearer %1").arg(token);
    QByteArray authorizationValue;
    authorizationValue.append(token);

    input.addHeader("X-Auth-Session", authorizationValue);

    HttpRequestWorker *worker = new HttpRequestWorker(this);
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

void MainWindow::handle_autentification(HttpRequestWorker *worker_)
{
    worker_->deleteLater();
    QByteArray result = worker_->response;

    QJsonDocument responseJson = QJsonDocument::fromJson(result/*, QJsonParseError *error = Q_NULLPTR*/); //TODO add error check
    QJsonObject responseJsonObject = responseJson.object();
    QJsonValue tokenJsonValue = responseJsonObject.value(QString("sessionId"));
    qDebug()<<result;
    if (tokenJsonValue.type() == QJsonValue::Undefined) {
        updateText("there is no token in response");
        //delete worker_;
        return;
    }

    token = tokenJsonValue.toString().toLatin1();
    if (token.isNull()) {
        updateText("token contains illegal non-Latin1 characters");
        //delete worker_;
        return;
    }

    //delete worker_;

    qDebug() << token;
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
