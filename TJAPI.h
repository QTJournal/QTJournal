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
    void getInfo();
    void getUserInfo();
    void getAccountPosts(int account=0);
    void setToken(QByteArray token);
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
    void getInfoExecutionFinished(HttpRequestWorker *worker);
    void getUserInfoExecutionFinished(HttpRequestWorker *worker);
    void verifyQRFinished(HttpRequestWorker *worker);
private slots:
    void handleResult(HttpRequestWorker *worker_);
};

#endif // TJAPI_H
