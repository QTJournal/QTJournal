#include "controller.h"
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>
#include <QtCore/QJsonValue>
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

void Controller::handleVerifyQRResult(HttpRequestWorker* worker)
{
    QByteArray result = worker->response;
    QJsonDocument responseJson = QJsonDocument::fromJson(result/*, QJsonParseError *error = Q_NULLPTR*/); //TODO add error check
    QJsonObject responseJsonObject = responseJson.object();
    QJsonValue tokenJsonValue = responseJsonObject.value(QString("sessionId"));
    qDebug()<<result;

    if (tokenJsonValue.type() == QJsonValue::Undefined) {
        //updateText("there is no token in response");
        emit updateText("there is no token in response");
        worker->deleteLater();
        return;
    }

    QByteArray token = tokenJsonValue.toString().toLatin1();
    if (token.isNull()) {
        emit updateText("token contains illegal non-Latin1 characters");
        worker->deleteLater();
        return;
    }

    worker->deleteLater();

    qDebug() << token;
    this->api->setToken(token);
    emit updateText(result);
    qDebug() << "handleVerifyQRResult(HttpRequestWorker* worker)";
}
