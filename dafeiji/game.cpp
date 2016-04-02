#include "game.h"
#include <QIcon>


Game::Game(QWidget*parent) : QGraphicsView(parent)
{
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

    // gettting root element
    QDomElement root = doc.firstChildElement();

    // retrievElement






    // setwindow
    setWindowIcon(QIcon(":/images/icon.png"));
    setWindowTitle("Tank Fires");


    scene = new QGraphicsScene();
    scene->setSceneRect(0,0,WindowsHeight,WindowsWidth); // make the scene 800x600 instead of infinity by infinit
    setBackgroundBrush(QBrush(QImage(":/resource/bg_01.jpg")));


    // make the scene visulized
    setScene(scene);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setFixedSize(WindowsHeight,WindowsWidth);

    // create the player
    player = new Player();
    player->setPos(WindowsHeight/2-25, WindowsWidth-50);
    // make the player focusable and set it to be the current focus
    player->setFlag(QGraphicsItem::ItemIsFocusable);
    player->setFocus();
    // add the player to the scene
    scene->addItem(player);

    // create the score/health
    score = new Scores();
    score->setPos(WindowsHeight - 100, 0);
    scene->addItem(score);
    health = new Health();
    scene->addItem(health);


}

Game::~Game()
{
}

void Game::xmlParser(QDomElement &root, QString tag, Qtring att)
{
    QDomNodeList nodes = root.elementsByTagName(tag);
    for
























}
