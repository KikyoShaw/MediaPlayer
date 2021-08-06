#pragma once

#include <QWidget>
#include <QThread>
#include "ui_singerItem.h"

class QNetworkAccessManager;
class QNetworkReply;

class SingerItem :public QWidget
{
	Q_OBJECT
public:
	SingerItem(QWidget *parent = Q_NULLPTR);
	~SingerItem();

	void setSingerItem(int classId, const QString & className, const QString & url);

private slots:
	void sltNetWorkSingerImg(QNetworkReply *reply);
	void sltRequsetImage();

private:
	Ui::singerItem ui;
	int m_classId;
	QString m_url;
	//Õº∆¨«Î«Û
	QNetworkAccessManager *m_netWorkSingerImg = nullptr;
	QThread m_threadFloat;
};