#ifndef POST_H
#define POST_H

#include "user.h"

#include <QDateTime>
#include <QObject>
#include <QString>
#include <QUrl>

class Post : public QObject
{
    Q_OBJECT
public:
    explicit Post(QObject *parent = 0);

    static void fromString(QString string);

    int getId() const;
    void setId(int value);

    QString getTitle() const;
    void setTitle(const QString &value);

    QUrl getUrl() const;
    void setUrl(const QUrl &value);

    QDateTime getDate() const;
    void setDate(const QDateTime &value);

    QString getIntro() const;
    void setIntro(const QString &value);

    bool getIsReadMore() const;
    void setIsReadMore(bool value);

    long getHits() const;
    void setHits(long value);

    int getCommentsCount() const;
    void setCommentsCount(int value);

    bool getIsFavorited() const;
    void setIsFavorited(bool value);

    QUrl getMobileAppUrl() const;
    void setMobileAppUrl(const QUrl &value);

    bool getIsDraft() const;
    void setIsDraft(bool value);

    bool getIsGold() const;
    void setIsGold(bool value);

    bool getIsVotingActive() const;
    void setIsVotingActive(bool value);

    bool getIsWide() const;
    void setIsWide(bool value);

    bool getIsAdvertising() const;
    void setIsAdvertising(bool value);

    bool getIsCommentsClosed() const;
    void setIsCommentsClosed(bool value);

    bool getIsStillUpdating() const;
    void setIsStillUpdating(bool value);

    bool getIsComplexMarkup() const;
    void setIsComplexMarkup(bool value);

    bool getIsBigPicture() const;
    void setIsBigPicture(bool value);

    User *getAuthor() const;
    void setAuthor(User *value);

    User *getPublicAuthor() const;
    void setPublicAuthor(User *value);

private:
    int id;
    QString title;
    QUrl url;
    QDateTime date;
    //dateRFC
    User* author;
    User* publicAuthor;
    //type should be enum
    QString intro;
    //Cover cover;
    //ExternalLink
    //inspired by
    bool isReadMore;
    long hits;
    //Likes
    int commentsCount;
    bool isFavorited;
    //userDevice
    QUrl mobileAppUrl;
    bool isDraft;
    bool isGold;
    bool isVotingActive;
    bool isWide;
    bool isAdvertising;
    bool isCommentsClosed;
    bool isStillUpdating;
    bool isComplexMarkup;
    bool isBigPicture;
    ~Post();

signals:

public slots:

public:
    class Cover
    {
    public:
        Cover() {}
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
};

#endif // POST_H
