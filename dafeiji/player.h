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
    Player(QGraphicsPixmapItem* parent = 0);
    void keyPressEvent(QKeyEvent* event);

private:
    QMediaPlayer *bulletSound;
};

#endif // PLAYER_H
