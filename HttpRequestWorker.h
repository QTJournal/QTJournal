/****************************************************************************
**
** Copyright (C) 2007-2015 Speedovation
** Copyright (C) 2015 creativepulse.gr
** Contact: Speedovation Lab (info@speedovation.com)
**
** KineticWing IDE CrashHandler
** http:// kineticwing.com
** This file is part of the core classes of the KiWi Editor (IDE)
**
** Author: creativepulse.gr
**   https://github.com/codeyash
**   https://github.com/fro0m
** License : Apache License 2.0
**
** All rights are reserved.
*/



#ifndef HTTPREQUESTWORKER_H
#define HTTPREQUESTWORKER_H

#include <QObject>
#include <QString>
#include <QMap>
#include <QNetworkAccessManager>
#include <QNetworkReply>


enum HttpRequestVarLayout {NOT_SET, ADDRESS, URL_ENCODED, MULTIPART};


class HttpRequestInputFileElement {

public:
    QString variableName;
    QString localFilename;
    QString requestFilename;
    QString mimeType;

};


class HttpRequestInput {

public:
    QString urlStr;
    QString httpMethod;
    HttpRequestVarLayout varLayout;
    QMap<QString, QString> vars;
    QMap<QByteArray, QByteArray> headers;
    QList<HttpRequestInputFileElement> files;

    HttpRequestInput();
    HttpRequestInput(const QString& v_urlStr, const QString& v_httpMethod);
    void initialize();
    void addVar(const QString& key, const QString& value);
    void addHeader(const QByteArray& header, const QByteArray& value);
    void addFile(const QString& variableName, const QString& localFilename, const QString& requestFilename, const QString& mimeType);

};


class HttpRequestWorker : public QObject {
    Q_OBJECT

public:
    explicit HttpRequestWorker(QObject *parent = 0);
    ~HttpRequestWorker();
    QByteArray response;
    QNetworkReply::NetworkError errorType;
    QString errorStr;


    QString httpAttributeEncode(QString attribute_name, QString input);
    void execute(HttpRequestInput *input);

signals:
    void executionFinished(HttpRequestWorker *worker);

private:
    QNetworkAccessManager *manager;

private slots:
    void managerFinished(QNetworkReply *reply);

};

#endif // HTTPREQUESTWORKER_H
