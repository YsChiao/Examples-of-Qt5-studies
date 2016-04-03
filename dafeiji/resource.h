#ifndef RESOURCE_H
#define RESOURCE_H

#include <QWidget>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QtCore>
#include <QtXml>
#include <QtDebug>


class Resource : public QGraphicsView
{
public:
    Resource(QWidget *parent = 0);
    ~Resource();

    static Resource* instance()
    {
        if (!resource)
        {
            resource = new Resource;
        }
        return resource;
    }

    void getPlane(QPixmap& Plane);
    void getHero_01(QPixmap& hero_01);
    void getHero_02(QPixmap& hero_02);
    void getBullet_01(QPixmap& bullet_01);
    void getBullet_02(QPixmap& bullet_02);
    void getEnemy_s(QPixmap& enemy_s);
    void getEnemy_m(QPixmap& enemy_m);
    void getEnemy_b(QPixmap& enemy_b);

    int getWidth();
    int getHeight();

private:
    static Resource *resource;
    QPixmap Plane;
    QPixmap hero_01;
    QPixmap hero_02;
    QPixmap bullet_01;
    QPixmap bullet_02;

    QPixmap enemy_s;
    QPixmap enemy_m;
    QPixmap enemy_b;

    int WindowWidth;
    int WindowHeight;
};


#endif // RESOURCE_H
