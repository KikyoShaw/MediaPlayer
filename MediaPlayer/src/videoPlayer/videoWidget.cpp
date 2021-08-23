#include "videoWidget.h"
#include <QVideoWidget>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QMouseEvent>
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <QFileInfo>
#include <QDir>
#include "videoControls.h"
#include "videoControlTops.h"

VideoWidget::VideoWidget(QWidget *parent) :
    QWidget(parent)
{
    ui.setupUi(this);

    //隐藏边框
	setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::WindowMinimizeButtonHint);

	ui.label_player->installEventFilter(this);

    //加载视频操作
	m_videoPlayer = new QMediaPlayer(this);
	m_videoPlayList = new QMediaPlaylist(this);
	m_videoPlayList->setPlaybackMode(QMediaPlaylist::Sequential);
	m_videoPlayer->setVolume(50);
	m_videoPlayer->setPlaylist(m_videoPlayList);
    m_videoPlayWidget = new QVideoWidget();
	m_videoPlayWidget->setStyleSheet("border:none;background-color:transparent;");
	layout_video = new QVBoxLayout();
	layout_video->setMargin(0);
	ui.label_player->setLayout(layout_video);
	m_videoPlayWidget->resize(ui.label_player->size()); 
	m_videoPlayer->setVideoOutput(m_videoPlayWidget);
	layout_video->addWidget(m_videoPlayWidget);
	//进度同步
	connect(m_videoPlayer, SIGNAL(durationChanged(qint64)), this, SLOT(sltDurationChanged(qint64)));
	connect(m_videoPlayer, SIGNAL(positionChanged(qint64)), this, SLOT(sltPositionChanged(qint64)));

	initVideoControls();

	m_netWorkMvDownLoad = new QNetworkAccessManager(this);
	connect(m_netWorkMvDownLoad, &QNetworkAccessManager::finished, this, &VideoWidget::sltNetWorkMvDownLoad, Qt::DirectConnection);
}

VideoWidget::~VideoWidget()
{
}

void VideoWidget::setVideoPlay(const QString & filePath)
{
	m_videoPlayList->addMedia(QUrl::fromLocalFile(filePath));
	m_videoPlayer->play();
	if (m_videoControls) {
		m_videoControls->setPlaying();
	}
}

void VideoWidget::setMvPlay(MvInfoPlayer info)
{
	if (m_isWorking) return;
	m_isWorking = true;
	if (m_videoPlayer->state() != QMediaPlayer::StoppedState) {
		m_videoPlayer->stop();
	}
	m_mvInfoPlayer = info;
	setWindowTitle(info.mvName);
	if (m_videoControlTops) {
		m_videoControlTops->setTitle(info.mvName);
	}
	//组装路径,如果存在则无需下载
	m_mvInfoPlayer.path = "out/" + info.hash + ".mp4";
	QFileInfo fileinfo(m_mvInfoPlayer.path);
	if (fileinfo.exists()) {
		setVideoPlay(m_mvInfoPlayer.path);
		m_isWorking = false;
		return;
	}
	QNetworkRequest request;
	//设置请求数据
	request.setUrl(QUrl(info.url));
	request.setHeader(QNetworkRequest::UserAgentHeader, "RT-Thread ART");
	m_netWorkMvDownLoad->get(request);
}

void VideoWidget::initVideoControls()
{
	m_videoControls = QSharedPointer<VideoControls>(new VideoControls(this));
	if (m_videoControls) {
		m_videoControls->setMvWidget();
		m_videoControls->installEventFilter(this);
		connect(m_videoControls.data(), &VideoControls::sigVideoPlayOrPause, this, &VideoWidget::sltVideoPlayOrPause);
		connect(m_videoControls.data(), &VideoControls::sigSetPosition, this, &VideoWidget::sltSetPosition);
		connect(m_videoControls.data(), &VideoControls::sigSoundVoiceValue, this, &VideoWidget::sltSoundVoiceValue);
	}

	m_videoControlTops = QSharedPointer<VideoControlTop>(new VideoControlTop(this));
	if (m_videoControlTops) {
		m_videoControlTops->installEventFilter(this);
		connect(m_videoControlTops.data(), &VideoControlTop::sigClose, this, [=]() {
			if (m_videoPlayer->state() != QMediaPlayer::StoppedState) {
				m_videoPlayer->stop();
			}
			close();
		});
	}
}

void VideoWidget::locateWidgets()
{
	if (m_videoControls) {
		int posX = ui.label_player->x() + 1;
		int posY = ui.label_player->height() - m_videoControls->height() - 1;
		m_videoControls->setFixedWidth(width() - posX - 2);
		m_videoControls->move(mapToGlobal(QPoint(posX, posY)));
		m_videoControls->locateWidgets();
	}

	if (m_videoControlTops) {
		int posX = ui.label_player->x() + 1;
		int posY = ui.label_player->y() + 1;
		m_videoControlTops->setFixedWidth(width() - posX - 2);
		m_videoControlTops->move(mapToGlobal(QPoint(posX, posY)));
	}
}

void VideoWidget::sltVideoPlayOrPause(bool state)
{
	if (state) {
		m_videoPlayer->play();
	}
	else {
		m_videoPlayer->pause();
	}
}

void VideoWidget::sltSoundVoiceValue(int value)
{
	m_videoPlayer->setVolume(value);
}

void VideoWidget::sltSetPosition(int value)
{
	m_videoPlayer->setPosition(value);
}

void VideoWidget::sltDurationChanged(qint64 duration)
{
	//设置进度条
	if (m_videoControls) {
		m_videoControls->setProgressDuration(duration);
	}
	//总时间
	auto hh = duration / 3600000;
	auto mm = (duration % 3600000) / 60000.0;
	auto ss = (duration % 60000) / 1000.0;
	QTime allTime(hh, mm, ss);
	m_videoTime = allTime.toString(tr("hh:mm:ss"));
}

void VideoWidget::sltPositionChanged(qint64 position)
{
	if (!m_videoControls) {
		return;
	}
	if (m_videoControls->isSliderDown()) {
		return;
	}
	m_videoControls->setSliderPosition(position);
	auto hh = position / 3600000;
	auto mm = (position % 3600000) / 60000.0;
	auto ss = (position % 60000) / 1000.0;
	QTime duration(hh, mm, ss);
	auto localTime = duration.toString(tr("hh:mm:ss"));
	auto text = QString("%1/%2").arg(localTime).arg(m_videoTime);
	m_videoControls->setProgressText(text);
}

void VideoWidget::sltNetWorkMvDownLoad(QNetworkReply * reply)
{
	//还原请求
	m_isWorking = false;
	//获取响应的信息，状态码为200表示正常
	QVariant status_code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
	//无错误返回
	if (reply->error() == QNetworkReply::NoError) {
		QByteArray bytes = reply->readAll();  //获取字节
		QFileInfo fileinfo(m_mvInfoPlayer.path);
		if (!fileinfo.exists()) {
			QDir().mkpath(fileinfo.absolutePath());
		}
		QFile file(m_mvInfoPlayer.path);
		if (!file.open(QFile::WriteOnly)) {
			return;
		}
		file.write(bytes);
		file.close();
		setVideoPlay(m_mvInfoPlayer.path);
	}
}

void VideoWidget::mouseMoveEvent(QMouseEvent * event)
{
	//判断左键是否被按下，只有左键按下了，其返回值就是1(true)
	if ((event->buttons() & Qt::LeftButton) && m_bMove)
	{
		move(event->globalPos() - m_point);
	}
	QWidget::mouseMoveEvent(event);
}

void VideoWidget::mousePressEvent(QMouseEvent * event)
{
	//鼠标事件中包含“按住的是左键”
	if (event->button() == Qt::LeftButton)
	{
		m_bMove = true;
		//获取移动的位移量
		m_point = event->globalPos() - frameGeometry().topLeft();
	}
}

void VideoWidget::mouseReleaseEvent(QMouseEvent * event)
{
	m_bMove = false;
}

void VideoWidget::showEvent(QShowEvent * event)
{
	QWidget::showEvent(event);
	locateWidgets();
}

void VideoWidget::closeEvent(QCloseEvent * event)
{
	if (m_videoControls) {
		m_videoControls->closeWidget();
	}
	if (m_videoControlTops) {
		m_videoControlTops->close();
	}
	QWidget::closeEvent(event);
}

void VideoWidget::moveEvent(QMoveEvent * event)
{
	QWidget::moveEvent(event);
	locateWidgets();
}

void VideoWidget::hideEvent(QHideEvent * event)
{
	if (m_videoControls) {
		m_videoControls->setVisible(false);
	}
	if (m_videoControlTops) {
		m_videoControlTops->setVisible(false);
	}
	locateWidgets();
	QWidget::hideEvent(event);
}

void VideoWidget::resizeEvent(QResizeEvent * event)
{
	QWidget::resizeEvent(event);
	locateWidgets();
}

bool VideoWidget::eventFilter(QObject * obj, QEvent * event)
{
	if (obj == Q_NULLPTR) {
		return false;
	}
	if (m_videoControls == Q_NULLPTR) {
		return false;
	}
	if (m_videoControlTops == Q_NULLPTR) {
		return false;
	}

	if (QEvent::Enter == event->type()) {
		if (obj == ui.label_player || obj == m_videoControls || obj == m_videoControlTops) {
			/*if (QMediaPlayer::StoppedState != m_videoPlayer->state()) {*/
				m_videoControls->setVisible(true);
				m_videoControlTops->setVisible(true);
			/*}*/
			return false;
		}
	}
	else if (QEvent::Leave == event->type()) {
		if (obj != ui.label_player || obj != m_videoControls || obj != m_videoControlTops) {
			auto volumIsVisible = m_videoControls->getVolumVisible();
			if (!volumIsVisible) {
				m_videoControls->setVisible(false);
				m_videoControlTops->setVisible(false);
			}
			return false;
		}
	}
	return QWidget::eventFilter(obj, event);
}
