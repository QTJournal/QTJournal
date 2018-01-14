#include "badge.h"

Badge::Badge()
{

}

QString Badge::getType() const
{
    return type;
}

void Badge::setType(const QString &value)
{
    type = value;
}

QString Badge::getText() const
{
    return text;
}

void Badge::setText(const QString &value)
{
    text = value;
}

QColor Badge::getBackground() const
{
    return background;
}

void Badge::setBackground(const QColor &value)
{
    background = value;
}

QColor Badge::getColor() const
{
    return color;
}

void Badge::setColor(const QColor &value)
{
    color = value;
}

QColor Badge::getBorder() const
{
    return border;
}

void Badge::setBorder(const QColor &value)
{
    border = value;
}
