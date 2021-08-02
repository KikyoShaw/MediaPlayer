#include "mvItemWidget.h"
#include <QMovie>
#include "gifProxyStyle.h"
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <QFileInfo>
#include <QDir>

MvItem::MvItem(QWidget *parent /* = Q_NULLPTR */)
	:QWidget(parent)
{
	ui.setupUi(this);

	m_netWorkMvImg = new QNetworkAccessManager(this);
	connect(m_netWorkMvImg, &QNetworkAccessManager::finished, this, &MvItem::sltNetWorkMvImg, Qt::DirectConnection);
}

MvItem::~MvItem()
{
}

void MvItem::setMvInfo(MvInfo info)
{
	//缓存数据
	m_mvInfo = info;
	//mv名
	ui.label_name->setText(info.fileName);
	//上传时间
	ui.label_time->setText(info.publishDate);
	//动态背景
	if (!info.ThumbGif.isEmpty()) {
		QNetworkRequest request;
		//设置请求数据
		request.setUrl(QUrl(info.ThumbGif));
		request.setHeader(QNetworkRequest::UserAgentHeader, "RT-Thread ART");
		m_netWorkMvImg->get(request);
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

void MvItem::sltNetWorkMvImg(QNetworkReply *reply)
{
	//获取响应的信息，状态码为200表示正常
	QVariant status_code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
	//无错误返回
	if (reply->error() == QNetworkReply::NoError) {
		QByteArray bytes = reply->readAll();  //获取字节
		//拼接
		auto destFileName = "out/" + m_mvInfo.mvHash + ".gif";
		QFileInfo fileinfo(destFileName);
		if (!fileinfo.exists()) {
			QDir().mkpath(fileinfo.absolutePath());
		}
		QFile file(destFileName);
		if (file.open(QFile::WriteOnly)) {
			file.write(bytes);
		}
		file.close();
		setGifModel(destFileName);
	}
}
