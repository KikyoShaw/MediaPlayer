#pragma once

#include <QObject>

class QNetworkReply;

class ThreadManager : public QObject
{
	Q_OBJECT
public:
	explicit ThreadManager(QObject* parent = Q_NULLPTR);
	~ThreadManager();

	void requestNetWork(const QString& url);

signals:
	void sigSendReplyData(QNetworkReply *reply);

};