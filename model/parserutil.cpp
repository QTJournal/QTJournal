#include "externallink.h"
#include "parserutil.h"

#include <QJsonObject>

User *ParserUtil::parseUser(QJsonObject user)
{
    User* userModel = new User();
    userModel->setId(user["id"].toInt());
    userModel->setName(user["name"].toString());
    userModel->setProfileImageUrl(QUrl(user["profile_image_url"].toString()));
    userModel->setProfileBigImageUrl(QUrl(user["profile_big_image_url"].toString()));
    userModel->setUrl(QUrl(user["url"].toString()));
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
    likesModel->setSum(likes["sum"].toInt());
    likesModel->setIsLiked(likes["isLiked"].toInt());
    likesModel->setIsHidden(likes["isHidden"].toBool());
    likesModel->setHash(likes["hash"].toString());
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
    postModel->setUrl(QUrl(post["url"].toString()));

    QDateTime date = QDateTime::fromSecsSinceEpoch(post["date"].toInt());
    postModel->setDate(date);

    postModel->setIntro(post["intro"].toString());
    postModel->setEntryJSON(post["entryJSON"].toString());
    postModel->setIsReadMore(post["isReadMore"].toBool());
    postModel->setHits(post["hits"].toInt());
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

ParserUtil::ParserUtil(QObject *parent) : QObject(parent)
{

}
