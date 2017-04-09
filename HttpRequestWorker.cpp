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
**          https://github.com/codeyash
**          https://github.com/fro0m
** License : Apache License 2.0
**
** All rights are reserved.
*/


#include "HttpRequestWorker.h"

#include <QDateTime>
#include <QUrl>
#include <QFileInfo>
#include <QBuffer>


HttpRequestInput::HttpRequestInput() {
    initialize();
}

HttpRequestInput::HttpRequestInput(const QString& v_urlStr, const QString &v_httpMethod) {
    initialize();
    urlStr = v_urlStr;
    httpMethod = v_httpMethod;
}

void HttpRequestInput::initialize() {
    varLayout = NOT_SET;
    urlStr = "";
    httpMethod = "GET";
}

void HttpRequestInput::addVar(const QString & key, const QString & value)
{
    vars[key] = value;
}

void HttpRequestInput::addHeader(const QByteArray &header, const QByteArray &value)
{
    headers[header] = value;
}

void HttpRequestInput::addFile(const QString &variableName, const QString &localFilename, const QString &requestFilename, const QString &mimeType) {
    HttpRequestInputFileElement file;
    file.variableName = variableName;
    file.localFilename = localFilename;
    file.requestFilename = requestFilename;
    file.mimeType = mimeType;
    files.append(file);
}


HttpRequestWorker::HttpRequestWorker(QObject *parent)
    : QObject(parent), manager(NULL)
{
    qsrand(QDateTime::currentDateTime().toTime_t());

    manager = new QNetworkAccessManager(this);
    connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(managerFinished(QNetworkReply*)));
}

HttpRequestWorker::~HttpRequestWorker()
{
    //    for (QObject *obj : this->children()) {
    //        qDebug() << "there is a child " << obj->metaObject()->className();
    //    }
    delete manager;
}

QString HttpRequestWorker::httpAttributeEncode(QString attribute_name, QString input) {
    // result structure follows RFC 5987
    bool need_utf_encoding = false;
    QString result = "";
    QByteArray input_c = input.toLocal8Bit();
    char c;
    for (int i = 0; i < input_c.length(); i++) {
        c = input_c.at(i);
        if (c == '\\' || c == '/' || c == '\0' || c < ' ' || c > '~') {
            // ignore and request utf-8 version
            need_utf_encoding = true;
        }
        else if (c == '"') {
            result += "\\\"";
        }
        else {
            result += c;
        }
    }

    if (result.length() == 0) {
        need_utf_encoding = true;
    }

    if (!need_utf_encoding) {
        // return simple version
        return QString("%1=\"%2\"").arg(attribute_name, result);
    }

    QString result_utf8 = "";
    for (int i = 0; i < input_c.length(); i++) {
        c = input_c.at(i);
        if (
                (c >= '0' && c <= '9')
                || (c >= 'A' && c <= 'Z')
                || (c >= 'a' && c <= 'z')
                ) {
            result_utf8 += c;
        }
        else {
            result_utf8 += "%" + QString::number(static_cast<unsigned char>(input_c.at(i)), 16).toUpper();
        }
    }

    // return enhanced version with UTF-8 support
    return QString("%1=\"%2\"; %1*=utf-8''%3").arg(attribute_name, result, result_utf8);
}

void HttpRequestWorker::execute(HttpRequestInput *input) {

    // reset variables

    QByteArray requestContent = "";
    response = "";
    errorType = QNetworkReply::NoError;
    errorStr = "";


    // decide on the variable layout

    if (input->files.length() > 0) {
        input->varLayout = MULTIPART;
    } else if (input->varLayout == NOT_SET) {
        input->varLayout = input->httpMethod == "GET" || input->httpMethod == "HEAD" ? ADDRESS : URL_ENCODED;
    }


    // prepare request content

    QString boundary = "";

    if (input->varLayout == ADDRESS || input->varLayout == URL_ENCODED) {
        // variable layout is ADDRESS or URL_ENCODED

        if (input->vars.count() > 0) {
            bool first = true;
            foreach (QString key, input->vars.keys()) {
                if (!first) {
                    requestContent.append("&");
                }
                first = false;

                requestContent.append(QUrl::toPercentEncoding(key));
                requestContent.append("=");
                requestContent.append(QUrl::toPercentEncoding(input->vars.value(key)));
            }

            if (input->varLayout == ADDRESS) {
                input->urlStr += "?" + requestContent;
                requestContent = "";
            }
        }
    }
    else {
        // variable layout is MULTIPART

        boundary = "__-----------------------"
                + QString::number(QDateTime::currentDateTime().toTime_t())
                + QString::number(qrand());
        QString boundaryDelimiter = "--";
        QString newLine = "\r\n";

        // add variables
        foreach (QString key, input->vars.keys()) {
            // add boundary
            requestContent.append(boundaryDelimiter);
            requestContent.append(boundary);
            requestContent.append(newLine);

            // add header
            //! The Content-Disposition response-header field has been proposed
            //! as a means for the origin server to suggest a default filename
            //! if the user requests that the content is saved to a file. This
            //! usage is derived from the definition of Content-Disposition
            //! in RFC 1806
            requestContent.append("Content-Disposition: form-data; ");
            requestContent.append(httpAttributeEncode("name", key));
            requestContent.append(newLine);
            requestContent.append("Content-Type: text/plain");
            requestContent.append(newLine);

            // add header to body splitter
            requestContent.append(newLine);

            // add variable content
            requestContent.append(input->vars.value(key));
            requestContent.append(newLine);
        }

        // add files
        for (QList<HttpRequestInputFileElement>::iterator fileInfo = input->files.begin(); fileInfo != input->files.end(); fileInfo++) {
            QFileInfo fi(fileInfo->localFilename);

            // ensure necessary variables are available
            if (
                    fileInfo->localFilename == NULL || fileInfo->localFilename.isEmpty()
                    || fileInfo->variableName == NULL || fileInfo->variableName.isEmpty()
                    || !fi.exists() || !fi.isFile() || !fi.isReadable()
                    ) {
                // silent abort for the current file
                continue;
            }

            QFile file(fileInfo->localFilename);
            if (!file.open(QIODevice::ReadOnly)) {
                // silent abort for the current file
                continue;
            }

            // ensure filename for the request
            if (fileInfo->requestFilename == NULL || fileInfo->requestFilename.isEmpty()) {
                fileInfo->requestFilename = fi.fileName();
                if (fileInfo->requestFilename.isEmpty()) {
                    fileInfo->requestFilename = "file";
                }
            }

            // add boundary
            requestContent.append(boundaryDelimiter);
            requestContent.append(boundary);
            requestContent.append(newLine);

            // add header
            requestContent.append(QString("Content-Disposition: form-data; %1; %2").arg(
                                      httpAttributeEncode("name", fileInfo->variableName),
                                      httpAttributeEncode("filename", fileInfo->requestFilename)
                                      ));
            requestContent.append(newLine);

            if (fileInfo->mimeType != NULL && !fileInfo->mimeType.isEmpty()) {
                requestContent.append("Content-Type: ");
                requestContent.append(fileInfo->mimeType);
                requestContent.append(newLine);
            }

            requestContent.append("Content-Transfer-Encoding: binary");
            requestContent.append(newLine);

            // add header to body splitter
            requestContent.append(newLine);

            // add file content
            requestContent.append(file.readAll());
            requestContent.append(newLine);

            file.close();
        }

        // add end of body
        requestContent.append(boundaryDelimiter);
        requestContent.append(boundary);
        requestContent.append(boundaryDelimiter);
    }


    // prepare connection

    QNetworkRequest request = QNetworkRequest(QUrl(input->urlStr));
    //! QNetworkRequest::UserAgentHeader is assosiated known header
    //request.setRawHeader("User-Agent", "Frumkin K. E. Test REST Client");
    request.setHeader(QNetworkRequest::UserAgentHeader, "Frumkin K. E. Test REST Client");

    if (input->varLayout == URL_ENCODED) {
        request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    }
    else if (input->varLayout == MULTIPART) {
        request.setHeader(QNetworkRequest::ContentTypeHeader, "multipart/form-data; boundary=" + boundary);
    }

    //! add custom headers
    foreach (const QByteArray& key, input->headers.keys()) {
        request.setRawHeader(key, input->headers[key]);
    }

    if (input->httpMethod == "GET") {
        manager->get(request);
    }
    else if (input->httpMethod == "POST") {
        manager->post(request, requestContent);
    }
    else if (input->httpMethod == "PUT") {
        manager->put(request, requestContent);
    }
    else if (input->httpMethod == "HEAD") {
        manager->head(request);
    }
    else if (input->httpMethod == "DELETE") {
        manager->deleteResource(request);
    }
    else {
        QBuffer buff(&requestContent);
        manager->sendCustomRequest(request, input->httpMethod.toLatin1(), &buff);
    }

}

void HttpRequestWorker::managerFinished(QNetworkReply *reply) {
    errorType = reply->error();
    if (errorType == QNetworkReply::NoError) {
        response = reply->readAll();
    }
    else {
        errorStr = reply->errorString();
    }

    reply->deleteLater();

    emit executionFinished(this);
}
