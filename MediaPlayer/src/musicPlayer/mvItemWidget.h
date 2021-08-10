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

private:
	virtual bool eventFilter(QObject *obj, QEvent *event);

private:
	Ui::mvItem ui;
	//��̬����
	QMovie *m_bgMovie = nullptr;
	//MV��ϸ��ȡ
	QNetworkAccessManager *m_netWorkMv = nullptr;
	//�Ƿ���gif��Դ
	bool m_isGifImg;
	//���浱ǰmv����
	MvInfo m_mvInfo;
	//�������ݶ������
	bool m_isWorking = false;
	//gif·��
	QString m_gifPath;
	//MV������
	QSharedPointer<VideoWidget> m_videoWidget = nullptr;
	//��������
	QSharedPointer<QThread> m_thread = nullptr;
	QSharedPointer<ThreadRequest> m_threadRequest = nullptr;
};
