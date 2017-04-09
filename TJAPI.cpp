#include "TJAPI.h"
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>
#include <QtCore/QJsonValue>
TJAPI::TJAPI()
{
}
void TJAPI::verifyQR(QString QRCode)
{
    worker = new HttpRequestWorker(this);
    QString urlStr = "https://api.tjournal.ru/2.3/account/verifyQR";

    HttpRequestInput input(urlStr, "POST");
    if(QRCode.size())
    {
        QString fknhash=QRCode+"hDv#L9Om>iHfAdT5^6uIy?&";
        QString encodedPass = QString(QCryptographicHash::hash((fknhash.toLocal8Bit()),QCryptographicHash::Md5).toHex());
        qDebug()<<encodedPass;
        input.addVar("hash", encodedPass);
        input.addVar("token", QRCode);
        connect(worker, SIGNAL(executionFinished(HttpRequestWorker*)), this, SLOT(handleAuth(HttpRequestWorker*)));
        worker->execute(&input);
    }
}
void TJAPI::getUserInfo()
{
    worker = new HttpRequestWorker(this);
    QString urlStr = "https://api.tjournal.ru/2.3/account/info";
    HttpRequestInput input(urlStr, "GET");
    QByteArray authorizationValue;
    authorizationValue.append(token);
    input.addHeader("X-Auth-Session", authorizationValue);
    connect(worker, SIGNAL(executionFinished(HttpRequestWorker*)), this, SLOT(handleResult(HttpRequestWorker*)));
    worker->execute(&input);
}
void TJAPI::handleAuth(HttpRequestWorker* worker_)
{

    QByteArray result = worker_->response;
    QJsonDocument responseJson = QJsonDocument::fromJson(result/*, QJsonParseError *error = Q_NULLPTR*/); //TODO add error check
    QJsonObject responseJsonObject = responseJson.object();
    QJsonValue tokenJsonValue = responseJsonObject.value(QString("sessionId"));
    qDebug()<<result;

    if (tokenJsonValue.type() == QJsonValue::Undefined) {
        //updateText("there is no token in response");
        emit responseIsHere("there is no token in response");
        worker_->deleteLater();
        return;
    }

    token = tokenJsonValue.toString().toLatin1();
    if (token.isNull()) {
        emit responseIsHere("token contains illegal non-Latin1 characters");
        worker_->deleteLater();
        return;
    }

    worker_->deleteLater();

    qDebug() << token;
    emit responseIsHere(result);
}
void TJAPI::handleResult(HttpRequestWorker * worker_)
{

    if (worker_->errorType != QNetworkReply::NoError) {
        qDebug() << worker_->errorStr;
        emit updateTextSignal(worker_->errorStr);
        worker_->deleteLater();
        return;
    }
    QString result = worker_->response;
    worker_->deleteLater();

    qDebug() << result;
    emit updateTextSignal(result);
}
void TJAPI::getInfo()
{
   worker = new HttpRequestWorker(this);
    QString urlStr = "https://api.tjournal.ru/2.3/club";

    HttpRequestInput input(urlStr, "GET");
    connect(worker, SIGNAL(executionFinished(HttpRequestWorker*)), this, SLOT(handleResult(HttpRequestWorker*)));
    worker->execute(&input);
}
