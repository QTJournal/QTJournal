#include "externallink.h"

ExternalLink::ExternalLink()
{

}

QUrl ExternalLink::getDataUrl() const
{
    return dataUrl;
}

void ExternalLink::setDataUrl(const QUrl &value)
{
    dataUrl = value;
}

QString ExternalLink::getDomain() const
{
    return domain;
}

void ExternalLink::setDomain(const QString &value)
{
    domain = value;
}

QUrl ExternalLink::getUrl() const
{
    return url;
}

void ExternalLink::setUrl(const QUrl &value)
{
    url = value;
}
