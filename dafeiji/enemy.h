#ifndef ENEMY_H
#define ENEMY_H

#include <QObject>
#include <QGraphicsPixmapItem>
#include <QGraphicsItem>


class Enemy : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    Enemy(const QPixmap& enemyPix, const QPixmap& bulletPix, QGraphicsPixmapItem * parent = 0);

signals:
    void isCreated();

public slots:
    void move();
    void fire();

private:
    QPixmap bulletPix;
};

#endif // ENEMY_H
