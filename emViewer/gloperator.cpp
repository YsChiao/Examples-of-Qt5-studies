#include "gloperator.h"
#include "ui_gloperator.h"

glOperator::glOperator(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::glOperator)
{
    ui->setupUi(this);

    // set dicimal digital, 0.1
    magnifier = 10;

    // set title of tools windows
    setWindowTitle("tools");

    // remoe the border of lineEdit
    ui->lineEdit->setStyleSheet("qproperty-frame: false");

    QObject::connect(ui->slider, SIGNAL(valueChanged(int)), this, SLOT(getLevelValue(int)));
    QObject::connect(ui->slider, SIGNAL(valueChanged(int)), this, SLOT(setLevelValue(int)));

}

glOperator::~glOperator()
{
    delete ui;
}

void glOperator::MaxMin(float max, float min)
{
    ui->maxValue->setText(QString::number(max));
    ui->minValue->setText(QString::number(min));

    // set silder range
    ui->slider->setMaximum(max*magnifier);
    ui->slider->setMinimum(min*magnifier);
}

void glOperator::FileName(QString &name)
{
    ui->lineEdit->setText(name);
}

void glOperator::setLevelValue(int value)
{
    // set dicimal digits, 0.1
    ui->levelValue->setText(QString::number(float(value)/magnifier));
}

void glOperator::getLevelValue(int value)
{
    emit sentLevelValue(value);
}
