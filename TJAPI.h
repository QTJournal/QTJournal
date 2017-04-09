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
private:
    HttpRequestWorker *worker;
    QByteArray token;
signals:
    void responseIsHere(QString response);
private slots:
    void handleAuth(HttpRequestWorker* worker_);
};

#endif // TJAPI_H
