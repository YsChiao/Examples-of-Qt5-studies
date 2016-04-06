#include "game.h"
#include "scores.h"
#include "health.h"
#include "resource.h"
#include <QApplication>

Resource *Resource::resource = NULL;
Scores *Scores::Score = NULL;
Health *Health::Healths = NULL;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Game w;
    w.show();

    return a.exec();
}
