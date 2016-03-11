#include "webcontent.h"
#include "ui_webcontent.h"

#include <QWebElementCollection>
#include <QWebFrame>
#include <QDateTime>
#include <QTimer>

webContent::webContent(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::webContent)
{
  ui->setupUi(this);
  QDateTime Data;
  ui->textLabel->setText(Data.currentDateTime().toString());

  QString EUR_URL = "http://www.kuaiyilicai.com/uprate/eur.html";
  QString USD_URL = "http://www.kuaiyilicai.com/uprate/usd.html";
  QString TWD_URL = "http://www.kuaiyilicai.com/uprate/twd.html";

  //qDebug() << "Main thread ID : " << QThread::currentThreadId() << " runing.";

  //================================EUR/CNY====================================================
  EurContent = new LoadContent(EUR_URL);
  EurThread = new QThread;
  EurContent->moveToThread(EurThread);

  // default display content
  //QTimer::singleShot(1, EurContent, SLOT(loadWebContent()));
  //QTimer::singleShot(1, this, SLOT(progressBarLoading()));
  QObject::connect(EurThread, SIGNAL(started()), EurContent, SLOT(loadWebContent()));
  QObject::connect(EurThread, SIGNAL(started()), this, SLOT(progressBarLoading()));

  QObject::connect(ui->euroButton, SIGNAL(clicked(bool)), EurContent, SLOT(loadWebContent()));
  QObject::connect(ui->euroButton, SIGNAL(clicked(bool)), this, SLOT(progressBarLoading()));
  QObject::connect(EurContent, SIGNAL(QNetworkReplyFinished(QNetworkReply*)),
                   this, SLOT(QNetworkReplyFinishedLoad(QNetworkReply*)));
  EurThread->start();

  //=================================UDS/CNY======================================================
  UsdContent = new LoadContent(USD_URL);
  UsdThread = new QThread;
  UsdContent->moveToThread(UsdThread);

  QObject::connect(ui->usdButton, SIGNAL(clicked(bool)), UsdContent, SLOT(loadWebContent()));
  QObject::connect(ui->usdButton, SIGNAL(clicked(bool)), this, SLOT(progressBarLoading()));
  QObject::connect(UsdContent, SIGNAL(QNetworkReplyFinished(QNetworkReply*)),
                   this, SLOT(QNetworkReplyFinishedLoad(QNetworkReply*)));
  UsdThread->start();

  //=================================CNY/TWD======================================================
  TwdContent = new LoadContent(TWD_URL);
  TwdThread = new QThread;
  TwdContent->moveToThread(TwdThread);

  QObject::connect(ui->twdButton, SIGNAL(clicked(bool)), TwdContent, SLOT(loadWebContent()));
  QObject::connect(ui->twdButton, SIGNAL(clicked(bool)), this, SLOT(progressBarLoading()));
  QObject::connect(TwdContent, SIGNAL(QNetworkReplyFinished(QNetworkReply*)),
                   this, SLOT(QNetworkReplyFinishedLoad(QNetworkReply*)));
  TwdThread->start();

  //=========================================================================================
  QObject::connect(this, SIGNAL(replyContentFinished(QWebElementCollection)),
                   this, SLOT(viewLoaded(QWebElementCollection)));
  QObject::connect(this, SIGNAL(viewLoadedFinished()), this, SLOT(progressBarStop()));
}

webContent::~webContent()
{
  delete ui;
  delete EurThread;
  delete UsdThread;
  delete TwdThread;
  delete EurContent;
  delete UsdContent;
  delete TwdContent;
}

void webContent::QNetworkReplyFinishedLoad(QNetworkReply * reply)
{
  const QString html(reply->readAll());
  QWebPage page;
  page.mainFrame()->setHtml(html);

  // get HTML element information
  QWebElementCollection collection = page.mainFrame()->findAllElements("div.table-responsive");
  emit replyContentFinished(collection);
}

void webContent::viewLoaded(QWebElementCollection collection)
{
  ui->textContent->clear();
  foreach (QWebElement elements, collection) {
      ui->textContent->setText(elements.toInnerXml());
    }
  emit viewLoadedFinished();
}

void webContent::progressBarLoading()
{
  // as the progress bar would goes forever.
  ui->progressBar->setRange(0,0);
  ui->progressBar->setValue(0);
}

void webContent::progressBarStop()
{
  // as the progress bar goes full.
  ui->progressBar->setRange(0,10);
  ui->progressBar->setValue(10);
}



