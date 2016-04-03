#include "bullet.h"
#include "player.h"
#include "enemy.h"
#include "resource.h"
#include "scores.h"
#include "health.h"
#include <typeinfo>
#include <QTimer>
#include <QList>
#include <QDebug>
#include <QGraphicsScene>

Bullet::Bullet(const QPixmap& bulletPix, bool up, QGraphicsItem *parent) : up(true), QGraphicsPixmapItem(parent)
{

    explosionSound = new QMediaPlayer;
    explosionSound->setMedia(QUrl("C:/Users/Yisong Qiao/Documents/Qt/examples/dafeiji/resource/explosion.mp3"));

    setPixmap(bulletPix);
    this->up = up;

    QTimer* timer = new QTimer(this);
    QObject::connect(timer, SIGNAL(timeout()), this, SLOT(move()));
    timer->start(20);


}

void Bullet::move()
{

    // get a list of all the items currently colliding with the bullet
    QList<QGraphicsItem*> colliding_items = collidingItems();


    if (this->up) // bullet downward
    {
        // within window
        if (this->pos().y() + this->pixmap().height() > 0 && this->pos().y() < Resource::instance()->getHeight())
        {

            this->setPos(this->pos().x(), this->pos().y()-10);

            // if one of the colliding items is an enmey, destroy both the bullet and the enemy
            for(int i = 0; i < colliding_items.size(); i++)
            {
                if ((typeid(*colliding_items.at(i))== typeid(Enemy)) || (typeid(*colliding_items.at(i)) == typeid(Bullet)))
                {
                    if (typeid(*colliding_items.at(i)) == typeid(Enemy))
                    {
                        Scores::instance()->incScores();
                    }
                    this->scene()->removeItem(colliding_items.at(i));
                    this->scene()->removeItem(this);

                    delete colliding_items.at(i);
                    delete this;
                    qDebug() << "enemy delete, bullet delete";
                }
                return;
            }
        }
        else
        {
            this->scene()->removeItem(this);
            delete this;
            qDebug() <<"bullet delete";
            return ;
        }
    }
    else // upward
    {
        // within window
        if (this->pos().y() + this->pixmap().height() > 0 && this->pos().y() < Resource::instance()->getHeight())
        {
            this->setPos(this->pos().x(), this->pos().y()+30);
            // if one of the colliding items is an hero, destroy the bullet
            for(int i = 0; i < colliding_items.size(); i++)
            {
                if ((typeid(*colliding_items.at(i))== typeid(Player)) || (typeid(*colliding_items.at(i)) == typeid(Bullet)))
                {
                    if (typeid(*colliding_items.at(i)) == typeid(Player))
                    {
                        Health::instance()->deHealth();

                        // explosion sound
                        if (explosionSound->state() == QMediaPlayer::PlayingState)
                        {
                            explosionSound->setPosition(0);
                        }
                        else if (explosionSound->state() == QMediaPlayer::StoppedState)
                        {
                            explosionSound->play();
                        }

                    }
                    this->scene()->removeItem(this);
                    delete this;
                    qDebug() << "Health decrease, bullet delete";
                }
                return;
            }
        }
        else
        {
            this->scene()->removeItem(this);
            delete this;
            qDebug() <<"bullet delete";
            return ;
        }
    }
}
