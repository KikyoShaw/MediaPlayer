#include "ThreadRequest.h"
#include <QNetworkReply>
#include <QNetworkAccessManager>

ThreadRequest::ThreadRequest(QObject* parent)
	:QObject(parent), m_url(QString())
{
	m_netWork = new QNetworkAccessManager(this);
	connect(m_netWork, &QNetworkAccessManager::finished, this, &ThreadRequest::sigNetWorkFinish, Qt::DirectConnection);
}

ThreadRequest::~ThreadRequest()
{
}

void ThreadRequest::sltRequestNetWork()
{
	QNetworkRequest request;
	request.setUrl(QUrl(m_url));
	//����ͷ�޷��õ�json��������Ϊ�˷�ֹ������ȡ
	//request.setRawHeader("Cookie", "kg_mid=2333");
	//request.setHeader(QNetworkRequest::CookieHeader, 2333);
	request.setHeader(QNetworkRequest::UserAgentHeader, "RT-Thread ART");
	m_netWork->get(request);
}
