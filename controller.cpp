#include "controller.h"
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>
#include <QtCore/QJsonValue>

#include <QZXing.h>

#include <QJsonArray>
#include <model/parserutil.h>
#include <model/post.h>

Controller::Controller(MainWindow *mainWindow, QObject *parent) : QObject(parent)
{
    this->mainWindow = mainWindow;
    this->api = new TJAPI();
    QJsonObject device;
    device["id"]="12345";
    device["app_version"]="1.0.0";
    device["os"]="iOS";
    device["app_build"]="0";
    device["locale"]="ru";
    device["os_version"]="1.0.0";
    device["name"]="Balalaika";
    QJsonObject useragent;
    useragent["device"]=device;
    api->setUseragent(QString(QJsonDocument(useragent).toJson(QJsonDocument::Compact)));


    connect(mainWindow, SIGNAL(getUserInfoButtonClicked()), this, SLOT(handleUserInfoButton()));
    connect(mainWindow, SIGNAL(getInfoButtonClicked()), this, SLOT(handleGetInfoButton()));
    connect(mainWindow, SIGNAL(verifyQRButtonClicked(QString)), this, SLOT(handleVerifyQRButton(QString)));

    connect(mainWindow, SIGNAL(QRtoDecode(QStringList)), this, SLOT(decodeQR(QStringList)));

    connect(api, SIGNAL(getClubPostsExecutionFinished(HttpRequestWorker*)), this,
            SLOT(handleGetClubPostsResult(HttpRequestWorker*)));

    connect(this, SIGNAL(updateText(QString)), mainWindow, SLOT(updateText(QString)));
    connect(this, SIGNAL(updateQRString(QString)), mainWindow, SLOT(updateQRString(QString)));
    connect(api, SIGNAL(getUserInfoExecutionFinished(HttpRequestWorker*)), this,
            SLOT(handleGetUserInfoResult(HttpRequestWorker*)));
    connect(api, SIGNAL(verifyQRFinished(HttpRequestWorker*)), this, SLOT(handleVerifyQRResult(HttpRequestWorker*)));
    connect(this, SIGNAL(updatePostsList(QList<Post*>*)), mainWindow, SLOT(updatePostsList(QList<Post*>*)));
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
    api->getClubPosts();
}

void Controller::handleGetClubPostsResult(HttpRequestWorker* worker)
{
    qDebug() << "handleGetClubPostsResult(HttpRequestWorker* worker)";
    if (worker->errorType != QNetworkReply::NoError) {
        qDebug() << worker->errorStr;
        emit updateText(worker->errorStr);
        worker->deleteLater();
        return;
    }
    QString result = worker->response;

    QByteArray byteResult = worker->response;
    QJsonDocument responseJson = QJsonDocument::fromJson(byteResult);
    QJsonArray posts = responseJson.object()["result"].toArray();

    QList<Post*>* postsList = new QList<Post*>();

    for (int i = 0; i < posts.size(); i++) {
        QJsonObject post = posts.at(i).toObject();
        Post* postModel = ParserUtil::parsePost(post);
        postsList->append(postModel);
    }

    worker->deleteLater();

    emit updatePostsList(postsList);
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
void Controller::decodeQR(QStringList FileList) {
    QImage img(FileList[0]);
    QZXing decoder;
    QString result = decoder.decodeImage(img).split('|')[1];

    emit updateQRString(result);
}
