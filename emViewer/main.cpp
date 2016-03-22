#include "emviewer.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    EMviewer w;
    //w.resize(w.sizeHint());
    w.show();

    return a.exec();
}
