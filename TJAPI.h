#ifndef TJAPI_H
#define TJAPI_H
#include <QObject>
#include <QFile>
#include <QFileDialog>
#include <QMimeType>
#include <QMimeDatabase>
#include "HttpRequestWorker.h"

class TJAPI : public QObject
{
    Q_OBJECT
public:
    TJAPI();
    void verifyQR(QString QRcode);
    void authorize(QString socialId, int socialType, QString token);
    void getClubPosts(int count=30, int offset=0, int type=0, QString sortMode="mainpage");
    void getClubPost(int entryId);
    void getUserInfo(int id=0);
    void getAccountPosts(int userId=0, int count=30, int offset=0);
    void getAccountComments(int userId=0, int count=30, int offset=0);
    void getNotifications();
    void getFavorites(int objectType, int count=30, int offset=0);
    void setToken(QByteArray token);
    void addFavorite(int objectId, int objectType);
    void removeFavorite(int objectId, int objectType);
    void getAccountSettings();
    void setAccountSettings(QList <QString> settings);
    void sendMisprint(QString text, QString url);
    void urlReval(QString url);
    void getNews(int listId, int count=50, QString interval="popular");
    void getNewsLists(int listId, bool showSources=1);
    void setNewsSettings(QList <QString> settingsML, QList <QString> settings);
    void setNewsListExcludes(int listId, QList <int> sources);
    void getTweets(int count=50, int offset=0, int listId=1, QString interval="fresh");
    void getBlacklist();
    void addBlacklisted(QString tweopleId, QString hash);
    void removeBlacklisted(QString tweopleId, QString hash);
    void newClubPost(QString title, QString url, QString content, QList <QString> file_attaches= {}, QList <QString> files= {});
    void likeClubPost(int entryId, bool sign, QString hash);
    void newComment(QString message, int paperId, QList<QString> file_attaches, QList<QString> files, int inReplyToCommentId);
    void likeComment(int entryId, bool sign, QString hash);
    void getComments(QString section, int paperId);
    void search(QString q, int count=30, int offset=0, int type=1);
    void setUseragent(QString);
    void activateTrial();
    QByteArray getToken();
private:
    QString API_HOST = "api.tjournal.ru";
    QString API_VERSION = "2.3";
    QString SALT = "hDv#L9Om>iHfAdT5^6uIy?&";
    QString API_URL = QString("https://%1/%2/").arg(API_HOST, API_VERSION);
    QString myuseragent="{\"device\":{\"id\":\"12345\",\"app_version\":\"1.0.0\",\"os\":\"Balalaika\",\"app_build\":\"144\",\"locale\":\"ru\",\"os_version\":\"1.0.0\",\"name\":\"Testing Device\"}}";
    QByteArray token;
    HttpRequestInput createRequest(QString url, QString method);
signals:
    void responseIsHere(QString response);
    void updateTextSignal(QString text);
    void getClubPostsExecutionFinished(HttpRequestWorker *worker);
    void getClubPostFinished(HttpRequestWorker *worker);
    void likeClubPostFinished(HttpRequestWorker *worker);
    void newCommentFinished(HttpRequestWorker *worker);
    void likeCommentFinished(HttpRequestWorker *worker);
    void getCommentsFinished(HttpRequestWorker *worker);
    void searchFinished(HttpRequestWorker *worker);
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
    void getNewsFinished(HttpRequestInput *worker);
    void getNewsListsFinished(HttpRequestInput *worker);
    void setNewsSettingsFinished(HttpRequestInput *worker);
    void setNewsListExcludesFinished(HttpRequestInput *worker);
    void getTweetsFinished(HttpRequestInput *worker);
    void getBlacklistFinished(HttpRequestInput *worker);
    void addBlacklistedFinished(HttpRequestInput *worker);
    void removeBlacklistedFinished(HttpRequestInput *worker);
    void activateTrialFinished(HttpRequestInput *worker);
;private slots:
    void handleResult(HttpRequestWorker *worker_);
};

#endif // TJAPI_H
