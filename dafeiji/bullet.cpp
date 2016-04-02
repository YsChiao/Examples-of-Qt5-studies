#include "bullet.h"
#include <QTimer>
#include <QGraphicsScene>

Bullet::Bullet(QGraphicsItem *parent) : QObject(), QGraphicsPixmapItem(parent)
{
    setPixmap(QPixmap(":/images/bullet.png").scaled(30,30, Qt::IgnoreAspectRatio,
                                                    Qt::SmoothTransformation));
    this->setRotation(270);

    QTimer* timer = new QTimer(this);
    QObject::connect(timer, SIGNAL(timeout()), this, SLOT(move()));
    timer->start(50);


}

void Bullet::move()
{
    if (this->pos().y() + 30 > 0)
    {
        this->setPos(this->pos().x(), this->pos().y()-10);
    }
    else
    {
        this->scene()->removeItem(this);
        delete this;
    }

}
