#include "controller.h"

Controller::Controller(MainWindow *mainWindow, QObject *parent) : QObject(parent)
{
    this->mainWindow = mainWindow;
    this->api = new TJAPI();

    connect(mainWindow, SIGNAL(getUserInfoButtonClicked()), this, SLOT(handleUserInfoButton()));
    connect(mainWindow, SIGNAL(getInfoButtonClicked()), this, SLOT(handleGetInfoButton()));
    connect(mainWindow, SIGNAL(verifyQRButtonClicked(QString)), this, SLOT(handleVerifyQRButton(QString)));
    connect(api, SIGNAL(getInfoExecutionFinished(HttpRequestWorker*)), this,
            SLOT(handleGetInfoResult(HttpRequestWorker*)));
    connect(this, SIGNAL(updateText(QString)), mainWindow, SLOT(updateText(QString)));
    connect(api, SIGNAL(getUserInfoExecutionFinished(HttpRequestWorker*)), this,
            SLOT(handleGetUserInfoResult(HttpRequestWorker*)));
    connect(api, SIGNAL(verifyQRFinished(HttpRequestWorker*)), this, SLOT(handleVerifyQRResult(HttpRequestWorker*)));
}

Controller::~Controller()
{
    delete api;
}

void Controller::handleUserInfoButton()
{
    qDebug() << "handleUserInfo()";
    api->getUserInfo();
}

void Controller::handleVerifyQRButton(QString QRCode)
{
    qDebug() << "handleVerifyQRButton(QString *)";
    api->verifyQR(QRCode);
}

void Controller::handleGetInfoButton()
{
    qDebug() << "handleGetInfoButton()";
    api->getInfo();
}

void Controller::handleGetInfoResult(HttpRequestWorker* worker)
{
    qDebug() << "handleGetInfoResult(HttpRequestWorker* worker)";
    if (worker->errorType != QNetworkReply::NoError) {
        qDebug() << worker->errorStr;
        emit updateText(worker->errorStr);
        worker->deleteLater();
        return;
    }
    QString result = worker->response;
    worker->deleteLater();

    emit updateText(result);
}

void Controller::handleGetUserInfoResult(HttpRequestWorker *worker)
{
    qDebug() << "handleGetUserInfoResult(HttpRequestWorker *worker)";
}

void Controller::handleVerifyQRResult(HttpRequestWorker* worker)
{
    qDebug() << "handleVerifyQRResult(HttpRequestWorker* worker)";
}
