#ifndef BADGE_H
#define BADGE_H

#include <QColor>
#include <QString>

class Badge
{
public:
    Badge();

    QString getType() const;
    void setType(const QString &value);

    QString getText() const;
    void setText(const QString &value);

    QColor getBackground() const;
    void setBackground(const QColor &value);

    QColor getColor() const;
    void setColor(const QColor &value);

    QColor getBorder() const;
    void setBorder(const QColor &value);

private:
    QString type;
    QString text;
    QColor background;
    QColor color;
    QColor border;
};

#endif // BADGE_H
