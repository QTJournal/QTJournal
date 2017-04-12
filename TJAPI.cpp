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

void TJAPI::verifyQR(QString QRсode)
{
    worker = new HttpRequestWorker(this);
    HttpRequestInput input = this->createRequest("account/verifyQR", "POST");
    if(QRсode.size())
    {
        QString QRwithsalt=QRсode+this->salt;
        QString QRhash = QString(QCryptographicHash::hash((QRwithsalt.toLocal8Bit()),QCryptographicHash::Md5).toHex());
        qDebug()<<QRhash;
        input.addVar("hash", QRhash);
        input.addVar("token", QRсode);
        connect(worker, SIGNAL(executionFinished(HttpRequestWorker*)), this,
                SIGNAL(verifyQRFinished(HttpRequestWorker*)));
        worker->execute(&input);
    }
}
void TJAPI::getUserInfo(int id)
{
    worker = new HttpRequestWorker(this);
    HttpRequestInput input = this->createRequest("account/info", "GET");

    if(id)
        input.addVar("userId", QString::number(id));

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

void TJAPI::getClubPosts(int count, int offset, int type, QString sortMode)
{
    worker = new HttpRequestWorker(this);
    HttpRequestInput input = this->createRequest("club", "GET");
    if (count!=30)
        input.addVar("count", QString::number(count));
    if (offset)
        input.addVar("offset", QString::number(offset));
    if (type)
        input.addVar("type", QString::number(type));
    if(sortMode!="mainpage")
        input.addVar("sortMode", sortMode);
    connect(worker, SIGNAL(executionFinished(HttpRequestWorker*)), this,
            SIGNAL(getClubPostsExecutionFinished(HttpRequestWorker*)));
    worker->execute(&input);
}

void TJAPI::getAccountPosts(int userId, int count, int offset)
{
    worker = new HttpRequestWorker(this);
    HttpRequestInput input = this->createRequest("account/posts", "GET");
    if(userId)
        input.addVar("userId", QString::number(userId));
    if (count!=30)
        input.addVar("count", QString::number(count));
    if (offset)
        input.addVar("offset", QString::number(offset));
    //TODO: ADD CONNECT!
    worker->execute(&input);
}

void TJAPI::getAccountComments(int userId, int count, int offset)
{
    worker = new HttpRequestWorker(this);
    HttpRequestInput input = this->createRequest("account/comments", "GET");
    if(userId)
        input.addVar("userId", QString::number(userId));
    if (count!=30)
        input.addVar("count", QString::number(count));
    if (offset)
        input.addVar("offset", QString::number(offset));
    //TODO: ADD CONNECT!
    worker->execute(&input);
}

void TJAPI::getNotifications()
{
    worker = new HttpRequestWorker(this);
    HttpRequestInput input = this->createRequest("account/notifications", "GET");
    //TODO: ADD CONNECT!
    worker->execute(&input);
}

void TJAPI::getFavorites(int objectType, int count, int offset)
{
    worker = new HttpRequestWorker(this);
    HttpRequestInput input = this->createRequest("favorites", "GET");
    if(objectType)
        input.addVar("objectType", QString::number(objectType));
    if (count!=30)
        input.addVar("count", QString::number(count));
    if (offset)
        input.addVar("offset", QString::number(offset));
    //TODO: ADD CONNECT!
    worker->execute(&input);
}

void TJAPI::addFavorite(int objectId, int objectType)
{
    worker = new HttpRequestWorker(this);
    HttpRequestInput input = this->createRequest("favorites/new", "POST");
    input.addVar("objectId", QString::number(objectId));
    input.addVar("objectType", QString::number(objectType));
    //TODO: ADD CONNECT!
    worker->execute(&input);
}

void TJAPI::removeFavorite(int objectId, int objectType)
{
    worker = new HttpRequestWorker(this);
    HttpRequestInput input = this->createRequest("favorites/remove", "POST");
    input.addVar("objectId", QString::number(objectId));
    input.addVar("objectType", QString::number(objectType));
    //TODO: ADD CONNECT!
    worker->execute(&input);
}

void TJAPI::getAccountSettings()
{
    worker = new HttpRequestWorker(this);
    HttpRequestInput input = this->createRequest("account/settings", "GET");
    //TODO: ADD CONNECT!
    worker->execute(&input);
}
