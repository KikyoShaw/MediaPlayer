#include "musicImage.h"
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <QMouseEvent>
#include "musicManager.h"

MusicImage::MusicImage(QWidget *parent /* = Q_NULLPTR */)
	:QLabel(parent)
{
	this->installEventFilter(this);
	m_netWorkMusicImg = new QNetworkAccessManager(this);
	connect(m_netWorkMusicImg, &QNetworkAccessManager::finished, this, &MusicImage::sltNetWorkMusicImg, Qt::DirectConnection);
}

MusicImage::~MusicImage()
{
}

void MusicImage::requestImage(const QString & url)
{
	QNetworkRequest request;
	//设置请求数据
	request.setUrl(QUrl(url));
	request.setHeader(QNetworkRequest::UserAgentHeader, "RT-Thread ART");
	m_netWorkMusicImg->get(request);
}

bool MusicImage::eventFilter(QObject * obj, QEvent * event)
{
	if (this == obj && QEvent::MouseButtonRelease == event->type()) {
		QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
		if (mouseEvent->button() == Qt::LeftButton) {
			emit sigMouseClicked();
			return true;
		}
	}
	return QWidget::eventFilter(obj, event);
}

void MusicImage::sltNetWorkMusicImg(QNetworkReply *reply)
{
	//获取响应的信息，状态码为200表示正常
	QVariant status_code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
	//无错误返回
	if (reply->error() == QNetworkReply::NoError) {
		QByteArray bytes = reply->readAll();  //获取字节
		QPixmap pixmap;
		pixmap.loadFromData(bytes);
		//缓存图片
		musicManager.saveMusicImage(pixmap);
		pixmap = musicManager.getRoundRectPixmap(pixmap, QSize(50,50), 4);
		if (!pixmap.isNull()) {
			setPixmap(pixmap);
		}
	}
}
