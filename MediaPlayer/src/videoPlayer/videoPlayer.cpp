#include "videoPlayer.h"
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QVideoWidget>
#include <QMouseEvent>
#include <QTime>
#include <QFileDialog>
#include <QScrollBar>
#include "videoControls.h"

VideoPlayer::VideoPlayer(QWidget *parent) :
	QWidget(parent), m_position(0), m_volume(50)
{
    ui.setupUi(this);

	setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::WindowMinimizeButtonHint);
	//setAttribute(Qt::WA_TranslucentBackground);
	ui.label_player->installEventFilter(this);

	//初始化播放器模块
	initVideoPlayModel();
	initVideoControls();

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
	//返回主界面
	connect(ui.pushButton_return, &QPushButton::clicked, this, &VideoPlayer::sltReturnPanel);
	//播放测试链接
	connect(ui.pushButton_movies, &QPushButton::clicked, this, [=]() {
		auto each = "http://fs.mv.web.kugou.com/202108231143/71a82e9f09cba1b55a29b3826f2bd504/G248/M04/0F/19/mIcBAF-6ABGAPdiEAg2aYeYrfdA165.mp4";
		m_videoPlayer->setMedia(QMediaContent(QUrl::fromLocalFile(each)));
		//m_videoPlayList->addMedia(QMediaContent(QUrl::fromLocalFile("http://example.com/myclip2.mp4")));
		m_videoPlayer->play();
		auto name = m_videoPlayList->currentMedia().canonicalUrl().fileName();
		ui.label_videoName->setText(name);
	});
}

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
	m_videoPlayer->setVolume(m_volume);
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

void VideoPlayer::initVideoControls()
{
	m_videoControls = QSharedPointer<VideoControls>(new VideoControls(this));
	if (m_videoControls) {
		m_videoControls->installEventFilter(this);
		m_videoControls->setVolume(m_volume);
		connect(m_videoControls.data(), &VideoControls::sigVideoPlayOrPause, this, &VideoPlayer::sltVideoPlayOrPause);
		connect(m_videoControls.data(), &VideoControls::sigNextVideoPlay, this, &VideoPlayer::sltNextVideoPlay);
		connect(m_videoControls.data(), &VideoControls::sigSetPlayCycle, this, &VideoPlayer::sltSetPlayCycle);
		connect(m_videoControls.data(), &VideoControls::sigSetPosition, this, &VideoPlayer::sltSetPosition);
		connect(m_videoControls.data(), &VideoControls::sigSoundVoiceValue, this, &VideoPlayer::sltSoundVoiceValue);
	}
}

void VideoPlayer::locateWidgets()
{
	if (m_videoControls) {
		int posX = ui.widget_video->x();
		int posY = ui.widget_top->height() + ui.widget_video->height() + 6 - 60 - 180;
		m_videoControls->setFixedWidth(width() - posX - 10);
		m_videoControls->move(mapToGlobal(QPoint(posX, posY)));
		m_videoControls->locateWidgets();
	}
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
		QStringLiteral("选择媒体文件"),
		".",
		QStringLiteral("媒体文件(*.avi *.mp4 *.flv *.mkv*.mp3 *.wav *.wma);"));

	if (fileList.isEmpty()) return;
	ui.listWidget_localList->clear();
	//存储数据实体
	for (auto &each : fileList) {
		QFileInfo fileInfo(each);
		if (fileInfo.exists()) {
			m_videoPlayList->addMedia(QUrl::fromLocalFile(each));
			auto videoName = fileInfo.fileName();
			if (!m_listInfo.contains(videoName)) {
				m_listInfo.append(videoName);
			}
		}
	}
	//遍历数据实体来填充列表
	for (int i = 0; i < m_listInfo.size(); i++) {
		auto fileName = m_listInfo.value(i);
		ui.listWidget_localList->insertItem(i, fileName);
	}
	if (QMediaPlayer::StoppedState == m_videoPlayer->state()) {
		m_videoPlayer->play();
		auto name = m_videoPlayList->currentMedia().canonicalUrl().fileName();
		ui.label_videoName->setText(name);
		if (m_videoControls) {
			m_videoControls->setPlaying();
		}
	}
}

void VideoPlayer::sltDurationChanged(qint64 duration)
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

void VideoPlayer::sltPositionChanged(qint64 position)
{
	if (!m_videoControls) {
		return;
	}
	if (m_videoControls->isSliderDown()) {
		return;
	}
	m_position = position;
	m_videoControls->setSliderPosition(position);
	auto hh = position / 3600000;
	auto mm = (position % 3600000) / 60000.0;
	auto ss = (position % 60000) / 1000.0;
	QTime duration(hh, mm, ss);
	auto localTime = duration.toString(tr("hh:mm:ss"));
	auto text = QString("%1/%2").arg(localTime).arg(m_videoTime);
	m_videoControls->setProgressText(text);
}

void VideoPlayer::sltListWidgetDoubleClicked(QListWidgetItem * item)
{
	if (item) {
		auto row = ui.listWidget_localList->currentRow();
		m_videoPlayList->setCurrentIndex(row);
		m_videoPlayer->play();
		auto name = m_videoPlayList->currentMedia().canonicalUrl().fileName();
		ui.label_videoName->setText(name);
		if (m_videoControls) {
			m_videoControls->setPlaying();
		}
	}
}

void VideoPlayer::sltVideoPlayOrPause(bool state)
{
	if (state) {
		m_videoPlayer->play();
	}
	else {
		m_videoPlayer->pause();
	}
}

void VideoPlayer::sltNextVideoPlay()
{
	if (0 != m_videoPlayList->currentIndex()) {
		m_videoPlayList->setCurrentIndex(m_videoPlayList->previousIndex());
		m_videoPlayer->play();
	}
	else {
		m_videoPlayList->setCurrentIndex(m_videoPlayList->mediaCount() - 1);
		m_videoPlayer->play();
	}
}

void VideoPlayer::sltSetPlayCycle(bool state)
{
	if (state) {
		m_videoPlayList->setPlaybackMode(QMediaPlaylist::CurrentItemInLoop);
	}
	else {
		m_videoPlayList->setPlaybackMode(QMediaPlaylist::Sequential);
	}
}

void VideoPlayer::sltSoundVoiceValue(int value)
{
	m_videoPlayer->setVolume(value);
}

void VideoPlayer::sltSetPosition(int value)
{
	m_videoPlayer->setPosition(value);
}

void VideoPlayer::sltReturnPanel()
{
	if (m_videoPlayer->state() != QMediaPlayer::StoppedState) {
		m_videoPlayer->stop();
	}
	emit sigReturnPanel();
	close();
}

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
	QWidget::mousePressEvent(event);
}

void VideoPlayer::mouseReleaseEvent(QMouseEvent * event)
{
	m_bMove = false;
	QWidget::mouseReleaseEvent(event);
}

void VideoPlayer::showEvent(QShowEvent * event)
{
	QWidget::showEvent(event);
	locateWidgets();
}

void VideoPlayer::closeEvent(QCloseEvent * event)
{
	if (m_videoControls) {
		m_videoControls->closeWidget();
	}
	QWidget::closeEvent(event);
}

void VideoPlayer::moveEvent(QMoveEvent * event)
{
	QWidget::moveEvent(event);
	locateWidgets();
}

void VideoPlayer::hideEvent(QHideEvent * event)
{
	if (m_videoControls) {
		m_videoControls->setVisible(false);
	}
	locateWidgets();
	QWidget::hideEvent(event);
}

void VideoPlayer::resizeEvent(QResizeEvent * event)
{
	QWidget::resizeEvent(event);
	locateWidgets();
}

bool VideoPlayer::eventFilter(QObject * obj, QEvent * event)
{
	if (obj == Q_NULLPTR) {
		return false;
	}
	if (m_videoControls == Q_NULLPTR) {
		return false;
	}

	if (QEvent::Enter == event->type()) {
		if (obj == ui.label_player || obj == m_videoControls) {
			if (QMediaPlayer::StoppedState != m_videoPlayer->state()) {
				m_videoControls->setVisible(true);
			}
			return false;
		}
	}
	else if (QEvent::Leave == event->type()) {
		if (obj != ui.label_player || obj != m_videoControls) {
			auto volumIsVisible = m_videoControls->getVolumVisible();
			if (!volumIsVisible) {
				m_videoControls->setVisible(false);
			}
			return false;
		}
	}
	return QWidget::eventFilter(obj, event);
}

//快进快退，时间为15s; 音量调节，大小间隔为5
void VideoPlayer::keyReleaseEvent(QKeyEvent *event)
{
	auto key = event->key();
	switch (key)
	{
	case Qt::Key_Left: {
		if (QMediaPlayer::PlayingState == m_videoPlayer->state()) {
			m_position = m_position - (15 * 1000);
			m_videoPlayer->setPosition(m_position);
		}
		break;
	}
	case Qt::Key_Right: {
		if (QMediaPlayer::PlayingState == m_videoPlayer->state()) {
			m_position = m_position + (15 * 1000);
			m_videoPlayer->setPosition(m_position);
		}
		break;
	}
	case Qt::Key_Up: {
		if (QMediaPlayer::PlayingState == m_videoPlayer->state()) {
			m_volume = m_volume + 5;
			//最大不超过100
			m_volume = qMin(m_volume, 100);
			m_videoControls->setVolume(m_volume);
		}
		break;
	}
	case Qt::Key_Down: {
		if (QMediaPlayer::PlayingState == m_videoPlayer->state()) {
			m_volume = m_volume - 5;
			//最小不小过0
			m_volume = qMax(m_volume, 0);
			m_videoControls->setVolume(m_volume);
		}
		break;
	}
	default:
		break;
	}
	QWidget::keyReleaseEvent(event);
}
