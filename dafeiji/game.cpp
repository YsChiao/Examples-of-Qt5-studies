#include "game.h"
#include "enemy.h"
#include "resource.h"
#include <QIcon>



Game::Game(QWidget*parent) : QGraphicsView(parent)
{
    QPixmap hero_01;
    QPixmap bullet_01;
    Resource::instance()->getHero_01(hero_01);
    Resource::instance()->getBullet_01(bullet_01);

    int WindowsWidth, WindowsHeight;
    WindowsWidth = Resource::instance()->getWidth();
    WindowsHeight = Resource::instance()->getHeight();

    // set window
    setWindowIcon(QIcon(":/images/icon.png"));
    setWindowTitle("Fighter Fires");

    // set background
    scene = new QGraphicsScene();
    scene->setSceneRect(0,0,WindowsWidth,WindowsHeight); // make the scene 480x600 instead of infinity by infinit
    setBackgroundBrush(QBrush(QImage(":/resource/bg_01.jpg")));


    // make the scene visulized
    setScene(scene);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setFixedSize(WindowsWidth,WindowsHeight);

    // create the player
    player = new Player(hero_01, bullet_01);
    player->setPos(WindowsWidth/2 - hero_01.width()/2, WindowsHeight-hero_01.height());
    // make the player focusable and set it to be the current focus
    player->setFlag(QGraphicsItem::ItemIsFocusable);
    player->setFocus();
    // add the player to the scene
    scene->addItem(player);

    // add score/health
    Scores::instance()->setPos(WindowsWidth - 100, 0);
    scene->addItem(Scores::instance());
    scene->addItem(Health::instance());

    // enemies
    QTimer *timer = new QTimer;
    QObject::connect(timer, SIGNAL(timeout()), this, SLOT(spawn()));
    timer->start(2000);

}

Game::~Game()
{
}

void Game::spawn()
{
    QPixmap enemy_s, bullet_02;
    Resource::instance()->getEnemy_s(enemy_s);
    Resource::instance()->getBullet_02(bullet_02);
    Enemy * enemy = new Enemy(enemy_s, bullet_02);
    this->scene->addItem(enemy);
    emit enemyAppear();
    QObject::connect(this, SIGNAL(enemyAppear()), enemy, SLOT(fire()));

}
