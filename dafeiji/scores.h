#ifndef SCORES_H
#define SCORES_H

#include <QGraphicsTextItem>


class Scores : public QGraphicsTextItem
{
public:
    Scores(QGraphicsItem* parent = 0);
    void getScores(int& scores);
    void incScores();

private:
    int scores;
};

#endif // SCORES_H
