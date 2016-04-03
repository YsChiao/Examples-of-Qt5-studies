#ifndef HEALTH_H
#define HEALTH_H

#include <QGraphicsTextItem>


class Health : public QGraphicsTextItem
{
public:
    Health(QGraphicsTextItem* parent = 0);
    ~Health();
    void getHealth(int& health);
    void deHealth();

    static Health* instance()
    {
        if (!Healths)
        {
            Healths = new Health;
        }
        return Healths;
    }

private:
    static Health* Healths;
    int health;
};

#endif // HEALTH_H
