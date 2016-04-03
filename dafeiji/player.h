#ifndef PLAYER_H
#define PLAYER_H

#include <QGraphicsPixmapItem>
#include <QObject>
#include <QGraphicsItem>
#include <QMediaPlayer>


class Player : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    Player(const QPixmap& hero, const QPixmap& bullet, QGraphicsPixmapItem* parent = 0);
    void keyPressEvent(QKeyEvent* event);

private:
    QMediaPlayer *bulletSound;
    QPixmap hero;
    QPixmap bullet;
};

#endif // PLAYER_H
