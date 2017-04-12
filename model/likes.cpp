#include "likes.h"

Likes::Likes()
{

}

int Likes::getCount() const
{
    return count;
}

void Likes::setCount(int value)
{
    count = value;
}

int Likes::getSum() const
{
    return sum;
}

void Likes::setSum(int value)
{
    sum = value;
}

int Likes::getIsLiked() const
{
    return isLiked;
}

void Likes::setIsLiked(int value)
{
    isLiked = value;
}

bool Likes::getIsHidden() const
{
    return isHidden;
}

void Likes::setIsHidden(bool value)
{
    isHidden = value;
}

QString Likes::getHash() const
{
    return hash;
}

void Likes::setHash(const QString &value)
{
    hash = value;
}
