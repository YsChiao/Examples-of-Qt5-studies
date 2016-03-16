#ifndef EMVIEWER_H
#define EMVIEWER_H

#include <QMainWindow>
#include <QLabel>

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

private:
    Ui::EMviewer *ui;
    QLabel *statusLabel;
};

#endif // EMVIEWER_H
