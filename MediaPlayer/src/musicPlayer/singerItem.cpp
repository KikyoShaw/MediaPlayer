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

	//��ʼ���߳�
	m_thread = new QThread();
	m_threadRequest = new ThreadRequest();
	m_threadRequest->setRequestUrl(url);
	m_threadRequest->moveToThread(m_thread);
	m_thread->start();
	connect(m_thread, &QThread::started, m_threadRequest, &ThreadRequest::sltRequestNetWork);
	connect(m_threadRequest, &ThreadRequest::sigNetWorkFinish, this, &SingerItem::sltNetWorkSingerImg, Qt::AutoConnection);
}

void SingerItem::sltNetWorkSingerImg(QNetworkReply *reply)
{
	//��ȡ��Ӧ����Ϣ��״̬��Ϊ200��ʾ����
	QVariant status_code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
	//�޴��󷵻�
	if (reply->error() == QNetworkReply::NoError) {
		QByteArray bytes = reply->readAll();  //��ȡ�ֽ�
		QPixmap pixmap;
		pixmap.loadFromData(bytes);
		pixmap = musicManager.getRoundRectPixmap(pixmap, QSize(120, 120), 6);
		if (!pixmap.isNull()) {
			ui.label_image->setPixmap(pixmap);
		}
	}

	if (m_thread) {
		m_thread->quit();
		m_thread->disconnect();
		m_thread->deleteLater();
	}

	if (m_threadRequest) {
		m_threadRequest->deleteLater();
	}
}
