#include "musicplayer.h"
#include <QMouseEvent>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QFileDialog>
#include <QScrollBar>
#include "vsliderwidget.h"
#include "LrcWidget.h"
#include <QWebEngineSettings>
#include "musicManager.h"
#include "musicInfoDelegate.h"
#include <QJsonParseError>
#include <QJsonObject>
#include <QJsonArray>
#include <QNetworkReply>

//设置循环按钮属性
constexpr char* Property_id = "id";
//播放顺序
enum E_PlatType
{
	E_SequentialPlay = 1,//顺序播放
	E_RandomPlay = 2,//随机播放
	E_LoopPlay = 3, //列表播放
	E_CurrentItemInLoopPlay = 4, //单曲循环
};

MusicPlayer::MusicPlayer(QWidget *parent) :
    QWidget(parent), m_filePath(QString()), m_musicTime(QString())
{
    ui.setupUi(this);

	setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::WindowMinimizeButtonHint);
	setAttribute(Qt::WA_TranslucentBackground);

	//音乐播放模块
	initMusicPlayer();

	connect(ui.pushButton_min, &QPushButton::clicked, this, &MusicPlayer::showMinimized);
	connect(ui.pushButton_close, &QPushButton::clicked, this, &MusicPlayer::close);
	/*connect(ui.pushButton_close, &QPushButton::clicked, this, [=] {
		exit(0);
	});*/
	connect(ui.pushButton_max, &QPushButton::clicked, this, &MusicPlayer::sltMaxOrNormal);

	//禁用slider_progress，连接信号槽(进度条)
	//ui.slider_progress->setEnabled(false);
	connect(ui.slider_progress,	&CustomSlider::costomSliderClicked,	this,	&MusicPlayer::sltSliderProgressClicked);
	connect(ui.slider_progress,	&CustomSlider::sliderReleased,	this,	&MusicPlayer::sltSliderProgressReleased);
	//播放控件
	connect(ui.pushButton_play, &QPushButton::clicked, this, &MusicPlayer::sltMusicPlayOrPause);
	connect(ui.pushButton_next, &QPushButton::clicked, this, &MusicPlayer::sltNextMusicPlay);
	connect(ui.pushButton_prev, &QPushButton::clicked, this, &MusicPlayer::sltPrevMusicPlay);
	//本地歌曲
	connect(ui.pushButton_local, &QPushButton::clicked, this, [=]() {
		ui.stackedWidget->setCurrentWidget(ui.page_localList);
	});
	//导入歌曲
	connect(ui.pushButton_open, &QPushButton::clicked, this, &MusicPlayer::sltOpenLocalMusicList);
	//播放顺序
	ui.pushButton_cycle->setProperty(Property_id, E_SequentialPlay);
	connect(ui.pushButton_cycle, &QPushButton::clicked, this, &MusicPlayer::sltSetPlayCycle);
	//返回主界面
	connect(ui.pushButton_return, &QPushButton::clicked, this, &MusicPlayer::sltReturnPanel);
	//搜索
	connect(ui.pushButton_search, &QPushButton::clicked, this, &MusicPlayer::sltSearchMusic);
	//播放搜索列表里的歌
	connect(&musicManager, &MusicManager::sigMouseDoubleClicked, this, &MusicPlayer::sltMouseDoubleClicked);
	//播放本地的歌
	connect(&musicManager, &MusicManager::sigMouseDoubleClicked_local, this, &MusicPlayer::sltPlayListClicked);
	//网络播放模块
	m_netWorkMusicPlay = new QNetworkAccessManager(this);
	connect(m_netWorkMusicPlay, &QNetworkAccessManager::finished, this, &MusicPlayer::sltNetWorkMusicPlay, Qt::DirectConnection);
	//网络下载模块
	m_netWorkDownLoad = new QNetworkAccessManager(this);
	connect(m_netWorkDownLoad, &QNetworkAccessManager::finished, this, &MusicPlayer::sltDownLoadByNetWork, Qt::DirectConnection);
	//点击歌曲封面
	connect(ui.label_image, &MusicImage::sigMouseClicked, this, [=]() {
		ui.page_lrc->setMusicInfo();
		ui.stackedWidget->setCurrentWidget(ui.page_lrc);
	});
	//下载接口
	connect(ui.pushButton_download, &QPushButton::clicked, this, &MusicPlayer::sltDownLoadButtonClick);

	//声音进度条初始化
	initVolumeSlider();

	//歌词模块初始化
	initLrcModel();

	//web模块初始化
	initWebModel();
}

MusicPlayer::~MusicPlayer()
{
}

void MusicPlayer::initMusicPlayer()
{
	//初始化播放器对象
	m_musicPlayer = new QMediaPlayer(this);
	//初始化列表
	m_musicPlayList = new QMediaPlaylist(this);
	m_musicPlayList->setPlaybackMode(QMediaPlaylist::Sequential);
	//m_musicPlayList->setPlaybackMode(QMediaPlaylist::Loop);
	m_musicPlayer->setVolume(50);
	m_musicPlayer->setPlaylist(m_musicPlayList);
	//设置positionChanged信号发送频率，毫秒级别
	m_musicPlayer->setNotifyInterval(500);
	//进度同步
	connect(m_musicPlayer, SIGNAL(durationChanged(qint64)), this, SLOT(sltDurationChanged(qint64)));
	connect(m_musicPlayer, SIGNAL(positionChanged(qint64)), this, SLOT(sltPositionChanged(qint64)));
	//优化滑动条
	QFile QSS1(":/qss/image/qss/whiteScrollbar.qss");
	if (QSS1.open(QIODevice::ReadOnly)) {
		QString strStyle = QSS1.readAll();
		ui.listView->verticalScrollBar()->setStyleSheet(strStyle);
	}
	//列表样式
	QFile QSS2(":/qss/image/qss/listViews.qss");
	if (QSS2.open(QIODevice::ReadOnly)) {
		QString strStyle = QSS2.readAll();
		ui.listView->setStyleSheet(strStyle);
	}
	//数据绑定
	ui.listView->setModel(&musicManager.getLocalMusicInfoModel());
	ui.listView->setSelectionMode(QAbstractItemView::SingleSelection);
	ui.listView->setEditTriggers(QAbstractItemView::NoEditTriggers);
	ui.listView->setUpdatesEnabled(true);
	ui.listView->setItemDelegate(new MusicInfoDelegate(E_Local));
}

void MusicPlayer::initLrcModel()
{
	m_lrcWidget = QSharedPointer<LrcWidget>(new LrcWidget());
	connect(ui.pushButton_lrc, &QPushButton::clicked, this, &MusicPlayer::sltShowLrcModel);
}

void MusicPlayer::initVolumeSlider()
{
	m_volumeSlider = new VSliderWidget(this);
	m_volumeSlider->setVoiceValue(50);
	m_volumeSlider->setVisible(false);
	connect(m_volumeSlider, &VSliderWidget::sigValueChanged, this, &MusicPlayer::sltSoundVoiceValue);
	connect(ui.pushButton_volum, &QPushButton::clicked, this, &MusicPlayer::sltShowVolumeSlider);
}

void MusicPlayer::initWebModel()
{
	ui.stackedWidget->setCurrentWidget(ui.page_web);
	connect(ui.pushButton, &QPushButton::clicked, this, [=]() {ui.stackedWidget->setCurrentWidget(ui.page_web);});
	//设置webEngine属性
	//禁止右键菜单
	ui.webEngineView->setContextMenuPolicy(Qt::NoContextMenu);
	//设置页面透明
	ui.webEngineView->page()->setBackgroundColor(Qt::transparent);
	//隐藏滑动条
	ui.webEngineView->page()->settings()->setAttribute(QWebEngineSettings::ShowScrollBars, false);
	//写入本地html
	//从html文件中读取内容后写入webview
	QString htmlPath = "E:/shaw/test/MediaPlayer/MediaPlayer/web/main.html";
	QFile file(htmlPath);
	if (!file.exists()){
		return;
	}
	ui.webEngineView->load(QUrl("file:///" + htmlPath));
}

void MusicPlayer::checkLrcWidget(int position)
{
	auto lrc = musicManager.getLrcByTime(position);
	if (!lrc.isEmpty()) {
		m_lrcWidget->setLrc(lrc);
	}
}

void MusicPlayer::parseJsonSongInfo(const QString & json)
{
	QByteArray byte_array;
	QJsonParseError json_error;
	QJsonDocument parse_doucment = QJsonDocument::fromJson(byte_array.append(json), &json_error);
	if (parse_doucment.isNull()) {
		return;
	}
	if (json_error.error == QJsonParseError::NoError){
		if (parse_doucment.isObject()){
			QJsonObject rootObj = parse_doucment.object();
			if (rootObj.contains("data")){
				QJsonValue valuedata = rootObj.value("data");
				if (valuedata.isObject()){
					QJsonObject valuedataObject = valuedata.toObject();
					//播放
					QString url = musicManager.getJsonData(valuedataObject, "play_url");
					m_musicPlayList->addMedia(QUrl::fromLocalFile(url));
					m_musicPlayList->setCurrentIndex(m_musicPlayList->mediaCount() - 1);
					m_musicPlayer->play();
					//音频名字显示
					QString name = musicManager.getJsonData(valuedataObject, "audio_name");
					ui.label_name->setText(name);
					//歌名,歌手，专辑
					MusicInfo info;
					info.musicName = musicManager.getJsonData(valuedataObject, "song_name");
					info.musicPlayer = musicManager.getJsonData(valuedataObject, "author_name");
					info.musicAlbum = musicManager.getJsonData(valuedataObject, "album_name");
					info.url = url;
					musicManager.savePlayingMusicInfo(info);
					//启用播放/暂停按钮，并将其文本设置为“暂停”
					ui.pushButton_play->setChecked(true);
					//歌词获取
					musicManager.getLrcJsonData(valuedataObject, "lyrics");
					//图片显示
					auto imageUrl = musicManager.getJsonData(valuedataObject, "img");
					ui.label_image->requestImage(imageUrl);
				}
				else
				{
					qDebug() << "出错";
				}
			}
		}
	}
}

void MusicPlayer::sltMaxOrNormal()
{
	if (isMaximized())
		showNormal();
	else
		showMaximized();
}

void MusicPlayer::sltSliderProgressClicked()
{
	m_musicPlayer->setPosition(ui.slider_progress->value());
}

void MusicPlayer::sltSliderProgressReleased()
{
	m_musicPlayer->setPosition(ui.slider_progress->value());
}

void MusicPlayer::sltMusicPlayOrPause()
{
	auto isPlay = ui.pushButton_play->isChecked();
	if (isPlay) {
		m_musicPlayer->play();
	}
	else {
		m_musicPlayer->pause();
	}
}

void MusicPlayer::sltNextMusicPlay()
{
	if(0 != m_musicPlayList->currentIndex()){
        m_musicPlayList->setCurrentIndex(m_musicPlayList->previousIndex());
        m_musicPlayer->play();
    }
    else{
		m_musicPlayList->setCurrentIndex(m_musicPlayList->mediaCount() - 1);
		m_musicPlayer->play();
    }
}

void MusicPlayer::sltPrevMusicPlay()
{
	if (1 != (m_musicPlayList->mediaCount() - m_musicPlayList->currentIndex())){
		m_musicPlayList->setCurrentIndex(m_musicPlayList->nextIndex());
		m_musicPlayer->play();
	}
	else{
		m_musicPlayList->setCurrentIndex(0);
		m_musicPlayer->play();
	}
}

void MusicPlayer::sltOpenLocalMusicList()
{
	auto fileList = QFileDialog::getOpenFileNames(this,
		QStringLiteral("选择音频文件"),
		".",
		QStringLiteral("音频文件(*.mp3 *.wav *.wma)mp3文件(*.mp3);;wav文件(*.wav);;wma文件(*.wma);;所有文件(*.*)"));

	if (fileList.isEmpty()) return;
	musicManager.getLocalMusicInfoModel().clear();
	for (auto & each : fileList) {
		QFileInfo fileInfo(each);
		if (fileInfo.exists()) {
			//添加到列表
			m_musicPlayList->addMedia(QUrl::fromLocalFile(each));
			MusicInfo info;
			info.musicName = fileInfo.fileName();
			m_filePath = each.replace(info.musicName, "");
			musicManager.getLocalMusicInfoModel().addInfoToModel(info);
		}
	}
	////先停止播放器组件
	//m_musicPlayer->stop();
	////播放列表模块
	//m_musicPlayList->clear();
}

void MusicPlayer::sltPlayListClicked(int row)
{
    m_musicPlayList->setCurrentIndex(row);
    m_musicPlayer->play();

    //启用播放/暂停按钮，并将其文本设置为“暂停”
	ui.pushButton_play->setChecked(true);
	//歌名
	auto name = m_musicPlayList->currentMedia().canonicalUrl().fileName();
	ui.label_name->setText(name);
	//歌词信息
	QString path = m_filePath + name.split(".").front() + ".lrc";
	musicManager.getLoaclLrcData(path);
}

void MusicPlayer::sltDurationChanged(qint64 duration)
{
	//设置进度条
	ui.slider_progress->setMaximum(duration);
	//总时间
	QTime allTime(0, duration / 60000, qRound((duration % 60000) / 1000.0));
	m_musicTime = allTime.toString(tr("mm:ss"));
}

void MusicPlayer::sltPositionChanged(qint64 position)
{
	if (ui.slider_progress->isSliderDown()) return;
	ui.slider_progress->setSliderPosition(position);
	QTime duration(0, position / 60000, qRound((position % 60000) / 1000.0));
	auto localTime = duration.toString(tr("mm:ss"));
	ui.label_volumNum->setText(QString("%1/%2").arg(localTime).arg(m_musicTime));
	//歌词校验
	checkLrcWidget(position);
	ui.page_lrc->setLrcInfo(position);
}

void MusicPlayer::sltSoundVoiceValue(int value)
{
	if (0 == value) {
		ui.pushButton_volum->setStyleSheet("border-image: url(:/musicPlayer/image/musicPlayer/jingyin.png);");
	}
	else {
		ui.pushButton_volum->setStyleSheet("border-image: url(:/musicPlayer/image/musicPlayer/shengyin.png);");
	}
	m_musicPlayer->setVolume(value);
}

void MusicPlayer::sltShowVolumeSlider()
{
	int posX = ui.pushButton_volum->x() + ui.widget_bCenter->x() + ui.widget_left->width() - 4;
	int posY = ui.pushButton_volum->y() + ui.widget_bottom->y() - m_volumeSlider->height() - 10;
	m_volumeSlider->move(posX, posY);
	auto isVisible = m_volumeSlider->isVisible();
	m_volumeSlider->setVisible(!isVisible);
}

void MusicPlayer::sltSetPlayCycle()
{
	auto playType = ui.pushButton_cycle->property(Property_id).toInt();
	switch (playType)
	{
	case E_SequentialPlay: {
		//当前顺序播放，切换到随机播放
		ui.pushButton_cycle->setProperty(Property_id, E_RandomPlay);
		m_musicPlayList->setPlaybackMode(QMediaPlaylist::Random);
		ui.pushButton_cycle->setStyleSheet("border-image: url(:/musicPlayer/image/musicPlayer/suiji.png);");
		break;
	}
	case E_RandomPlay: {
		//当前随机播放，切换到列表循环
		ui.pushButton_cycle->setProperty(Property_id, E_LoopPlay);
		m_musicPlayList->setPlaybackMode(QMediaPlaylist::Loop);
		ui.pushButton_cycle->setStyleSheet("border-image: url(:/musicPlayer/image/musicPlayer/danquxunhuan.png);");
		break;
	}
	case E_LoopPlay: {
		//当前列表循环播放，切换到单曲循环
		ui.pushButton_cycle->setProperty(Property_id, E_CurrentItemInLoopPlay);
		m_musicPlayList->setPlaybackMode(QMediaPlaylist::CurrentItemInLoop);
		ui.pushButton_cycle->setStyleSheet("border-image: url(:/musicPlayer/image/musicPlayer/danqu.png);");
		break;
	}
	case E_CurrentItemInLoopPlay:{
		//当前单曲循环播放，切换到顺序播放
		ui.pushButton_cycle->setProperty(Property_id, E_SequentialPlay);
		m_musicPlayList->setPlaybackMode(QMediaPlaylist::Sequential);
		ui.pushButton_cycle->setStyleSheet("border-image: url(:/musicPlayer/image/musicPlayer/xunhuan.png);");
		break;
	}
	default:
		break;
	}
}

void MusicPlayer::sltShowLrcModel()
{
	auto isVisible = m_lrcWidget->isVisible();
	m_lrcWidget->setVisible(!isVisible);
}

void MusicPlayer::sltReturnPanel()
{
	if (m_musicPlayer->state() != QMediaPlayer::StoppedState) {
		m_musicPlayer->stop();
	}
	emit sigReturnPanel();
	close();
}

void MusicPlayer::sltSearchMusic()
{
	auto word = ui.lineEdit_search->text();
	ui.page_search->searchMusic(word);
	ui.stackedWidget->setCurrentWidget(ui.page_search);
}

void MusicPlayer::sltMouseDoubleClicked(int row)
{
	auto hash = musicManager.getMusicInfoModel().getHash(row);
	auto albumId = musicManager.getMusicInfoModel().getAlbumId(row);
	QString url = QString("http://www.kugou.com/yy/index.php?r=play/getdata"
		"&hash=%1&album_id=%2&_=1497972864535").arg(hash).arg(albumId);
	//设置请求数据
	QNetworkRequest request;
	request.setUrl(QUrl(url));
	//不加头无法得到json，可能是为了防止机器爬取
	request.setRawHeader("Cookie", "kg_mid=2333");
	request.setHeader(QNetworkRequest::CookieHeader, 2333);
	m_netWorkMusicPlay->get(request);
}

void MusicPlayer::sltNetWorkMusicPlay(QNetworkReply * reply)
{
	//获取响应的信息，状态码为200表示正常
	QVariant status_code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
	//无错误返回
	if (reply->error() == QNetworkReply::NoError){
		QByteArray bytes = reply->readAll();  //获取字节
		QString result(bytes);  //转化为字符串
		parseJsonSongInfo(result);
	}
}

void MusicPlayer::sltDownLoadButtonClick()
{
	ui.pushButton_download->setEnabled(false);
	QNetworkRequest request;
	//设置请求数据
	auto info = musicManager.getPlayingMusicInfo();
	request.setUrl(QUrl(info.url));
	request.setHeader(QNetworkRequest::UserAgentHeader, "RT-Thread ART");
	m_netWorkDownLoad->get(request);
}

void MusicPlayer::sltDownLoadByNetWork(QNetworkReply * reply)
{
	//获取响应的信息，状态码为200表示正常
	QVariant status_code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
	//无错误返回
	if (reply->error() == QNetworkReply::NoError) {
		QByteArray bytes = reply->readAll();  //获取字节
		//下载路径（测试先固定路径,后续扩展）
		auto info = musicManager.getPlayingMusicInfo();
		//拼接
		auto destFileName = info.musicName + ".mp3";
		QFile file(destFileName);
		file.open(QIODevice::WriteOnly);
		file.write(bytes);
		file.close();
	}
	ui.pushButton_download->setEnabled(true);
}

void MusicPlayer::mouseMoveEvent(QMouseEvent * event)
{
	//判断左键是否被按下，只有左键按下了，其返回值就是1(true)
	if ((event->buttons() & Qt::LeftButton) && m_bMove)
	{
		move(event->globalPos() - m_point);
	}
	QWidget::mouseMoveEvent(event);
}

void MusicPlayer::mousePressEvent(QMouseEvent * event)
{
	//鼠标事件中包含“按住的是左键”
	if ((event->button() == Qt::LeftButton) && (event->pos().y() < 56) && (event->pos().y() > 0))
	{
		m_bMove = true;
		//获取移动的位移量
		m_point = event->globalPos() - frameGeometry().topLeft();
	}
}

void MusicPlayer::mouseReleaseEvent(QMouseEvent * event)
{
	m_bMove = false;
}
