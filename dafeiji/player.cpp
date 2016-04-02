#include "player.h"
#include <QKeyEvent>
#include "game.h"
#include "bullet.h"


Player::Player(QGraphicsPixmapItem *parent) : QGraphicsPixmapItem(parent)
{
    //bulletSound = new QMediaPlayer;
    setPixmap(QPixmap(":/images/tank.png"));
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
        if (this->pos().x() + 50 < Game::WindowsWidth)
        {
            this->setPos(this->pos().x()+10, this->pos().y());
        }
    }
    else if (event->key() == Qt::Key_Up)
    {
        if (this->pos().y() > 0)
        {
            this->setPos(this->pos().x(), this->pos().y()-10);
        }
    }
    else if (event->key() == Qt::Key_Down)
    {
        if (this->pos().y() + 50 < Game::WindowsWidth)
        {
            this->setPos(this->pos().x(), this->pos().y()+10);
        }
    }
    else if (event->key() == Qt::Key_Space)
    {
        Bullet *bullet = new Bullet();
        bullet->setPos(this->pos().x()+9, this->pos().y()); // set the position of bullet
        this->scene()->addItem(bullet);
    }
}
