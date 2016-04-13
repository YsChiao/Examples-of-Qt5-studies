#ifndef EMVIEWER_H
#define EMVIEWER_H

#include <QMainWindow>
#include <QLabel>
#include "object.h"
#include "tools.h"

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
    void show_Message_toolWidget(QString &Message);

private:
    Ui::EMviewer *ui;
    QLabel *statusLabel;
    Tools *toolsWidget;
};

#endif // EMVIEWER_H
