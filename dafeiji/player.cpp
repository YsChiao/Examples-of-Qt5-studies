#include "resource.h"
#include "player.h"
#include "bullet.h"
#include <QKeyEvent>

Player::Player(const QPixmap& hero, const QPixmap& bullet, QGraphicsPixmapItem *parent) : QGraphicsPixmapItem(parent)
{
    setPixmap(hero);
    this->bullet = bullet;
    bulletSound = new QMediaPlayer;
    bulletSound->setMedia(QUrl("C:/Users/Yisong Qiao/Documents/Qt/examples/dafeiji/resource/shoot.mp3"));
}

void Player::keyPressEvent(QKeyEvent *event)
{

    if (event->key() == Qt::Key_Left)
    {
        if (this->pos().x() > 0)
        {
            this->setPos(this->pos().x()-10, this->pos().y());
        }
    }
    else if (event->key() == Qt::Key_Right)
    {
        if (this->pos().x() + this->pixmap().width() < Resource::instance()->getWidth())
        {
            this->setPos(this->pos().x()+10, this->pos().y());
        }
    }
    else if (event->key() == Qt::Key_Up)
    {
        if (this->pos().y() > 10)
        {
            this->setPos(this->pos().x(), this->pos().y()-10) ;
        }
    }
    else if (event->key() == Qt::Key_Down)
    {
        if (this->pos().y() + this->pixmap().height() < Resource::instance()->getHeight())
        {
            this->setPos(this->pos().x(), this->pos().y()+10);
        }
    }
    else if (event->key() == Qt::Key_Space)
    {
        Bullet *bullet_left = new Bullet(this->bullet);
        bullet_left->setPos(this->pos().x(), this->pos().y()); // set the position of bullet left
        this->scene()->addItem(bullet_left);

        Bullet *bullet_right = new Bullet(this->bullet);
        qreal offset = this->pos().x() + this->pixmap().width() - this->bullet.width();
        bullet_right->setPos(offset, this->pos().y()); // set the position of bullet right
        this->scene()->addItem(bullet_right);


        // fire bullet with bulletsound
        if (bulletSound->state() == QMediaPlayer::PlayingState)
        {
            bulletSound->setPosition(0);
        }
        else if (bulletSound->state() == QMediaPlayer::StoppedState)
        {
            bulletSound->play();
        }
    }
}
