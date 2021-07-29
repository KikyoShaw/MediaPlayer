#include "videoPlayer.h"
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QVideoWidget>
#include <QMouseEvent>
#include <QTime>
#include <QFileDialog>
#include <QScrollBar>

//播放状态，true为播放，false为暂停
bool play_state;

//与Slider有关的播放控制变量
QTimer * timer;
int maxValue = 1000;//设置进度条的最大值

//音量控制slider
CustomSlider * slider_volume;

VideoPlayer::VideoPlayer(QWidget *parent) :
	QWidget(parent)
{
    ui.setupUi(this);

	setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::WindowMinimizeButtonHint);
	//setAttribute(Qt::WA_TranslucentBackground);

	//初始化播放器模块
	initVideoPlayModel();

	//优化滑动条
	QFile QSS1(":/qss/image/qss/whiteScrollbar.qss");
	if (QSS1.open(QIODevice::ReadOnly)) {
		QString strStyle = QSS1.readAll();
		ui.listWidget_localList->verticalScrollBar()->setStyleSheet(strStyle);
	}

	//列表样式
	QFile QSS2(":/qss/image/qss/listWidget.qss");
	if (QSS2.open(QIODevice::ReadOnly)) {
		QString strStyle = QSS2.readAll();
		ui.listWidget_localList->setStyleSheet(strStyle);
	}

	connect(ui.pushButton_min, &QPushButton::clicked, this, &VideoPlayer::showMinimized);
	connect(ui.pushButton_close, &QPushButton::clicked, this, &VideoPlayer::close);
	connect(ui.pushButton_max, &QPushButton::clicked, this, &VideoPlayer::sltMaxOrNormal);

	//打开本地文件
	connect(ui.pushButton_open, &QPushButton::clicked, this, &VideoPlayer::sltOpenLocalVideoList);
	//列表点击事件
	connect(ui.listWidget_localList, &QListWidget::itemDoubleClicked, this, &VideoPlayer::sltListWidgetDoubleClicked);

  //  //设置窗口图标
  //  //QIcon icon;
  //  //icon.addFile(QStringLiteral(":/new/image/MediaPlayer.ico"), QSize(), QIcon::Normal, QIcon::Off);
  //  //this->setWindowIcon(icon);
  //  //隐藏边框
  ////  this->setWindowFlags(Qt::FramelessWindowHint);
  //  //ui.pushButton_play_and_pause->setEnabled(false);
  //  //ui.pushButton_volume->setEnabled(false);
  //  ////禁用slider_progress，连接信号槽
  //  //ui.slider_progress->setEnabled(false);
  //  //connect(ui.slider_progress,&CustomSlider::costomSliderClicked,this,&MediaPlayer::slider_progress_clicked);
  //  //connect(ui.slider_progress,&CustomSlider::sliderMoved,this,&MediaPlayer::slider_progress_moved);
  //  //connect(ui.slider_progress,&CustomSlider::sliderReleased,this,&MediaPlayer::slider_progress_released);
  //  //手动设置slider_volume 包括初始化 方向 禁用，以及槽函数
  //  slider_volume = new CustomSlider(this);
  //  slider_volume->setOrientation(Qt::Vertical);
  //  slider_volume->setEnabled(false);
  //  /*slider_volume->setStyleSheet("QSlider {background:rgba(52,7,91,100%);border-color:rgba(52,7,91,100%)}"
  //                               "QSlider::groove:vertical {background:#cbcbcb;width:6px;border-radius:1px;padding-left:-1px;padding-right:-1px;padding-top:-1px;padding-bottom:-1px;}"
  //                               "QSlider::sub-page:vertical{ background:#cbcbcb;border-radius:2px;}"
  //                               "QSlider::add-page:vertical {  background: qlineargradient(x1 : 0, y1 : 0, x2 : 0, y2 : 1,stop : 0 #439cf4, stop:1 #439cf4);background: qlineargradient(x1 : 0, y1 : 0.2, x2 : 1, y2 : 1,stop : 0 #439cf4, stop: 1 #439cf4);width:10px;border-radius:2px;}"
  //                               "QSlider::handle:vertical {margin: -2px - 7px - 2px - 7px;height: 17px;}"
  //                           );*/
  //  slider_volume->hide();
  //  ////由于不涉及到slider值的刷新，因此只需对move和自定义click两个信号进行处理，并且可以共用一个槽函数
  //  //connect(slider_volume,&CustomSlider::costomSliderClicked,this,&MediaPlayer::slider_volume_changed);
  //  //connect(slider_volume,&CustomSlider::sliderMoved,this,&MediaPlayer::slider_volume_changed);
  //  //connect(slider_volume,&CustomSlider::sliderReleased,this,&MediaPlayer::slider_released);


  //  //实例化三个全局变量
  //  layout_video = new QVBoxLayout;
}

int VideoPlayer::play_flag = 0;

////播放进度控制Slider的槽函数
//void MediaPlayer::slider_progress_clicked()
//{
//	m_videoPlayer->setPosition(ui.slider_progress->value()*m_videoPlayer->duration()/maxValue);
//}
//
//void MediaPlayer::slider_progress_moved()
//{
//    //暂时停止计时器，在用户拖动过程中不修改slider的值
//    timer->stop();
//	m_videoPlayer->setPosition(ui.slider_progress->value()*m_videoPlayer->duration()/maxValue);
//}
//
//void MediaPlayer::slider_progress_released()
//{
//    //用户释放滑块后，重启定时器
//    timer->start();
//}
//
////音量控制Slider的槽函数
//void MediaPlayer::slider_volume_changed()
//{
//    qDebug() << "slider_volume->value() = " << slider_volume->value();
//    if(status == true)
//    {
//        ui.pushButton->show();
//    }
//    else
//    {
//       ui.pushButton->hide();
//    }
//	m_videoPlayer->setVolume(slider_volume->value());
//    ui.pushButton->setText(QString("%1").arg(slider_volume->value()));
//}
//
//void MediaPlayer::slider_released()
//{
//    status == false;
//    if(status == true)
//    {
//        ui.pushButton->show();
//    }
//    else
//    {
//       ui.pushButton->hide();
//    }
//}


VideoPlayer::~VideoPlayer()
{
}

void VideoPlayer::initVideoPlayModel()
{
	//初始化播放器对象
	m_videoPlayer = new QMediaPlayer(this);
	//初始化列表
	m_listInfo.clear();
	m_videoPlayList = new QMediaPlaylist(this);
	m_videoPlayList->setPlaybackMode(QMediaPlaylist::Sequential);
	m_videoPlayer->setVolume(50);
	m_videoPlayer->setPlaylist(m_videoPlayList);
	//初始化播放窗口
	m_videoPlayWidget = new QVideoWidget();
	m_videoPlayWidget->setStyleSheet("border:none;background-color:transparent;");
	layout_video = new QVBoxLayout();
	layout_video->setMargin(0);
	m_videoPlayWidget->resize(ui.label_player->size());
	//m_videoPlayWidget->setAspectRatioMode(Qt::IgnoreAspectRatio);
	layout_video->addWidget(m_videoPlayWidget);
	ui.label_player->setLayout(layout_video);
	m_videoPlayer->setVideoOutput(m_videoPlayWidget);
	//进度同步
	connect(m_videoPlayer, SIGNAL(durationChanged(qint64)), this, SLOT(sltDurationChanged(qint64)));
	connect(m_videoPlayer, SIGNAL(positionChanged(qint64)), this, SLOT(sltPositionChanged(qint64)));
}

void VideoPlayer::sltMaxOrNormal()
{
	if (isMaximized())
		showNormal();
	else
		showMaximized();
}

void VideoPlayer::sltOpenLocalVideoList()
{
	auto fileList = QFileDialog::getOpenFileNames(this,
		QStringLiteral("选择视频文件"),
		".",
		QStringLiteral("视频文件(*.avi *.mp4 *.flv *.mkv);音频文件(*.mp3 *.wav *.wma)"));

	if (fileList.isEmpty()) return;
	ui.listWidget_localList->clear();
	//存储数据实体
	for (auto &each : fileList) {
		QFileInfo fileInfo(each);
		if (fileInfo.exists()) {
			m_videoPlayList->addMedia(QUrl::fromLocalFile(each));
			auto videoName = fileInfo.fileName();
			m_listInfo.append(videoName);
		}
	}
	//遍历数据实体来填充列表
	for (int i = 0; i < m_listInfo.size(); i++) {
		auto fileName = m_listInfo.value(i);
		ui.listWidget_localList->insertItem(i, fileName);
	}
	if (QMediaPlayer::StoppedState == m_videoPlayer->state()) {
		m_videoPlayer->play();
	}
}

void VideoPlayer::sltDurationChanged(qint64 duration)
{
	//设置进度条
	//ui.slider_progress->setMaximum(duration);
	//总时间
	QTime allTime(0, duration / 60000, qRound((duration % 60000) / 1000.0));
	m_videoTime = allTime.toString(tr("mm:ss"));
}

void VideoPlayer::sltPositionChanged(qint64 position)
{
	//if (ui.slider_progress->isSliderDown()) return;
	//ui.slider_progress->setSliderPosition(position);
	QTime duration(0, position / 60000, qRound((position % 60000) / 1000.0));
	auto localTime = duration.toString(tr("mm:ss"));
	//ui.label_volumNum->setText(QString("%1/%2").arg(localTime).arg(m_musicTime));
}

void VideoPlayer::sltListWidgetDoubleClicked(QListWidgetItem * item)
{
	if (item) {
		auto row = ui.listWidget_localList->currentRow();
		m_videoPlayList->setCurrentIndex(row);
		m_videoPlayer->play();
	}
}

//void MediaPlayer::on_pushButton_open_file_clicked()
//{
//    if(play_flag==1)
//    {
//		m_videoPlayer->stop();
//        timer->stop();
//    }
//
//    layout_video->setMargin(1);
//    //根据label_player尺寸设置播放区域
//	m_videoPlayWidget->resize(ui.label_player->size());
//    layout_video->addWidget(m_videoPlayWidget);
//    ui.label_player->setLayout(layout_video);
//	m_videoPlayer->setVideoOutput(m_videoPlayWidget);
//
//    //选择视频文件
//    QString filename = QFileDialog::getOpenFileName(this,tr("选择视频文件"),".",tr("视频格式(*.avi *.mp4 *.flv *.mkv)"));
//    QFile file(filename);
//    if(!file.open(QIODevice::ReadOnly))
//    {
//        QMessageBox::information(NULL, "提示", "请重新选择视频文件", QMessageBox::Ok, QMessageBox::Ok);
//        return;
//    }
//
//    //设置播放器
//   // m_videoPlayer->setMedia(QUrl::fromLocalFile(filename));
//	m_videoPlayList->addMedia(QUrl("http://fs.mv.web.kugou.com/202107270955/5018e19c140d5adf019a877ae12f31b2/G031/M08/12/1D/_5MEAFXd0KSAZMLpARSo2wZan7k579.mp4"));
//	//m_videoPlayer->setMedia(QUrl::fromLocalFile("http://fs.mv.web.kugou.com/202107270955/5018e19c140d5adf019a877ae12f31b2/G031/M08/12/1D/_5MEAFXd0KSAZMLpARSo2wZan7k579.mp4"));
//    //play_state为true表示播放，false表示暂停
//    play_state = true;
//    //启用播放/暂停按钮，并将其文本设置为“暂停”
//    ui.pushButton_play_and_pause->setEnabled(true);
//    ui.pushButton_play_and_pause->setText("暂停");
//    //播放器开启
//	m_videoPlayer->play();
//    play_flag=1;
//
//    //启用slider并设置范围
//    ui.slider_progress->setEnabled(true);
//    ui.slider_progress->setRange(0,maxValue);
//    timer->start();
//
//    //启用音量控制slider
//    slider_volume->setEnabled(true);
//    slider_volume->setRange(0, 100);
//    //启用音量按钮
//    ui.pushButton_volume->setEnabled(true);
//}

//void MediaPlayer::on_pushButton_play_and_pause_clicked()
//{
//    //反转播放状态
//    if(play_state)
//    {
//		m_videoPlayer->pause();
//        ui.pushButton_play_and_pause->setText("播放");
//    }
//    else
//    {
//		m_videoPlayer->play();
//        ui.pushButton_play_and_pause->setText("暂停");
//    }
//
//    play_state = !play_state;
//}
//
//void MediaPlayer::onTimerOut()
//{
//    ui.slider_progress->setValue(m_videoPlayer->position()*maxValue/ m_videoPlayer->duration());
//}
//
//
//bool state_slider_volume = false;
//void MediaPlayer::on_pushButton_volume_clicked()
//{
//    qDebug()<<state_slider_volume;
//    //通过hide()和show()方法，实现音量控制Slider的唤出和收起
//    if(state_slider_volume)
//    {
//        slider_volume->hide();
//    }
//    else
//    {
//        slider_volume->setValue(m_videoPlayer->volume());
//        //计算位置，使其位于音量控制按钮的上方
//        slider_volume->setGeometry(QRect(ui.pushButton_volume->pos().rx()+0.5*ui.pushButton_volume->width()-15,
//                                         ui.pushButton_volume->y()-100 , 30, 102));
//        slider_volume->show();
//    }
//    state_slider_volume = !state_slider_volume;
//}
//void MediaPlayer::on_pushButton_flont_clicked()
//{
//    if(play_flag==1)
//    {
//		m_videoPlayer->stop();
//        timer->stop();
//    }
//    this->close();
//    emit mySignal();
//}

void VideoPlayer::mouseMoveEvent(QMouseEvent * event)
{
	//判断左键是否被按下，只有左键按下了，其返回值就是1(true)
	if ((event->buttons() & Qt::LeftButton) && m_bMove)
	{
		move(event->globalPos() - m_point);
	}
	QWidget::mouseMoveEvent(event);
}

void VideoPlayer::mousePressEvent(QMouseEvent * event)
{
	//鼠标事件中包含“按住的是左键”
	if ((event->button() == Qt::LeftButton) && (event->pos().y() < 56) && (event->pos().y() > 0))
	{
		m_bMove = true;
		//获取移动的位移量
		m_point = event->globalPos() - frameGeometry().topLeft();
	}
}

void VideoPlayer::mouseReleaseEvent(QMouseEvent * event)
{
	m_bMove = false;
}