#include "resource.h"
#include <QDomAttr>
#include <QTimer>
#include <iostream>

Resource::Resource(QWidget*parent) : QGraphicsView(parent)
{
    // set size of window
    this->WindowWidth = 480;
    this->WindowHeight = 600;

    // create a document
    QDomDocument doc;
    QFile file(":/resource/plane.xml");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "resource file open error!";
        std::exit(1);
    }
    else
    {
        // loading
        if(!doc.setContent(&file))
        {
            qDebug() << "resource loads error!";
            std::exit(1);
        }
        file.close();
    }

    // gettting root element, get plane.jpg
    QDomElement docElem = doc.documentElement();
    QString tag = docElem.tagName();
    QString path = docElem.attributeNode("imagePath").value();
    std::cout << tag.toStdString() << " " << "Path" << path.toStdString() << std::endl;

    // plane image with all attribute of charactor images;
    Plane = QPixmap(path);

    // load each element
    QDomNode n = docElem.firstChild();
    while(!n.isNull())
    {
        QDomElement e = n.toElement(); // try to convert the node to an element
        if(!e.isNull())
        {
            std::cout << qPrintable(e.tagName()) << " ";
            if(e.hasAttribute("name"))
            {
                QString name = e.attributeNode("name").value();
                std::cout << "Name: " << name.toStdString() << " ";

                QString x, y, width, height;

                if(e.hasAttribute("x"))
                {
                    x = e.attributeNode("x").value();
                    std::cout << "x: " << x.toStdString() << " ";
                }
                if(e.hasAttribute("y"))
                {
                    y = e.attributeNode("y").value();
                    std::cout << "y: " << y.toStdString() << " ";
                }
                if(e.hasAttribute("width"))
                {
                    width = e.attributeNode("width").value();
                    std::cout << "width: " << width.toStdString() << " ";
                }
                if(e.hasAttribute("height"))
                {
                    height = e.attributeNode("height").value();
                    std::cout << "height: " << height.toStdString() << " ";
                    std::cout << std::endl;
                }
                if (name.toStdString() == "hero_1")
                {
                    QRect rect(x.toInt(), y.toInt(), width.toInt(), height.toInt());
                    hero_01 = Plane.copy(rect);
                }
                if (name.toStdString() == "hero_2")
                {
                    QRect rect(x.toInt(), y.toInt(), width.toInt(), height.toInt());
                    hero_02 = Plane.copy(rect);
                }
                if (name.toStdString() == "bullet_1") // blue bullet
                {
                    QRect rect(x.toInt(), y.toInt(), width.toInt(), height.toInt());
                    bullet_01 = Plane.copy(rect);
                }
                if (name.toStdString() == "bullet_0") // yellow bullet
                {
                    QRect rect(x.toInt(), y.toInt(), width.toInt(), height.toInt());
                    bullet_02 = Plane.copy(rect);
                }
                if (name.toStdString() == "enemy_s")
                {
                    QRect rect(x.toInt(), y.toInt(), width.toInt(), height.toInt());
                    enemy_s = Plane.copy(rect);
                }
                if (name.toStdString() == "enemy_m")
                {
                    QRect rect(x.toInt(), y.toInt(), width.toInt(), height.toInt());
                    enemy_m = Plane.copy(rect);
                }
                if (name.toStdString() == "enemy_b")
                {
                    QRect rect(x.toInt(), y.toInt(), width.toInt(), height.toInt());
                    enemy_b = Plane.copy(rect);
                }
            }
        }
        n = n.nextSibling();

    }
}

Resource::~Resource()
{
    delete resource;
    qDebug() <<"resource delete";
}


void Resource::getPlane(QPixmap &Plane)
{
    Plane = this->Plane;
}

void Resource::getHero_01(QPixmap &hero_01)
{
    hero_01 = this->hero_01;
}

void Resource::getHero_02(QPixmap &hero_02)
{
    hero_02 = this->hero_02;
}

void Resource::getBullet_01(QPixmap &bullet_01)
{
    bullet_01 = this->bullet_01;
}

void Resource::getBullet_02(QPixmap &bullet_02)
{
    bullet_02 = this->bullet_02;
}

void Resource::getEnemy_s(QPixmap &enemy_s)
{
    enemy_s = this->enemy_s;
}

void Resource::getEnemy_m(QPixmap &enemy_m)
{
    enemy_m = this->enemy_m;
}

void Resource::getEnemy_b(QPixmap &enemy_b)
{
    enemy_b = this->enemy_b;
}

int Resource::getWidth()
{
    return this->WindowWidth;
}

int Resource::getHeight()
{
    return this->WindowHeight;
}






