#ifndef TJAPI_H
#define TJAPI_H
#include <QObject>
#include "HttpRequestWorker.h"

class TJAPI : public QObject
{
    Q_OBJECT
public:
    TJAPI();
    void verifyQR(QString QRCode);
    void getUserInfo();
    void getInfo();
private:
    HttpRequestWorker *worker;
    QByteArray token;
signals:
    void responseIsHere(QString response);
    void updatedText(QString text);
public slots:
    void handleResult(HttpRequestWorker* worker_);
private slots:
    void handleAuth(HttpRequestWorker* worker_);
};

#endif // TJAPI_H
