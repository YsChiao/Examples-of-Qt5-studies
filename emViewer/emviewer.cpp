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



    // create tool window
    toolsWidget = new glOperator(this);



    // connection
    QObject::connect(ui->ExitAct, SIGNAL(triggered(bool)), this, SLOT(close()));
    QObject::connect(ui->OpenAct, SIGNAL(triggered(bool)), ui->globject, SLOT(open()));
    QObject::connect(ui->globject, SIGNAL(sendMessage(QString&)), this, SLOT(showMessage(QString&)));

    //connect glObject and glOperator
    QObject::connect(ui->globject, SIGNAL(sendMaxMin(float,float)), this->toolsWidget, SLOT(MaxMin(float,float)));
    QObject::connect(ui->globject, SIGNAL(sendFileName(QString&)),  this->toolsWidget, SLOT(FileName(QString&)));
    QObject::connect(ui->globject, SIGNAL(renderDone()), this, SLOT(toolsWindowShow()));
    QObject::connect(this->toolsWidget, SIGNAL(sentLevelValue(int)), ui->globject, SLOT(getLevel(int)));
}

EMviewer::~EMviewer()
{
    delete ui;
    delete statusLabel;
    delete toolsWidget;
}


void EMviewer::showMessage(QString &Message)
{
    statusLabel->setText(Message);
}

void EMviewer::toolsWindowShow()
{

    // get mainwindow position
    QRect window = this->geometry();
    //std::cout << window.x() << " " << window.y() << std::endl;
    // set the position of tools widget
    int height = toolsWidget->height();
    int width = toolsWidget->width();
    QRect toolsWindow(window.x()+window.width()+10, window.y()+height, width, height);
    toolsWidget->setGeometry(toolsWindow);
    toolsWidget->show();
}






