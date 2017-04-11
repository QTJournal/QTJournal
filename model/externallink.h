#ifndef EXTERNALLINK_H
#define EXTERNALLINK_H

#include <QUrl>



class ExternalLink
{
public:
    ExternalLink();

    QUrl getDataUrl() const;
    void setDataUrl(const QUrl &value);

    QString getDomain() const;
    void setDomain(const QString &value);

    QUrl getUrl() const;
    void setUrl(const QUrl &value);

private:
    QUrl dataUrl;
    QString domain;
    QUrl url;
};

#endif // EXTERNALLINK_H
