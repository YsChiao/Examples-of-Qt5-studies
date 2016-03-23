#ifndef EMVIEWER_H
#define EMVIEWER_H

#include <QMainWindow>
#include <QLabel>
#include "gloperator.h"

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
    void showMessage(QString &Message);
    void toolsWindowShow();

private:
    Ui::EMviewer *ui;
    QLabel *statusLabel;
    glOperator *toolsWidget;
};

#endif // EMVIEWER_H
