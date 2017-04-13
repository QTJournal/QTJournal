#include "TJAPI.h"
TJAPI::TJAPI()
{
    //apiurl=
}

HttpRequestInput TJAPI::createRequest(QString url, QString method)
{
    QString urlStr = this->API_URL+url;
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
        QString QRwithsalt=QRcode+this->SALT;
        QString authhash = QString(QCryptographicHash::hash((QRwithsalt.toLocal8Bit()),QCryptographicHash::Md5).toHex());
        qDebug()<<authhash;
        input.addVar("hash", authhash);
        input.addVar("token", QRcode);
        connect(worker, SIGNAL(executionFinished(HttpRequestWorker*)), this,
                SIGNAL(verifyQRFinished(HttpRequestWorker*)));
        worker->execute(&input);
    }
}

void TJAPI::authorize(QString socialId, int socialType, QString token)
{
    worker = new HttpRequestWorker(this);
    HttpRequestInput input = this->createRequest("account/authorize", "POST");
    QString socialwithsalt=socialId+socialType+token+this->SALT;
    QString authhash = QString(QCryptographicHash::hash((socialwithsalt.toLocal8Bit()),QCryptographicHash::Md5).toHex());
    qDebug()<<authhash;
    input.addVar("socialId", socialId);
    input.addVar("socialType", QString::number(socialType));
    input.addVar("token", token);
    input.addVar("hash", authhash);
    connect(worker, SIGNAL(executionFinished(HttpRequestWorker*)), this,
            SIGNAL(authorizeFinished(HttpRequestWorker*)));
    worker->execute(&input);
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
    connect(worker, SIGNAL(executionFinished(HttpRequestWorker*)), this,
            SIGNAL(getAccountPostsFinished(HttpRequestWorker*)));
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
    connect(worker, SIGNAL(executionFinished(HttpRequestWorker*)), this,
            SIGNAL(getAccountCommentsFinished(HttpRequestWorker*)));
    worker->execute(&input);
}

void TJAPI::getNotifications()
{
    worker = new HttpRequestWorker(this);
    HttpRequestInput input = this->createRequest("account/notifications", "GET");
    connect(worker, SIGNAL(executionFinished(HttpRequestWorker*)), this,
            SIGNAL(getNotificationsFinished(HttpRequestWorker*)));
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
    connect(worker, SIGNAL(executionFinished(HttpRequestWorker*)), this,
            SIGNAL(getFavoritesFinished(HttpRequestWorker*)));
    worker->execute(&input);
}

void TJAPI::addFavorite(int objectId, int objectType)
{
    worker = new HttpRequestWorker(this);
    HttpRequestInput input = this->createRequest("favorites/new", "POST");
    input.addVar("objectId", QString::number(objectId));
    input.addVar("objectType", QString::number(objectType));
    connect(worker, SIGNAL(executionFinished(HttpRequestWorker*)), this,
            SIGNAL(addFavoriteFinished(HttpRequestWorker*)));
    worker->execute(&input);
}

void TJAPI::removeFavorite(int objectId, int objectType)
{
    worker = new HttpRequestWorker(this);
    HttpRequestInput input = this->createRequest("favorites/remove", "POST");
    input.addVar("objectId", QString::number(objectId));
    input.addVar("objectType", QString::number(objectType));
    connect(worker, SIGNAL(executionFinished(HttpRequestWorker*)), this,
            SIGNAL(removeFavoriteFinished(HttpRequestWorker*)));
    worker->execute(&input);
}

void TJAPI::getAccountSettings()
{
    worker = new HttpRequestWorker(this);
    HttpRequestInput input = this->createRequest("account/settings", "GET");
    connect(worker, SIGNAL(executionFinished(HttpRequestWorker*)), this,
            SIGNAL(getAccountSettingsFinished(HttpRequestWorker*)));
    worker->execute(&input);
}

void TJAPI::setAccountSettings(QString &settings)
{
    worker = new HttpRequestWorker(this);
    HttpRequestInput input = this->createRequest("account/settings", "POST");
    foreach (QString sett, settings)
    {
        input.addVar("notifications[email][]", sett);
    }
    connect(worker, SIGNAL(executionFinished(HttpRequestWorker*)), this,
            SIGNAL(setAccountSettingsFinished(HttpRequestWorker*)));
    worker->execute(&input);
}

void TJAPI::sendMisprint(QString text, QString url)
{
    worker = new HttpRequestWorker(this);
    HttpRequestInput input = this->createRequest("misprints/new", "POST");
    input.addVar("text", text);
    input.addVar("url", url);
    connect(worker, SIGNAL(executionFinished(HttpRequestWorker*)), this,
            SIGNAL(sendMisprintFinished(HttpRequestWorker*)));
    worker->execute(&input);
}

void TJAPI::urlReval(QString url)
{
    worker = new HttpRequestWorker(this);
    HttpRequestInput input = this->createRequest("content/reveal", "GET");
    input.addVar("url", url);
    connect(worker, SIGNAL(executionFinished(HttpRequestWorker*)), this,
            SIGNAL(urlRevalFinished(HttpRequestWorker*)));
    worker->execute(&input);
}

void TJAPI::getNews(int listId, int count, QString interval)
{
    worker = new HttpRequestWorker(this);
    HttpRequestInput input = this->createRequest("news", "GET");
    input.addVar("listId", QString::number(listId));
    if(count!=50)
        input.addVar("count", QString::number(count));
    if (interval!="popular")
        input.addVar("interval", interval);
    connect(worker, SIGNAL(executionFinished(HttpRequestWorker*)), this,
            SIGNAL(getNewsFinished(HttpRequestWorker*)));
    worker->execute(&input);
}

void TJAPI::getNewsLists(int listId, bool showSources)
{
    worker = new HttpRequestWorker(this);
    HttpRequestInput input = this->createRequest("news/lists", "GET");
    input.addVar("listId", QString::number(listId));
    if(!showSources)
        input.addVar("showSources", "0");
    connect(worker, SIGNAL(executionFinished(HttpRequestWorker*)), this,
            SIGNAL(getNewsListsFinished(HttpRequestInput*)));
    worker->execute(&input);
}

void TJAPI::setNewsSettings(QString &settingsML, QString &settings)
{
    worker = new HttpRequestWorker(this);
    HttpRequestInput input = this->createRequest("news/settings", "POST");
    foreach (QString sett, settingsML)
    {
        input.addVar("settings[mailList]", sett);
    }
    foreach (QString sett, settings)
    {
        input.addVar("settings[lists][]", sett);
    }
    connect(worker, SIGNAL(executionFinished(HttpRequestWorker*)), this,
            SIGNAL(setNewsSettingsFinished(HttpRequestInput*)));
    worker->execute(&input);
}

void TJAPI::setNewsListExcludes(int listId, int sources[])
{
    worker = new HttpRequestWorker(this);
    HttpRequestInput input = this->createRequest("news/listExcludes", "POST");
    input.addVar("listId", QString::number(listId));
    for (int i=0; i<int(sizeof(sources)); i++)
    {
        input.addVar("sources[]", QString::number(sources[i]));
    }
    connect(worker, SIGNAL(executionFinished(HttpRequestWorker*)), this,
            SIGNAL(setNewsListExcludesFinished(HttpRequestInput*)));
    worker->execute(&input);
}

void TJAPI::getTweets(int count, int offset, int listId, QString interval)
{
    worker = new HttpRequestWorker(this);
    HttpRequestInput input = this->createRequest("news", "GET");
    if(count!=50)
        input.addVar("count", QString::number(count));
    if(offset)
        input.addVar("offset", QString::number(offset));
    if(listId!=1)
        input.addVar("listId", QString::number(listId));
    if (interval!="fresh")
        input.addVar("interval", interval);
    connect(worker, SIGNAL(executionFinished(HttpRequestWorker*)), this,
            SIGNAL(getTweetsFinished(HttpRequestWorker*)));
    worker->execute(&input);
}

void TJAPI::getBlacklist()
{
    worker = new HttpRequestWorker(this);
    HttpRequestInput input = this->createRequest("tweets/blacklist", "GET");
    connect(worker, SIGNAL(executionFinished(HttpRequestWorker*)), this,
            SIGNAL(getBlacklistFinished(HttpRequestWorker*)));
    worker->execute(&input);
}

void TJAPI::addBlacklisted(QString tweopleId, QString hash)
{
    worker = new HttpRequestWorker(this);
    HttpRequestInput input = this->createRequest("tweets/blacklistAdd", "POST");
    input.addVar("tweopleId", tweopleId);
    input.addVar("hash", hash);
    connect(worker, SIGNAL(executionFinished(HttpRequestWorker*)), this,
            SIGNAL(addBlecklistedinished(HttpRequestWorker*)));
    worker->execute(&input);
}

void TJAPI::removeBlacklisted(QString tweopleId, QString hash)
{
    worker = new HttpRequestWorker(this);
    HttpRequestInput input = this->createRequest("tweets/blacklistRemove", "POST");
    input.addVar("tweopleId", tweopleId);
    input.addVar("hash", hash);
    connect(worker, SIGNAL(executionFinished(HttpRequestWorker*)), this,
            SIGNAL(addBlacklistedFinished(HttpRequestWorker*)));
    worker->execute(&input);
}

