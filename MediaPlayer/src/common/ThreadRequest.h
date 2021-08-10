#pragma once

#include <QObject>

class QNetworkReply;
class QNetworkAccessManager;

class ThreadRequest : public QObject
{
	Q_OBJECT
public:
	explicit ThreadRequest(QObject* parent = Q_NULLPTR);
	~ThreadRequest();

	void setRequestUrl(const QString& url) { m_url = url; };

public slots :
	void sltRequestNetWork();

signals:
	void sigNetWorkFinish(QNetworkReply *reply);

private:
	//ͼƬ����
	QNetworkAccessManager *m_netWork = nullptr;
	//����
	QString m_url;
};