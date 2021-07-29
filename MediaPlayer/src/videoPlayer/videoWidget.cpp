#include "videoWidget.h"
#include <QVideoWidget>
#include <QMediaPlayer>
#include <QMouseEvent>

VideoWidget::VideoWidget(QWidget *parent) :
    QWidget(parent)
{
    ui.setupUi(this);

    //隐藏边框
    setWindowFlags(Qt::FramelessWindowHint);

    //加载视频操作
	m_videoPlayer = new QMediaPlayer(this);
    m_videoPlayWidget = new QVideoWidget(this);
	layout_video = new QVBoxLayout();
	layout_video->setContentsMargins(0, 0, 0, 0);
	layout_video->setSpacing(0);
	layout_video->setAlignment(Qt::AlignVCenter);
	ui.label_player->setLayout(layout_video);
	m_videoPlayWidget->resize(ui.label_player->size()); 
	//m_videoPlayWidget->setAspectRatioMode(Qt::IgnoreAspectRatio);
	m_videoPlayer->setVideoOutput(m_videoPlayWidget);
	layout_video->addWidget(m_videoPlayWidget);
}

VideoWidget::~VideoWidget()
{
}

void VideoWidget::setVideoPlay(const QString & filePath)
{
	m_videoPlayer->setMedia(QUrl::fromLocalFile(filePath));
	m_videoPlayer->setVolume(100);
	m_videoPlayer->play();
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
	if ((event->button() == Qt::LeftButton) && (event->pos().y() < 56) && (event->pos().y() > 0))
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
