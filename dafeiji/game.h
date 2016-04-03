#ifndef GAME_H
#define GAME_H

#include <QWidget>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QtCore>
#include <QtXml>
#include <QtDebug>
#include "player.h"
#include "scores.h"
#include "health.h"

class Game : public QGraphicsView
{
    Q_OBJECT

public:
    Game(QWidget *parent = 0);
    ~Game();

signals:
    void enemyAppear();

public slots:
    void spawn();


private:
    QGraphicsScene *scene;
    Player *player;
    Scores *score;
    Health *health;

};

#endif // GAME_H
