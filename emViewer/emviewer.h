#ifndef EMVIEWER_H
#define EMVIEWER_H

#include <QMainWindow>

namespace Ui {
class EMviewer;
}

class EMviewer : public QMainWindow
{
    Q_OBJECT

public:
    explicit EMviewer(QWidget *parent = 0);
    ~EMviewer();

public slots:

private:
    Ui::EMviewer *ui;
};

#endif // EMVIEWER_H
