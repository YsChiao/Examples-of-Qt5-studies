#include "loadcontent.h"

LoadContent::LoadContent(const QString& URL, QObject* parent) :
  QObject(parent)
{
  url.setUrl(URL);
}

LoadContent::~LoadContent()
{
  delete manager;
  delete request;
}

void LoadContent::loadWebContent()
{
  //qDebug() << "Thread : " << QThread::currentThreadId();
  manager = new QNetworkAccessManager();
  request = new QNetworkRequest();
  QObject::connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));
  manager->get(QNetworkRequest(url));
}

void LoadContent::replyFinished(QNetworkReply * reply)
{
  emit QNetworkReplyFinished(reply);
}






