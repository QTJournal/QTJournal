#include "TJAPI.h"
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
        connect(worker, SIGNAL(executionFinished(HttpRequestWorker*)), this, SIGNAL(verifyQRFinished(HttpRequestWorker*)));
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
    //connect(worker, SIGNAL(executionFinished(HttpRequestWorker*)), this, SLOT(handleResult(HttpRequestWorker*)));
    connect(worker, SIGNAL(executionFinished(HttpRequestWorker*)), this,
            SIGNAL(getUserInfoExecutionFinished(HttpRequestWorker*)));
    worker->execute(&input);
}
void TJAPI::setToken(QByteArray Gtoken)
{
    this->token=Gtoken;
}
QByteArray TJAPI::getToken()
{
    return this->token;
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
    //connect(worker, SIGNAL(executionFinished(HttpRequestWorker*)), this, SLOT(handleResult(HttpRequestWorker*)));
    connect(worker, SIGNAL(executionFinished(HttpRequestWorker*)), this,
            SIGNAL(getInfoExecutionFinished(HttpRequestWorker*)));
    worker->execute(&input);
}
