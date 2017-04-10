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
    void getInfo();
    void getUserInfo();
    void setToken(QByteArray token);
    QByteArray getToken();
private:
    HttpRequestWorker *worker;
    QByteArray token;
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
