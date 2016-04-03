#include "game.h"
#include "scores.h"
#include "health.h"
#include "resource.h"
#include <QApplication>

Resource *Resource::resource = nullptr;
Scores *Scores::Score = nullptr;
Health *Health::Healths = nullptr;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Game w;
    w.show();

    return a.exec();
}
