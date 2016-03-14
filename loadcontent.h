#ifndef LOADCONTENT_H
#define LOADCONTENT_H

#include <QObject>
#include <QThread>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QWebPage>
#include <QWebFrame>
#include <QWebElementCollection>
#include <QDebug>
#include <QApplication>


class LoadContent : public QObject
{
  Q_OBJECT

public:
  explicit LoadContent(const QString& URL, QObject* parent = 0);
  virtual ~LoadContent();

signals:
  void QNetworkReplyFinished(QNetworkReply*);

public slots:
  void loadWebContent();
  void replyFinished(QNetworkReply*);

private:
  QNetworkAccessManager *manager;
  QNetworkRequest *request;
  QUrl url;
};

#endif // LOADCONTENT_H
