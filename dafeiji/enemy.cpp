#include "enemy.h"
#include "bullet.h"
#include "resource.h"
#include <QTimer>
#include <algorithm>

Enemy::Enemy(const QPixmap &enemyPix, const QPixmap &bulletPix, QGraphicsPixmapItem *parent) : QGraphicsPixmapItem(parent)
{
    // set random x position
    this->setPixmap(enemyPix);
    int random_number  = std::rand() % (Resource::instance()->getWidth() - this->pixmap().width());
    this->setPos(random_number, 0);

    // bullet image
    this->bulletPix = bulletPix;

    // movenent of enemy
    QTimer* timer = new QTimer(this);
    QObject::connect(timer, SIGNAL(timeout()), this, SLOT(move()));
    timer->start(50);
}


void Enemy::move()
{
    // move
    if (this->pos().y() < Resource::instance()->getHeight())
    {
        this->setPos(this->pos().x(), this->pos().y()+5);
    }
    else
    {
        this->scene()->removeItem(this);
        qDebug() <<"enemy delete";
        delete this;
    }
}

void Enemy::fire()
{
    // after the enemy create, create bullet
    Bullet * bullet  = new Bullet(this->bulletPix, 0);
    bullet->setPos(this->pos().x()+ this->pixmap().width()/2 - this->bulletPix.width()/2,
                   this->pos().y()+ this->pixmap().height()); // set the position of bullet right
    this->scene()->addItem(bullet);
    QTimer* timer = new QTimer(this);
    QObject::connect(timer, SIGNAL(timeout()), bullet, SLOT(move()));
}
