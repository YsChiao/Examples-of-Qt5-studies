#include <QtWidgets>
#include "emviewer.h"
#include "ui_emviewer.h"

EMviewer::EMviewer(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::EMviewer)
{
    ui->setupUi(this);

    // create object for the label within statusbar
    statusLabel = new QLabel(this);
    ui->statusBar->addWidget(statusLabel);

    // connection
    QObject::connect(ui->ExitAct, SIGNAL(triggered(bool)), this, SLOT(close()));
    QObject::connect(ui->OpenAct, SIGNAL(triggered(bool)), ui->globject, SLOT(open()));
    QObject::connect(ui->globject, SIGNAL(sendMessage(QString&)), this, SLOT(showMessage(QString&)));
}

EMviewer::~EMviewer()
{
    delete ui;
}


void EMviewer::showMessage(QString &Message)
{
    statusLabel->setText(Message);
}






