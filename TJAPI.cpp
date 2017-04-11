#include "TJAPI.h"
TJAPI::TJAPI()
{
    apiurl=QString("https://%1/%2/").arg(apihost, apiversion);
}
HttpRequestInput TJAPI::createRequest(QString url, QString method)
{
    QString urlStr = this->apiurl+url;
    HttpRequestInput input(urlStr, method);
    if(!token.isEmpty())
    {
        QByteArray authorizationValue;
        authorizationValue.append(token);
        input.addHeader("X-Auth-Session", authorizationValue);
    }
    return input;
}

void TJAPI::verifyQR(QString QRcode)
{
    worker = new HttpRequestWorker(this);
    HttpRequestInput input = this->createRequest("account/verifyQR", "POST");
    if(QRcode.size())
    {
        QString QRwithsalt=QRcode+this->salt;
        QString QRhash = QString(QCryptographicHash::hash((QRwithsalt.toLocal8Bit()),QCryptographicHash::Md5).toHex());
        qDebug()<<QRhash;
        input.addVar("hash", QRhash);
        input.addVar("token", QRcode);
        connect(worker, SIGNAL(executionFinished(HttpRequestWorker*)), this,
                SIGNAL(verifyQRFinished(HttpRequestWorker*)));
        worker->execute(&input);
    }
}
void TJAPI::getUserInfo()
{
    worker = new HttpRequestWorker(this);
    HttpRequestInput input = this->createRequest("account/info", "GET");
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
    QString urlStr = apiurl+"club";
    HttpRequestInput input(urlStr, "GET");
    connect(worker, SIGNAL(executionFinished(HttpRequestWorker*)), this,
            SIGNAL(getInfoExecutionFinished(HttpRequestWorker*)));
    worker->execute(&input);
}
void TJAPI::getAccountPosts(int account)
{
}
