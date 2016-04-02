#include "scores.h"
#include <QFont>


Scores::Scores(QGraphicsItem *parent) : QGraphicsTextItem(parent)
{
    scores = 0;

    //draw text
    setPlainText(QString("Score: ") + QString::number(scores));
    setDefaultTextColor(Qt::gray);
    setFont(QFont("times", 16));
}

void Scores::getScores(int &scores)
{
    scores = this->scores;
}

void Scores::incScores()
{
    ++this->scores;
}
