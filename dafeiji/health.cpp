#include "health.h"
#include <QFont>

Health::Health(QGraphicsTextItem *parent) : QGraphicsTextItem(parent)
{
    this->health = 100;
    setPlainText(QString("Health: ") + QString::number(health));
    setDefaultTextColor(Qt::gray);
    setFont(QFont("times", 16));
}

Health::~Health()
{
    delete this->Healths;
}

void Health::getHealth(int& health)
{
    health = this->health;
}

void Health::deHealth()
{
    --this->health;
    setPlainText(QString("Health: ") + QString::number(health));
}
