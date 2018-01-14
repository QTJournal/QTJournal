#ifndef LIKES_H
#define LIKES_H

#include <QString>



class Likes
{
public:
    Likes();

    int getCount() const;
    void setCount(int value);

    int getSum() const;
    void setSum(int value);

    int getIsLiked() const;
    void setIsLiked(int value);

    bool getIsHidden() const;
    void setIsHidden(bool value);

private:
    int count;
    int sum;
    int isLiked;
    bool isHidden;
};

#endif // LIKES_H
