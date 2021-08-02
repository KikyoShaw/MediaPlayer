#include "mvItemWidget.h"
#include <QMovie>
#include "gifProxyStyle.h"
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <QJsonParseError>
#include <QJsonObject>
#include <QJsonArray>
#include <QFileInfo>
#include <QDir>
#include "musicManager.h"
#include <QMouseEvent>
#include "videoWidget.h"
#include "frameless.h"

MvItem::MvItem(QWidget *parent /* = Q_NULLPTR */)
	:QWidget(parent), m_isGifImg(false), m_gifPath(QString())
{
	ui.setupUi(this);

	ui.label_img->installEventFilter(this);
	ui.widget_controls->setAttribute(Qt::WA_TransparentForMouseEvents, true);

	m_netWorkMvImg = new QNetworkAccessManager(this);
	connect(m_netWorkMvImg, &QNetworkAccessManager::finished, this, &MvItem::sltNetWorkMvImg, Qt::DirectConnection);

	m_netWorkMv = new QNetworkAccessManager(this);
	connect(m_netWorkMv, &QNetworkAccessManager::finished, this, &MvItem::sltNetWorkMvInfo, Qt::DirectConnection);
}

MvItem::~MvItem()
{
}

void MvItem::setMvInfo(MvInfo info)
{
	//缓存数据
	m_mvInfo = info;
	//mv名
	//处理文字过长
	QFontMetrics fm(ui.label_name->font());
	auto _width = fm.width(info.fileName);
	QString strElidedText = info.fileName;
	if (_width > 470) {
		strElidedText = fm.elidedText(info.fileName, Qt::ElideRight, 470, Qt::TextShowMnemonic);
		ui.label_name->setToolTip(info.fileName);
	}
	ui.label_name->setText(strElidedText);
	//上传时间
	ui.label_time->setText(info.publishDate);
	//观看数量
	auto watch = musicManager.createNumber(info.historyHeat);
	ui.label_watch->setText(watch);
	//动态背景
	if (!info.ThumbGif.isEmpty()) {
		//组装路径,如果存在则无需下载
		QString hashName = QCryptographicHash::hash(info.ThumbGif.toStdString().c_str(), QCryptographicHash::Md5).toHex().toUpper();
		m_gifPath = "out/" + hashName + ".gif";
		QFileInfo fileinfo(m_gifPath);
		if (fileinfo.exists()) {
			setGifModel(m_gifPath);
			return;
		}
		m_isGifImg = true;
		QNetworkRequest request;
		//设置请求数据
		request.setUrl(QUrl(info.ThumbGif));
		request.setHeader(QNetworkRequest::UserAgentHeader, "RT-Thread ART");
		m_netWorkMvImg->get(request);
	}
	else { //动态背景不存在则使用静态背景
		if (!info.pic.isEmpty()) {
			m_isGifImg = false;
			QNetworkRequest request;
			//组装url
			auto pic = info.pic.left(8);
			QString imgUrl = QString("http://imge.kugou.com/mvhdpic/400/%1/%2").arg(pic).arg(info.pic);
			//设置请求数据
			request.setUrl(QUrl(imgUrl));
			request.setHeader(QNetworkRequest::UserAgentHeader, "RT-Thread ART");
			m_netWorkMvImg->get(request);
		}
	}
}

void MvItem::setGifModel(const QString & filePath)
{
	//设置gif所在的label类属性
	auto size = ui.label_img->size();
	ui.label_img->setStyle(new GifProxyStyle(size, 6));
	//设置动态背景
	m_bgMovie = new QMovie(this);
	if (m_bgMovie) {
		m_bgMovie->setFileName(filePath);
		ui.label_img->setMovie(m_bgMovie);
		m_bgMovie->start();
	}
}

void MvItem::requsetMvInfo()
{
	if (m_isWorking) return;
	m_isWorking = true;
	QString url = QString("http://m.kugou.com/app/i/mv.php"
		"?cmd=100&hash=%1&ismp3=1&ext=mp4").arg(m_mvInfo.mvHash);
	QNetworkRequest request;
	//设置请求数据
	request.setUrl(QUrl(url));
	request.setHeader(QNetworkRequest::UserAgentHeader, "RT-Thread ART");
	m_netWorkMv->get(request);
}

void MvItem::parseJson(const QString & json)
{
	QByteArray byte_array;
	QJsonParseError json_error;
	QJsonDocument parse_doucment = QJsonDocument::fromJson(byte_array.append(json), &json_error);
	if (parse_doucment.isNull()) {
		return;
	}
	if (json_error.error == QJsonParseError::NoError) {
		if (parse_doucment.isObject()) {
			QJsonObject rootObj = parse_doucment.object();
			MvInfoPlayer info;
			info.mvName = musicManager.getJsonData(rootObj, "songname");
			if (rootObj.contains("mvdata")) {
				QJsonValue valuedata = rootObj.value("mvdata");
				if (valuedata.isObject()) {
					QJsonObject valuedataObject = valuedata.toObject();
					if (valuedataObject.contains("sq")) {
						QJsonValue valueSq = valuedataObject.value("sq");
						if (valueSq.isObject()) {
							QJsonObject object = valueSq.toObject();
							info.url = musicManager.getJsonData(object, "downurl");
							info.hash = musicManager.getJsonData(rootObj, "hash");
							m_videoWidget = QSharedPointer<VideoWidget>(new VideoWidget());
							if (m_videoWidget) {
								//拉伸器
								FrameLess *frameLess = new FrameLess(m_videoWidget.data());
								m_videoWidget->setMvPlay(info);
								m_videoWidget->show();
							}
						}
					}
				}
			}
		}
	}
}

void MvItem::sltNetWorkMvInfo(QNetworkReply * reply)
{
	//还原请求
	m_isWorking = false;
	//获取响应的信息，状态码为200表示正常
	QVariant status_code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
	//无错误返回
	if (reply->error() == QNetworkReply::NoError) {
		QByteArray bytes = reply->readAll();  //获取字节
		QString result(bytes);                //转化为字符串
		parseJson(result);
	}
}

bool MvItem::eventFilter(QObject * obj, QEvent * event)
{
	if (ui.label_img == obj && QEvent::MouseButtonDblClick == event->type()) {
		//QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
		//if (mouseEvent->button() == Qt::LeftButton) {
			requsetMvInfo();
			return true;
		/*}*/
	}
	return QWidget::eventFilter(obj, event);
}

void MvItem::sltNetWorkMvImg(QNetworkReply *reply)
{
	//获取响应的信息，状态码为200表示正常
	QVariant status_code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
	//无错误返回
	if (reply->error() == QNetworkReply::NoError) {
		QByteArray bytes = reply->readAll();  //获取字节
		//动态图片处理
		if (m_isGifImg) {
			QFileInfo fileinfo(m_gifPath);
			if (!fileinfo.exists()) {
				QDir().mkpath(fileinfo.absolutePath());
			}
			QFile file(m_gifPath);
			if (!file.open(QFile::WriteOnly)) {
				return;
			}
			file.write(bytes);
			file.close();
			setGifModel(m_gifPath);
		}
		else { //静态图片处理
			QPixmap pixmap;
			pixmap.loadFromData(bytes);
			pixmap = musicManager.getRoundRectPixmap(pixmap, QSize(250, 142), 6);
			if (!pixmap.isNull()) {
				ui.label_img->setPixmap(pixmap);
			}
		}
	}
}
