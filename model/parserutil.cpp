#include "externallink.h"
#include "parserutil.h"

#include <QJsonObject>
#include <QJsonArray>

User *ParserUtil::parseUser(QJsonObject user)
{
    User* userModel = new User();
    userModel->setId(user["id"].toInt());
    userModel->setName(user["name"].toString());
    userModel->setProfileImageUrl(QUrl(user["avatar_url"].toString()));
    return userModel;
}

Cover *ParserUtil::parseCover(QJsonObject cover)
{
    Cover* coverModel = new Cover();
    coverModel->setUrl(QUrl(cover["url"].toString()));
    coverModel->setThumbnailUrl(QUrl(cover["thumbnailUrl"].toString()));
    QJsonObject size = cover["size"].toObject();
    coverModel->setHeight(size["height"].toInt());
    coverModel->setWidth(size["width"].toInt());
    return coverModel;
}

Likes *ParserUtil::parseLikes(QJsonObject likes)
{
    Likes *likesModel = new Likes();
    likesModel->setCount(likes["count"].toInt());
    likesModel->setSum(likes["summ"].toInt());
    likesModel->setIsLiked(likes["is_liked"].toInt());
    likesModel->setIsHidden(likes["is_hidden"].toBool());
    return likesModel;
}

ExternalLink *ParserUtil::parseExternalLink(QJsonObject externalLink)
{
    ExternalLink *externalLinkModel = new ExternalLink();
    externalLinkModel->setDomain(externalLink["domain"].toString());
    externalLinkModel->setUrl(QUrl(externalLink["url"].toString()));
    externalLinkModel->setDataUrl(QUrl(externalLink["data"].toObject()["url"].toString()));
    return externalLinkModel;
}

Post *ParserUtil::parsePost(QJsonObject post)
{
    Post *postModel = new Post();
    postModel->setId(post["id"].toInt());
    postModel->setTitle(post["title"].toString());

    postModel->setBadges(ParserUtil::parseBadges(post["badges"].toArray()));

    QDateTime date = QDateTime::fromSecsSinceEpoch(post["date"].toInt());
    postModel->setDate(date);

    postModel->setIntro(post["intro"].toString());
    postModel->setEntryJSON(post["entryJSON"].toString());
    postModel->setIsReadMore(post["isReadMore"].toBool());
    postModel->setHits(post["hitsCount"].toInt());
    postModel->setCommentsCount(post["commentsCount"].toInt());
    postModel->setIsFavorited(post["isFavorited"].toBool());
    postModel->setMobileAppUrl(QUrl(post["mobileAppUrl"].toString()));
    postModel->setIsDraft(post["isDraft"].toBool());
    postModel->setIsGold(post["isGold"].toBool());
    postModel->setIsVotingActive(post["isVotingActive"].toBool());
    postModel->setIsWide(post["isWide"].toBool());
    postModel->setIsAdvertising(post["isAdvertising"].toBool());
    postModel->setIsCommentsClosed(post["isCommentsClosed"].toBool());
    postModel->setIsStillUpdating(post["isStillUpdating"].toBool());

    postModel->setAuthor(ParserUtil::parseUser(post["author"].toObject()));
    postModel->setPublicAuthor(ParserUtil::parseUser(post["publicAuthor"].toObject()));

    postModel->setCover(ParserUtil::parseCover(post["cover"].toObject()));
    postModel->setExternalLink(ParserUtil::parseExternalLink(post["externalLink"].toObject()));
    postModel->setLikes(ParserUtil::parseLikes(post["likes"].toObject()));

    postModel->setType(post["type"].toInt());
    return postModel;
}

QList<Badge*> *ParserUtil::parseBadges(QJsonArray badges)
{
    QList<Badge*>* badgesList = new QList<Badge*>();
    for (int i = 0; i < badges.size(); i++) {
        QJsonObject badgeJson = badges.at(i).toObject();
        Badge* badge = new Badge();
        badge->setType(badgeJson["type"].toString());
        badge->setText(badgeJson["text"].toString());
        badge->setColor(QColor(badgeJson["color"].toString()));
        badge->setBackground(QColor(badgeJson["background"].toString()));
        badge->setBorder(QColor(badgeJson["border"].toString()));
        badgesList->append(badge);
    }
    return badgesList;
}

ParserUtil::ParserUtil(QObject *parent) : QObject(parent)
{

}
