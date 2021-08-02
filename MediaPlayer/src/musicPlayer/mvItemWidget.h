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
	//��̬����
	QMovie *m_bgMovie = nullptr;
	//mvͼƬ����
	QNetworkAccessManager *m_netWorkMvImg = nullptr;
	//���浱ǰmv����
	MvInfo m_mvInfo;
};
