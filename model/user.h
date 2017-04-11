#ifndef USER_H
#define USER_H

#include <QObject>
#include <QUrl>

class User : public QObject
{
    Q_OBJECT
public:
    explicit User(QObject *parent = 0);

    int getId() const;
    void setId(int value);

    QString getName() const;
    void setName(const QString &value);

    QUrl getProfileImageUrl() const;
    void setProfileImageUrl(const QUrl &value);

    QUrl getProfileBigImageUrl() const;
    void setProfileBigImageUrl(const QUrl &value);

    QUrl getUrl() const;
    void setUrl(const QUrl &value);

private:
    int id;
    QString name;
    QUrl profileImageUrl;
    QUrl profileBigImageUrl;
    QUrl url;

signals:

public slots:
};

#endif // USER_H
