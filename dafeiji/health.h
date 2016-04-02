#ifndef HEALTH_H
#define HEALTH_H

#include <QGraphicsTextItem>


class Health : public QGraphicsTextItem
{
public:
    Health(QGraphicsTextItem* parent = 0);
    void getHealth(int& health);
    void deHealth();

private:
    int health;
};

#endif // HEALTH_H
