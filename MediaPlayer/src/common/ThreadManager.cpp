#include "ThreadManager.h"
#include <QThread>
#include "ThreadRequest.h"
#include <QNetworkReply>

ThreadManager::ThreadManager(QObject* parent)
	:QObject(parent)
{

}

ThreadManager::~ThreadManager()
{
}

void ThreadManager::requestNetWork(const QString & url)
{
	QThread* m_thread = new QThread();
	ThreadRequest *m_threadRequest = new ThreadRequest();
	m_threadRequest->setRequestUrl(url);
	m_threadRequest->moveToThread(m_thread);
	m_thread->start();

	connect(m_thread, &QThread::started, m_threadRequest, &ThreadRequest::sltRequestNetWork);
	connect(m_threadRequest, &ThreadRequest::sigNetWorkFinish, m_thread, &QThread::quit);
	connect(m_thread, &QThread::finished, m_thread, &QThread::deleteLater);
	connect(m_threadRequest, &ThreadRequest::sigNetWorkFinish, this, &ThreadManager::sigSendReplyData);
	connect(m_threadRequest, &ThreadRequest::sigNetWorkFinish, m_threadRequest, &ThreadRequest::deleteLater);
}
