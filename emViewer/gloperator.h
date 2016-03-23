#ifndef GLOPERATOR_H
#define GLOPERATOR_H

#include <QWidget>
#include <QDialog>
#include <QString>

namespace Ui {
class glOperator;
}

class glOperator : public QDialog
{
    Q_OBJECT

public:
    explicit glOperator(QWidget *parent = 0);
    ~glOperator();

signals:
    void sentLevelValue(int value);

public slots:
    void MaxMin(float max, float min);
    void FileName(QString& name);

    void setLevelValue(int value);
    void getLevelValue(int value);

private:
    Ui::glOperator *ui;
    int magnifier; // decimal digits
};

#endif // GLOPERATOR_H
