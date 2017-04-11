#ifndef PARSERUTIL_H
#define PARSERUTIL_H

#include "cover.h"
#include "externallink.h"
#include "likes.h"
#include "post.h"
#include "user.h"

#include <QJsonObject>
#include <QObject>

class ParserUtil : public QObject
{
    Q_OBJECT
public:
    static User *parseUser(QJsonObject user);
    static Cover *parseCover(QJsonObject cover);
    static Likes *parseLikes(QJsonObject likes);
    static ExternalLink *parseExternalLink(QJsonObject externalLink);
    static Post *parsePost(QJsonObject post);

private:
    explicit ParserUtil(QObject *parent = 0);

signals:

public slots:
};

#endif // PARSERUTIL_H
