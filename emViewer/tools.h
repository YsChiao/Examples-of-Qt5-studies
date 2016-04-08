#ifndef TOOLS_H
#define TOOLS_H

#include <QWidget>
#include <QDialog>
#include <QString>
#include <QCloseEvent>

namespace Ui {
class Tools;
}

class Tools : public QDialog
{
    Q_OBJECT

public:
    explicit Tools(QWidget *parent = 0);
    ~Tools();

    void closeEvent(QCloseEvent* event);

signals:
    void sentLevelValue(int value);

public slots:
    void MaxMin(float max, float min);
    void FileName(QString& name);

    void setLevelValue(int value);
    void getLevelValue(int value);

    Ui::Tools* getUi();

private:
    Ui::Tools *ui;
    int magnifier; // decimal digits
};

#endif // TOOLS_H
