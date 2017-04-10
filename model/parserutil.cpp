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

ParserUtil::ParserUtil(QObject *parent) : QObject(parent)
{

}
