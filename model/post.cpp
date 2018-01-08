#include "post.h"

Post::Post(QObject *parent) : QObject(parent)
{

}

void Post::fromString(QString string)
{

}

int Post::getId() const
{
    return id;
}

void Post::setId(int value)
{
    id = value;
}

QString Post::getTitle() const
{
    return title;
}

void Post::setTitle(const QString &value)
{
    title = value;
}

QDateTime Post::getDate() const
{
    return date;
}

void Post::setDate(const QDateTime &value)
{
    date = value;
}

QString Post::getIntro() const
{
    return intro;
}

void Post::setIntro(const QString &value)
{
    intro = value;
}

bool Post::getIsReadMore() const
{
    return isReadMore;
}

void Post::setIsReadMore(bool value)
{
    isReadMore = value;
}

long Post::getHits() const
{
    return hits;
}

void Post::setHits(long value)
{
    hits = value;
}

int Post::getCommentsCount() const
{
    return commentsCount;
}

void Post::setCommentsCount(int value)
{
    commentsCount = value;
}

bool Post::getIsFavorited() const
{
    return isFavorited;
}

void Post::setIsFavorited(bool value)
{
    isFavorited = value;
}

QUrl Post::getMobileAppUrl() const
{
    return mobileAppUrl;
}

void Post::setMobileAppUrl(const QUrl &value)
{
    mobileAppUrl = value;
}

bool Post::getIsDraft() const
{
    return isDraft;
}

void Post::setIsDraft(bool value)
{
    isDraft = value;
}

bool Post::getIsGold() const
{
    return isGold;
}

void Post::setIsGold(bool value)
{
    isGold = value;
}

bool Post::getIsVotingActive() const
{
    return isVotingActive;
}

void Post::setIsVotingActive(bool value)
{
    isVotingActive = value;
}

bool Post::getIsWide() const
{
    return isWide;
}

void Post::setIsWide(bool value)
{
    isWide = value;
}

bool Post::getIsAdvertising() const
{
    return isAdvertising;
}

void Post::setIsAdvertising(bool value)
{
    isAdvertising = value;
}

bool Post::getIsCommentsClosed() const
{
    return isCommentsClosed;
}

void Post::setIsCommentsClosed(bool value)
{
    isCommentsClosed = value;
}

bool Post::getIsStillUpdating() const
{
    return isStillUpdating;
}

void Post::setIsStillUpdating(bool value)
{
    isStillUpdating = value;
}

bool Post::getIsComplexMarkup() const
{
    return isComplexMarkup;
}

void Post::setIsComplexMarkup(bool value)
{
    isComplexMarkup = value;
}

bool Post::getIsBigPicture() const
{
    return isBigPicture;
}

void Post::setIsBigPicture(bool value)
{
    isBigPicture = value;
}

User *Post::getAuthor() const
{
    return author;
}

void Post::setAuthor(User *value)
{
    author = value;
}

User *Post::getPublicAuthor() const
{
    return publicAuthor;
}

void Post::setPublicAuthor(User *value)
{
    publicAuthor = value;
}

QList<Badge *> *Post::getBadges() const
{
    return badges;
}

void Post::setBadges(QList<Badge *> *value)
{
    badges = value;
}

QString Post::getEntryJSON() const
{
    return entryJSON;
}

void Post::setEntryJSON(const QString &value)
{
    entryJSON = value;
}

void Post::setType(const PostTypes &value)
{
    type = value;
}

void Post::setType(const int value)
{
    type = static_cast<PostTypes>(value);
}

ExternalLink *Post::getExternalLink() const
{
    return externalLink;
}

void Post::setExternalLink(ExternalLink *value)
{
    externalLink = value;
}

Likes *Post::getLikes() const
{
    return likes;
}

void Post::setLikes(Likes *value)
{
    likes = value;
}

Cover *Post::getCover() const
{
    return cover;
}

void Post::setCover(Cover *value)
{
    cover = value;
}


Post::~Post()
{
    delete author;
    delete publicAuthor;
    delete cover;
    delete externalLink;
    delete likes;
}
