#ifndef COVER_H
#define COVER_H

#include <QUrl>

class Cover
{
public:
    Cover();

    QUrl getThumbnailUrl() const;
    void setThumbnailUrl(const QUrl &value);

    QUrl getUrl() const;
    void setUrl(const QUrl &value);

    int getWidth() const;
    void setWidth(int value);

    int getHeight() const;
    void setHeight(int value);

private:
    //type enum
    //additional data
    QUrl thumbnailUrl;
    QUrl url;
    int width;
    int height;
};

#endif // COVER_H
