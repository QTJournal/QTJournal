#ifndef PARSERUTIL_H
#define PARSERUTIL_H

#include "user.h"

#include <QJsonObject>
#include <QObject>

class ParserUtil : public QObject
{
    Q_OBJECT
public:
    static User *parseUser(QJsonObject user);
private:
    explicit ParserUtil(QObject *parent = 0);

signals:

public slots:
};

#endif // PARSERUTIL_H
