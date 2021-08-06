#pragma once

#include <QWidget>
#include "ui_singerWidget.h"
#include <QNetworkReply>
#include <QObjectCleanupHandler>

class QNetworkAccessManager;
class FlowLayout;

class SingerWidget : public QWidget
{
	Q_OBJECT
public:
	SingerWidget(QWidget *parent = Q_NULLPTR);
	~SingerWidget();

	void requestSinger();

protected slots:
	void sltSingerResult(QNetworkReply *reply);

private:
	void setSingerItem(int classId, const QString& className, const QString& url);
	void parseJson(const QString& json);

private:
	Ui::singer ui;
	//�������
	QNetworkAccessManager *m_netWorkManager = nullptr;
	//�������ݶ������
	bool m_isWorking = false;
	//������
	FlowLayout *m_flowLayout = nullptr;
	//���ڹ���
	QObjectCleanupHandler m_mvWidgetsHandler;
};

