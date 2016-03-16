#include <QtWidgets>
#include "emviewer.h"
#include "ui_emviewer.h"

EMviewer::EMviewer(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::EMviewer)
{
    ui->setupUi(this);
    QObject::connect(ui->ExitAct, SIGNAL(triggered(bool)), this, SLOT(close()));
    QObject::connect(ui->openAct, SIGNAL(triggered(bool)), ui->globject, SLOT(open()));
}

EMviewer::~EMviewer()
{
    delete ui;
}

