#include "singerItem.h"
#include <QNetworkReply>
#include <QJsonParseError>
#include <QJsonObject>
#include <QJsonArray>
#include "musicManager.h"
#include <QThread>
#include "ThreadRequest.h"

SingerItem::SingerItem(QWidget *parent)
	:QWidget(parent), m_classId(0), m_url(QString())
{
	ui.setupUi(this);
}

SingerItem::~SingerItem()
{
}

void SingerItem::setSingerItem(int classId, const QString & className, const QString & url)
{
	m_classId = classId;
	ui.label_name->setText(className);
	m_url = url;

	//初始化线程
	m_thread = new QThread();
	if (m_thread) {
		m_threadRequest = new ThreadRequest();
		if (m_threadRequest) {
			m_threadRequest->setRequestUrl(url);
			m_threadRequest->moveToThread(m_thread);
			m_thread->start();
			connect(m_thread, &QThread::started, m_threadRequest, &ThreadRequest::sltRequestNetWork);
			connect(m_threadRequest, &ThreadRequest::sigNetWorkFinish, this, &SingerItem::sltNetWorkSingerImg, Qt::AutoConnection);
			connect(m_thread, &QThread::finished, this, &SingerItem::sltThreadFinsh);
		}
	}
}

void SingerItem::sltThreadFinsh()
{
	if (m_threadRequest) {
		m_threadRequest->deleteLater();
	}
	if (m_thread) {
		m_thread->quit();
		m_thread->disconnect();
		m_thread->deleteLater();
	}
}

void SingerItem::sltNetWorkSingerImg(QNetworkReply *reply)
{
	//获取响应的信息，状态码为200表示正常
	QVariant status_code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
	//无错误返回
	if (reply->error() == QNetworkReply::NoError) {
		QByteArray bytes = reply->readAll();  //获取字节
		QPixmap pixmap;
		pixmap.loadFromData(bytes);
		pixmap = musicManager.getRoundRectPixmap(pixmap, QSize(120, 120), 6);
		if (!pixmap.isNull()) {
			ui.label_image->setPixmap(pixmap);
		}
	}
}
