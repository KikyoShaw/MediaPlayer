#pragma once

#include <QWidget>
#include "mvInfoModel.h"
#include "ui_mvItemWidget.h"

class QMovie;
class QNetworkAccessManager;
class QNetworkReply;
class VideoWidget;

class QThread;
class ThreadRequest;

class MvItem :public QWidget
{
	Q_OBJECT
public:
	MvItem(QWidget *parent = Q_NULLPTR);
	~MvItem();

	void setMvInfo(MvInfo info);

private:
	void setGifModel(const QString& filePath);
	void requsetMvInfo();
	void parseJson(const QString& json);

private slots:
	void sltNetWorkMvImg(QNetworkReply *reply);
	void sltNetWorkMvInfo(QNetworkReply *reply);
	void sltThreadFinsh();

private:
	virtual bool eventFilter(QObject *obj, QEvent *event);

private:
	Ui::mvItem ui;
	//动态背景
	QMovie *m_bgMovie = nullptr;
	//MV详细获取
	QNetworkAccessManager *m_netWorkMv = nullptr;
	//是否是gif资源
	bool m_isGifImg;
	//缓存当前mv数据
	MvInfo m_mvInfo;
	//避免数据多次请求
	bool m_isWorking = false;
	//gif路径
	QString m_gifPath;
	//MV播放器
	QSharedPointer<VideoWidget> m_videoWidget = nullptr;
	//网络请求
	QThread *m_thread = nullptr;
	ThreadRequest *m_threadRequest = nullptr;
};
