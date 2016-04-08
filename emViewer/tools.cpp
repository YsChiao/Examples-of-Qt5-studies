#include <QDebug>
#include <QDialog>
#include "tools.h"
#include "ui_tools.h"


Tools::Tools(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Tools)
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

Tools::~Tools()
{
    delete ui;
}

void Tools::closeEvent(QCloseEvent *event)
{

    event->accept();
}


void Tools::MaxMin(float max, float min)
{
    ui->maxValue->setText(QString::number(max));
    ui->minValue->setText(QString::number(min));

    // set silder range
    ui->slider->setMaximum(max*magnifier);
    ui->slider->setMinimum(min*magnifier);
}

void Tools::FileName(QString &name)
{
    ui->lineEdit->setText(name);
}

void Tools::setLevelValue(int value)
{
    // set dicimal digits, 0.1
    ui->levelValue->setText(QString::number(float(value)/magnifier));
}

void Tools::getLevelValue(int value)
{
    emit sentLevelValue(value);
}

Ui::Tools *Tools::getUi()
{
    return ui;
}
