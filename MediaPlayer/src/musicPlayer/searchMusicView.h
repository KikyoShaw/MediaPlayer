#pragma once

#include <QWidget>
#include <QNetworkReply>
#include "ui_searchMusicView.h"

class QNetworkAccessManager;

class SearchMusic : public QWidget
{
	Q_OBJECT
public:
	SearchMusic(QWidget *parent = Q_NULLPTR);
	~SearchMusic();

	void searchMusic(const QString& word);

private slots:
	void sltSearchMusicResult(QNetworkReply *reply);
	void sltSearchMusicResultByScrollBar(int value);

private:
	void _searchMusic(const QString& word, int page, int pageSize);
	void parseJson(const QString& json);

private:
	Ui::searchMusic ui;
	QNetworkAccessManager *m_searchNetWorkManager = nullptr;
	//�������ݶ������
	bool m_isWorking = false;
	//���浱ǰ��������
	QString m_searchWord;
};