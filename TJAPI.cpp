#include "TJAPI.h"
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>
#include <QtCore/QJsonValue>
TJAPI::TJAPI()
{
    worker = new HttpRequestWorker(this);
}
void TJAPI::verifyQR(QString QRCode)
{
    QString urlStr = "https://api.tjournal.ru/2.3/account/verifyQR";

    HttpRequestInput input(urlStr, "POST");
    if(QRCode.size())
    {
        QString fknhash=QRCode+"hDv#L9Om>iHfAdT5^6uIy?&";
        QString encodedPass = QString(QCryptographicHash::hash((fknhash.toLocal8Bit()),QCryptographicHash::Md5).toHex());
        qDebug()<<encodedPass;
        input.addVar("hash", encodedPass);
        input.addVar("token", QRCode);
        // HttpRequestWorker *worker = new HttpRequestWorker(this);
        connect(worker, SIGNAL(executionFinished(HttpRequestWorker*)), this, SLOT(handleAuth(HttpRequestWorker*)));
        worker->execute(&input);
    }
}
void TJAPI::handleAuth(HttpRequestWorker* worker_)
{
    worker_->deleteLater();
    QByteArray result = worker_->response;

    QJsonDocument responseJson = QJsonDocument::fromJson(result/*, QJsonParseError *error = Q_NULLPTR*/); //TODO add error check
    QJsonObject responseJsonObject = responseJson.object();
    QJsonValue tokenJsonValue = responseJsonObject.value(QString("sessionId"));
    qDebug()<<result;
    if (tokenJsonValue.type() == QJsonValue::Undefined) {
        //updateText("there is no token in response");
        emit responseIsHere("there is no token in response");
        //delete worker_;
        return;
    }

    token = tokenJsonValue.toString().toLatin1();
    if (token.isNull()) {
        emit responseIsHere("token contains illegal non-Latin1 characters");
        //delete worker_;
        return;
    }

    //delete worker_;

    qDebug() << token;
   // updateText(result);
    emit responseIsHere(result);
}
