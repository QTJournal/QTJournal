#include "TJAPI.h"
TJAPI::TJAPI()
{

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

void TJAPI::setUseragent(QString agent)
{
    qDebug()<<agent;
    this->myuseragent=agent;
}

void TJAPI::verifyQR(QString QRcode)
{
    HttpRequestWorker *worker = new HttpRequestWorker(this);
    worker->setUseragent(this->myuseragent);

    HttpRequestInput input = this->createRequest("auth/qr", "POST");
    if(QRcode.size())
    {
        //QString QRwithsalt=QRcode+this->SALT;
       // QString QRhash = QString(QCryptographicHash::hash((QRwithsalt.toLocal8Bit()),QCryptographicHash::Md5).toHex());
        //qDebug()<<QRhash;
        //input.addVar("hash", QRhash);

        input.addVar("token", QRcode);
        connect(worker, SIGNAL(executionFinished(HttpRequestWorker*)), this,
                SIGNAL(verifyQRFinished(HttpRequestWorker*)));
        worker->execute(&input);
    }
}

void TJAPI::authorize(QString socialId, int socialType, QString token)
{
    HttpRequestWorker *worker = new HttpRequestWorker(this);
    worker->setUseragent(this->myuseragent);

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
    HttpRequestWorker *worker = new HttpRequestWorker(this);
    worker->setUseragent(this->myuseragent);

    HttpRequestInput input = this->createRequest("user/me", "GET");

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
    HttpRequestWorker *worker = new HttpRequestWorker(this);
    worker->setUseragent(this->myuseragent);

    HttpRequestInput input = this->createRequest("timeline/club", "GET");
    input.addVar("count", QString::number(count));
    input.addVar("offset", QString::number(offset));
    if(sortMode!="mainpage")
        input.addVar("sortMode", sortMode);
    connect(worker, SIGNAL(executionFinished(HttpRequestWorker*)), this,
            SIGNAL(getClubPostsExecutionFinished(HttpRequestWorker*)));
    worker->execute(&input);
}

void TJAPI::getClubPost(int entryId)
{
    HttpRequestWorker *worker = new HttpRequestWorker(this);
    worker->setUseragent(this->myuseragent);

    HttpRequestInput input = this->createRequest("entry/"+ QString::number(entryId), "GET");
    //input.addVar("entryId",);
    connect(worker, SIGNAL(executionFinished(HttpRequestWorker*)), this,
            SIGNAL(getClubPostFinished(HttpRequestWorker*)));
    worker->execute(&input);
}

void TJAPI::likeClubPost(int entryId, bool sign)
{
    HttpRequestWorker *worker = new HttpRequestWorker(this);
    worker->setUseragent(this->myuseragent);

    HttpRequestInput input = this->createRequest("entry/"+QString::number(entryId)+"/likes", "POST");
    //input.addVar("entryId", );
    input.addVar("sign", sign?"+1":"-1");
   // input.addVar("hash", hash);
    connect(worker, SIGNAL(executionFinished(HttpRequestWorker*)), this,
            SIGNAL(likeClubPostFinished(HttpRequestWorker*)));
    worker->execute(&input);
}

void TJAPI::newClubPost(QString title, QString session_id, QString text, QList<QString> file_attaches, QList<QString> files)
{
    HttpRequestWorker *worker = new HttpRequestWorker(this);
    worker->setUseragent(this->myuseragent);

    HttpRequestInput input = this->createRequest("api/entry/create", "POST");
    input.addVar("title", title);
    input.addVar("text", text);
    input.addVar("session_id", session_id);
    if(file_attaches.size()+files.size()>2)
        //exception
    for(int i=0; i<file_attaches.size(); i++)
    {
        QFileInfo fileInfo (file_attaches[i]);
        QMimeDatabase db;
        QMimeType fileType = db.mimeTypeForFile(file_attaches[i]);
        //TODO: add exceptions
        input.addFile("file_attaches["+QString::number(i)+"]", fileInfo.filePath(), NULL, fileType.name());
    }
    for(int i=0; i<files.size(); i++)
        input.addVar("files["+QString::number(i)+"]", files[i]);
    input.varLayout=MULTIPART;
    connect(worker, SIGNAL(executionFinished(HttpRequestWorker*)), this,
            SIGNAL(getClubPostFinished(HttpRequestWorker*)));
    worker->execute(&input);
}

void TJAPI::newComment(QString message, int paperId, QList<QString> file_attaches, QList<QString> files, int inReplyToCommentId=0)
{
    HttpRequestWorker *worker = new HttpRequestWorker(this);
    worker->setUseragent(this->myuseragent);

    HttpRequestInput input = this->createRequest("entry/"+QString::number(paperId)+"/comments", "POST");
    input.addVar("text", message);
    //input.addVar("paperId", QString::number(paperId));
    input.addVar("reply_to", QString::number(inReplyToCommentId));

    if(file_attaches.size()+files.size()>2)
        //exception
    for(int i=0; i<file_attaches.size(); i++)
    {
        QFileInfo fileInfo (file_attaches[i]);
        QMimeDatabase db;
        QMimeType fileType = db.mimeTypeForFile(file_attaches[i]);
        //TODO: add exceptions
        input.addFile("file_attaches["+QString::number(i)+"]", fileInfo.filePath(), NULL, fileType.name());
    }
    for(int i=0; i<files.size(); i++)
        input.addVar("files["+QString::number(i)+"]", files[i]);
    input.varLayout=MULTIPART;
    connect(worker, SIGNAL(executionFinished(HttpRequestWorker*)), this,
            SIGNAL(newCommentFinished(HttpRequestWorker*)));
    worker->execute(&input);
}

void TJAPI::likeComment(int entryId, bool sign, QString hash)
{
    HttpRequestWorker *worker = new HttpRequestWorker(this);
    worker->setUseragent(this->myuseragent);

    HttpRequestInput input = this->createRequest("comment/"+QString::number(entryId)+"/likes", "POST");
    //input.addVar("entryId", );
    input.addVar("sign", sign?"+1":"-1");
   // input.addVar("hash", hash);
    connect(worker, SIGNAL(executionFinished(HttpRequestWorker*)), this,
            SIGNAL(likeCommentFinished(HttpRequestWorker*)));
    worker->execute(&input);
}

void TJAPI::getComments(int paperId, QString sorting)
{
    HttpRequestWorker *worker = new HttpRequestWorker(this);
    worker->setUseragent(this->myuseragent);

    HttpRequestInput input = this->createRequest("entry/"+QString::number(paperId)+"/comments/"+sorting, "GET");
    //input.addVar("section", section);
    //input.addVar("paperId", QString::number(paperId));
    connect(worker, SIGNAL(executionFinished(HttpRequestWorker*)), this,
            SIGNAL(getCommentsFinished(HttpRequestWorker*)));
    worker->execute(&input);
}

void TJAPI::getAccountPosts(int userId, int count, int offset)
{
    HttpRequestWorker *worker = new HttpRequestWorker(this);
    worker->setUseragent(this->myuseragent);
    HttpRequestInput input = this->createRequest("user/"+userId?QString::number(userId):QString("me")+"/entries", "GET");
    if (count)
        input.addVar("count", QString::number(count));
    if (offset)
        input.addVar("offset", QString::number(offset));

    connect(worker, SIGNAL(executionFinished(HttpRequestWorker*)), this,
            SIGNAL(getAccountPostsFinished(HttpRequestWorker*)));
    worker->execute(&input);
}

void TJAPI::search(QString q, int count, int offset, int type)
{
    HttpRequestWorker *worker = new HttpRequestWorker(this);
    worker->setUseragent(this->myuseragent);

    HttpRequestInput input = this->createRequest("search", "GET");
    input.addVar("q", q);
    if (count!=30)
        input.addVar("count", QString::number(count));
    if (offset)
        input.addVar("offset", QString::number(offset));
    if (type!=1)
        input.addVar("type", QString::number(type));
    connect(worker, SIGNAL(executionFinished(HttpRequestWorker*)), this,
            SIGNAL(searchFinished(HttpRequestWorker*)));
    worker->execute(&input);
}

void TJAPI::getAccountComments(int userId, int count, int offset)
{
    HttpRequestWorker *worker = new HttpRequestWorker(this);
    worker->setUseragent(this->myuseragent);

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
    HttpRequestWorker *worker = new HttpRequestWorker(this);
    worker->setUseragent(this->myuseragent);

    HttpRequestInput input = this->createRequest("account/notifications", "GET");
    connect(worker, SIGNAL(executionFinished(HttpRequestWorker*)), this,
            SIGNAL(getNotificationsFinished(HttpRequestWorker*)));
    worker->execute(&input);
}

void TJAPI::getFavorites(int objectType, int count, int offset)
{
    HttpRequestWorker *worker = new HttpRequestWorker(this);
    worker->setUseragent(this->myuseragent);

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
    HttpRequestWorker *worker = new HttpRequestWorker(this);
    worker->setUseragent(this->myuseragent);

    HttpRequestInput input = this->createRequest("favorites/new", "POST");
    input.addVar("objectId", QString::number(objectId));
    input.addVar("objectType", QString::number(objectType));
    connect(worker, SIGNAL(executionFinished(HttpRequestWorker*)), this,
            SIGNAL(addFavoriteFinished(HttpRequestWorker*)));
    worker->execute(&input);
}

void TJAPI::removeFavorite(int objectId, int objectType)
{
    HttpRequestWorker *worker = new HttpRequestWorker(this);
    worker->setUseragent(this->myuseragent);

    HttpRequestInput input = this->createRequest("favorites/remove", "POST");
    input.addVar("objectId", QString::number(objectId));
    input.addVar("objectType", QString::number(objectType));
    connect(worker, SIGNAL(executionFinished(HttpRequestWorker*)), this,
            SIGNAL(removeFavoriteFinished(HttpRequestWorker*)));
    worker->execute(&input);
}

void TJAPI::getAccountSettings()
{
    HttpRequestWorker *worker = new HttpRequestWorker(this);
    worker->setUseragent(this->myuseragent);

    HttpRequestInput input = this->createRequest("account/settings", "GET");
    connect(worker, SIGNAL(executionFinished(HttpRequestWorker*)), this,
            SIGNAL(getAccountSettingsFinished(HttpRequestWorker*)));
    worker->execute(&input);
}

void TJAPI::setAccountSettings(QList <QString> settings)
{
    HttpRequestWorker *worker = new HttpRequestWorker(this);
    worker->setUseragent(this->myuseragent);

    HttpRequestInput input = this->createRequest("account/settings", "POST");
    for (QString sett : settings)
    {
        input.addVar("notifications[email][]", sett);
    }
    connect(worker, SIGNAL(executionFinished(HttpRequestWorker*)), this,
            SIGNAL(setAccountSettingsFinished(HttpRequestWorker*)));
    worker->execute(&input);
}

void TJAPI::sendMisprint(QString text, QString url)
{
    HttpRequestWorker *worker = new HttpRequestWorker(this);
    worker->setUseragent(this->myuseragent);

    HttpRequestInput input = this->createRequest("misprints/new", "POST");
    input.addVar("text", text);
    input.addVar("url", url);
    connect(worker, SIGNAL(executionFinished(HttpRequestWorker*)), this,
            SIGNAL(sendMisprintFinished(HttpRequestWorker*)));
    worker->execute(&input);
}

void TJAPI::urlReval(QString url)
{
    HttpRequestWorker *worker = new HttpRequestWorker(this);
    worker->setUseragent(this->myuseragent);

    HttpRequestInput input = this->createRequest("content/reveal", "GET");
    input.addVar("url", url);
    connect(worker, SIGNAL(executionFinished(HttpRequestWorker*)), this,
            SIGNAL(urlRevalFinished(HttpRequestWorker*)));
    worker->execute(&input);
}

void TJAPI::getNews(int listId, int count, QString interval)
{
    HttpRequestWorker *worker = new HttpRequestWorker(this);
    worker->setUseragent(this->myuseragent);

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
    HttpRequestWorker *worker = new HttpRequestWorker(this);
    worker->setUseragent(this->myuseragent);

    HttpRequestInput input = this->createRequest("news/lists", "GET");
    input.addVar("listId", QString::number(listId));
    if(!showSources)
        input.addVar("showSources", "0");
    connect(worker, SIGNAL(executionFinished(HttpRequestWorker*)), this,
            SIGNAL(getNewsListsFinished(HttpRequestInput*)));
    worker->execute(&input);
}

void TJAPI::setNewsSettings(QList <QString> settingsML, QList <QString> settings)
{
    HttpRequestWorker *worker = new HttpRequestWorker(this);
    worker->setUseragent(this->myuseragent);

    HttpRequestInput input = this->createRequest("news/settings", "POST");
    for (QString sett : settingsML)
    {
        input.addVar("settings[mailList]", sett);
    }
    for (QString sett : settings)
    {
        input.addVar("settings[lists][]", sett);
    }
    connect(worker, SIGNAL(executionFinished(HttpRequestWorker*)), this,
            SIGNAL(setNewsSettingsFinished(HttpRequestInput*)));
    worker->execute(&input);
}

void TJAPI::setNewsListExcludes(int listId, QList <int> sources)
{
    HttpRequestWorker *worker = new HttpRequestWorker(this);
    worker->setUseragent(this->myuseragent);

    HttpRequestInput input = this->createRequest("news/listExcludes", "POST");
    input.addVar("listId", QString::number(listId));
    for (int source : sources)
    {
        input.addVar("sources[]", QString::number(source));
    }
    connect(worker, SIGNAL(executionFinished(HttpRequestWorker*)), this,
            SIGNAL(setNewsListExcludesFinished(HttpRequestInput*)));
    worker->execute(&input);
}

void TJAPI::getTweets(int count, int offset, int listId, QString interval)
{
    HttpRequestWorker *worker = new HttpRequestWorker(this);
    worker->setUseragent(this->myuseragent);

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
    HttpRequestWorker *worker = new HttpRequestWorker(this);
    worker->setUseragent(this->myuseragent);

    HttpRequestInput input = this->createRequest("tweets/blacklist", "GET");
    connect(worker, SIGNAL(executionFinished(HttpRequestWorker*)), this,
            SIGNAL(getBlacklistFinished(HttpRequestWorker*)));
    worker->execute(&input);
}

void TJAPI::addBlacklisted(QString tweopleId, QString hash)
{
    HttpRequestWorker *worker = new HttpRequestWorker(this);
    worker->setUseragent(this->myuseragent);

    HttpRequestInput input = this->createRequest("tweets/blacklistAdd", "POST");
    input.addVar("tweopleId", tweopleId);
    input.addVar("hash", hash);
    connect(worker, SIGNAL(executionFinished(HttpRequestWorker*)), this,
            SIGNAL(addBlecklistedinished(HttpRequestWorker*)));
    worker->execute(&input);
}

void TJAPI::removeBlacklisted(QString tweopleId, QString hash)
{
    HttpRequestWorker *worker = new HttpRequestWorker(this);
    worker->setUseragent(this->myuseragent);

    HttpRequestInput input = this->createRequest("tweets/blacklistRemove", "POST");
    input.addVar("tweopleId", tweopleId);
    input.addVar("hash", hash);
    connect(worker, SIGNAL(executionFinished(HttpRequestWorker*)), this,
            SIGNAL(addBlacklistedFinished(HttpRequestWorker*)));
    worker->execute(&input);
}

void TJAPI::activateTrial()
{
    HttpRequestWorker *worker = new HttpRequestWorker(this);
    worker->setUseragent(this->myuseragent);

    HttpRequestInput input = this->createRequest("purchases/trial", "POST");
    connect(worker, SIGNAL(executionFinished(HttpRequestWorker*)), this,
            SIGNAL(activateTrialFinished(HttpRequestWorker*)));
    worker->execute(&input);
}
