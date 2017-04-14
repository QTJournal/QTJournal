#ifndef TJAPI_H
#define TJAPI_H
#include <QObject>
#include "HttpRequestWorker.h"

class TJAPI : public QObject
{
    Q_OBJECT
public:
    TJAPI();
    void verifyQR(QString QRcode);
    void authorize(QString socialId, int socialType, QString token);
    void getClubPosts(int count=30, int offset=0, int type=0, QString sortMode="mainpage");
    void getUserInfo(int id=0);
    void getAccountPosts(int userId=0, int count=30, int offset=0);
    void getAccountComments(int userId=0, int count=30, int offset=0);
    void getNotifications();
    void getFavorites(int objectType, int count=30, int offset=0);
    void setToken(QByteArray token);
    void addFavorite(int objectId, int objectType);
    void removeFavorite(int objectId, int objectType);
    void getAccountSettings();
    void setAccountSettings(QString &settings);
    void sendMisprint(QString text, QString url);
    void urlReval(QString url);
    QByteArray getToken();
private:
    QString API_HOST = "api.tjournal.ru";
    QString API_VERSION = "2.3";
    QString SALT = "hDv#L9Om>iHfAdT5^6uIy?&";
    QString API_URL = QString("https://%1/%2/").arg(API_HOST, API_VERSION);

    HttpRequestWorker *worker;
    QByteArray token;
    HttpRequestInput createRequest(QString url, QString method);
signals:
    void responseIsHere(QString response);
    void updateTextSignal(QString text);
    void getClubPostsExecutionFinished(HttpRequestWorker *worker);
    void getUserInfoExecutionFinished(HttpRequestWorker *worker);
    void verifyQRFinished(HttpRequestWorker *worker);
    void authorizeFinished(HttpRequestWorker *worker);
    void getAccountPostsFinished(HttpRequestWorker *worker);
    void getAccountCommentsFinished(HttpRequestWorker *worker);
    void getNotificationsFinished(HttpRequestWorker *worker);
    void getFavoritesFinished(HttpRequestWorker *worker);
    void removeFavoriteFinished(HttpRequestWorker *worker);
    void getAccountSettingsFinished(HttpRequestWorker *worker);
    void setAccountSettingsFinished(HttpRequestWorker *worker);
    void sendMisprintFinished(HttpRequestInput *worker);
    void urlRevalFinished(HttpRequestInput *worker);
private slots:
    void handleResult(HttpRequestWorker *worker_);
};

#endif // TJAPI_H
