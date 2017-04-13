#include "user.h"

User::User(QObject *parent) : QObject(parent)
{

}

int User::getId() const
{
    return id;
}

void User::setId(int value)
{
    id = value;
}

QString User::getName() const
{
    return name;
}

void User::setName(const QString &value)
{
    name = value;
}

QUrl User::getProfileImageUrl() const
{
    return profileImageUrl;
}

void User::setProfileImageUrl(const QUrl &value)
{
    profileImageUrl = value;
}

QUrl User::getProfileBigImageUrl() const
{
    return profileBigImageUrl;
}

void User::setProfileBigImageUrl(const QUrl &value)
{
    profileBigImageUrl = value;
}

QUrl User::getUrl() const
{
    return url;
}

void User::setUrl(const QUrl &value)
{
    url = value;
}
