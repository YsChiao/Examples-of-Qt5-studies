#ifndef WEBCONTENT_H
#define WEBCONTENT_H

#include "loadcontent.h"

#include <QWidget>
#include <QWebView>
//#include <QThread>

namespace Ui {
  class webContent;
}

class webContent : public QWidget
{
  Q_OBJECT

public:
  explicit webContent(QWidget *parent = 0);
  virtual ~webContent();

public slots:
  void QNetworkReplyFinishedLoad(QNetworkReply*);
  void viewLoaded(QWebElementCollection);
  void progressBarLoading();
  void progressBarStop();

signals:
  void viewLoadedFinished();
  void QNetworkReplyFinished();
  void replyContentFinished(QWebElementCollection);

private:
  Ui::webContent *ui;
  QThread* EurThread;
  QThread* UsdThread;
  QThread* TwdThread;
  LoadContent* EurContent;
  LoadContent* UsdContent;
  LoadContent* TwdContent;
};

#endif // WEBCONTENT_H
