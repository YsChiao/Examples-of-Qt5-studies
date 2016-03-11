#include "webcontent.h"
#include <QApplication>

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);
  webContent w;
  w.show();

  return a.exec();
}
