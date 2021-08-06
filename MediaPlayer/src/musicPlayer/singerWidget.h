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
	//网络组件
	QNetworkAccessManager *m_netWorkManager = nullptr;
	//避免数据多次请求
	bool m_isWorking = false;
	//布局器
	FlowLayout *m_flowLayout = nullptr;
	//窗口管理
	QObjectCleanupHandler m_mvWidgetsHandler;
};

