#include "singerItem.h"
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <QJsonParseError>
#include <QJsonObject>
#include <QJsonArray>
#include "musicManager.h"

SingerItem::SingerItem(QWidget *parent)
	:QWidget(parent), m_classId(0), m_url(QString())
{
	ui.setupUi(this);

	m_netWorkSingerImg = new QNetworkAccessManager(this);
	connect(m_netWorkSingerImg, &QNetworkAccessManager::finished, this, &SingerItem::sltNetWorkSingerImg, Qt::DirectConnection);

	//connect(&m_threadFloat, &QThread::start, this, &SingerItem::sltRequsetImage);
	//connect(&m_threadFloat, &QThread::start, this, &SingerItem::sltRequsetImage);
}

SingerItem::~SingerItem()
{
}

void SingerItem::setSingerItem(int classId, const QString & className, const QString & url)
{
	m_classId = classId;
	ui.label_name->setText(className);
	m_url = url;
	//ͼƬ����
	QNetworkRequest request;
	//������������
	request.setUrl(url);
	request.setHeader(QNetworkRequest::UserAgentHeader, "RT-Thread ART");
	m_netWorkSingerImg->get(request);
	//m_threadFloat.start();
}

void SingerItem::sltRequsetImage()
{
	//ͼƬ����
	QNetworkRequest request;
	//������������
	request.setUrl(m_url);
	request.setHeader(QNetworkRequest::UserAgentHeader, "RT-Thread ART");
	m_netWorkSingerImg->get(request);
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
	/*	m_threadFloat.exit();
		m_threadFloat.disconnect();*/
	}
}
