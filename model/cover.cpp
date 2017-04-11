#include "cover.h"

Cover::Cover()
{

}

QUrl Cover::getThumbnailUrl() const
{
    return thumbnailUrl;
}

void Cover::setThumbnailUrl(const QUrl &value)
{
    thumbnailUrl = value;
}

QUrl Cover::getUrl() const
{
    return url;
}

void Cover::setUrl(const QUrl &value)
{
    url = value;
}

int Cover::getWidth() const
{
    return width;
}

void Cover::setWidth(int value)
{
    width = value;
}

int Cover::getHeight() const
{
    return height;
}

void Cover::setHeight(int value)
{
    height = value;
}
