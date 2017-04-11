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

ParserUtil::ParserUtil(QObject *parent) : QObject(parent)
{

}
