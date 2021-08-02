#pragma once

#include <QWidget>
#include "mvInfoModel.h"
#include "ui_mvItemWidget.h"

class QMovie;
class QNetworkAccessManager;
class QNetworkReply;

class MvItem :public QWidget
{
	Q_OBJECT
public:
	MvItem(QWidget *parent = Q_NULLPTR);
	~MvItem();

	void setMvInfo(MvInfo info);

private:
	void setGifModel(const QString& filePath);

private slots:
	void sltNetWorkMvImg(QNetworkReply *reply);

private:
	Ui::mvItem ui;
	//动态背景
	QMovie *m_bgMovie = nullptr;
	//mv图片请求
	QNetworkAccessManager *m_netWorkMvImg = nullptr;
	//缓存当前mv数据
	MvInfo m_mvInfo;
};
