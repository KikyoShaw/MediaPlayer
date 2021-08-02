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
	//�����ؼ���
	QString m_mvWord;
	//�����±�
	int m_pIndex;
	//�����������
	QNetworkAccessManager *m_searchNetWorkManager = nullptr;
	//�������ݶ������
	bool m_isWorking = false;
	//������
	FlowLayout *m_flowLayout = nullptr;
	//���ڹ���
	QObjectCleanupHandler m_mvWidgetsHandler;
};
