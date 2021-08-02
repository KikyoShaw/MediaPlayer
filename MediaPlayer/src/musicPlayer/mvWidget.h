#pragma once

#include <QWidget>
#include <QNetworkReply>
#include <QObjectCleanupHandler>
#include "ui_mvWidget.h"

class QNetworkAccessManager;
class FlowLayout;

class MVWidget :public QWidget
{
	Q_OBJECT
public:
	MVWidget(QWidget *parent = Q_NULLPTR);
	~MVWidget();

private:
	void searchMV(const QString& word, int page, int pageSize);
	void parseJson(const QString& json);
	void setMvModelItem();

private slots:
	void sltSearchMV();
	void sltSearchMvResult(QNetworkReply *reply);
	void sltSearchMvResultByScrollBar(int value);

private:
	Ui::mv ui;
	//搜索关键词
	QString m_mvWord;
	//搜索下标
	int m_pIndex;
	//搜索网络组件
	QNetworkAccessManager *m_searchNetWorkManager = nullptr;
	//避免数据多次请求
	bool m_isWorking = false;
	//布局器
	FlowLayout *m_flowLayout = nullptr;
	//窗口管理
	QObjectCleanupHandler m_mvWidgetsHandler;
};
