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
	//��������
	m_mvInfo = info;
	//mv��
	ui.label_name->setText(info.fileName);
	//�ϴ�ʱ��
	ui.label_time->setText(info.publishDate);
	//��̬����
	if (!info.ThumbGif.isEmpty()) {
		QNetworkRequest request;
		//������������
		request.setUrl(QUrl(info.ThumbGif));
		request.setHeader(QNetworkRequest::UserAgentHeader, "RT-Thread ART");
		m_netWorkMvImg->get(request);
	}
}

void MvItem::setGifModel(const QString & filePath)
{
	//����gif���ڵ�label������
	auto size = ui.label_img->size();
	ui.label_img->setStyle(new GifProxyStyle(size, 6));
	//���ö�̬����
	m_bgMovie = new QMovie(this);
	if (m_bgMovie) {
		m_bgMovie->setFileName(filePath);
		ui.label_img->setMovie(m_bgMovie);
		m_bgMovie->start();
	}
}

void MvItem::sltNetWorkMvImg(QNetworkReply *reply)
{
	//��ȡ��Ӧ����Ϣ��״̬��Ϊ200��ʾ����
	QVariant status_code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
	//�޴��󷵻�
	if (reply->error() == QNetworkReply::NoError) {
		QByteArray bytes = reply->readAll();  //��ȡ�ֽ�
		//ƴ��
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
