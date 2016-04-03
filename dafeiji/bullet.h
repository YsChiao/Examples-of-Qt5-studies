#ifndef BULLET_H
#define BULLET_H
#include <QGraphicsPixmapItem>
#include <QGraphicsItem>
#include <QMediaPlayer>
#include <QObject>

class Bullet : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT

public:
    Bullet(const QPixmap& bulletPix, bool up = true, QGraphicsItem* parent = 0);

signals:
    void scoreIncrease();

public slots:
    void move();

private:
    bool up; // control the bullet direction, up or down. default is down
    QMediaPlayer *explosionSound;


};

#endif // BULLET_H
