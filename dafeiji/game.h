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

    void xmlParser(QDomElement& root, QString tag, Qtring att);


    // game window size
    static const int WindowsHeight = 480;
    static const int WindowsWidth = 600;

private:
    QGraphicsScene *scene;
    Player *player;
    Scores *score;
    Health *health;

};

#endif // GAME_H
