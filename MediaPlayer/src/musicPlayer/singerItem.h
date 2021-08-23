#pragma once

#include <QWidget>
#include "ui_singerItem.h"

class QNetworkReply;
class QThread;
class ThreadRequest;

class SingerItem :public QWidget
{
	Q_OBJECT
public:
	SingerItem(QWidget *parent = Q_NULLPTR);
	~SingerItem();

	void setSingerItem(int classId, const QString & className, const QString & url);

private slots:
	void sltNetWorkSingerImg(QNetworkReply *reply);

	void sltThreadFinsh();

private:
	Ui::singerItem ui;
	int m_classId;
	QString m_url;
	//请求数据线程
	QThread* m_thread = nullptr;
	ThreadRequest *m_threadRequest = nullptr;
};