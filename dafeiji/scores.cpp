#include "scores.h"
#include <QFont>
#include <QDebug>


Scores::Scores(QGraphicsItem *parent) : QGraphicsTextItem(parent)
{
    this->scores = 0;
    //draw text
    setPlainText(QString("Score: ") + QString::number(this->scores));
    setDefaultTextColor(Qt::gray);
    setFont(QFont("times", 16));
}

Scores::~Scores()
{
    delete this->Score;
}

void Scores::getScores(int &scores)
{
    scores = this->scores;
}

void Scores::incScores()
{
    qDebug() << "++scores";
    ++this->scores;
    setPlainText(QString("Score: ") + QString::number(this->scores));

}
