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
#include "videoWidget.h"
#include "frameless.h"

//����ѭ����ť����
constexpr char* Property_id = "id";
//����˳��
enum E_PlatType
{
	E_SequentialPlay = 1,//˳�򲥷�
	E_RandomPlay = 2,//�������
	E_LoopPlay = 3, //�б���
	E_CurrentItemInLoopPlay = 4, //����ѭ��
};

MusicPlayer::MusicPlayer(QWidget *parent) :
    QWidget(parent), m_filePath(QString()), m_musicTime(QString())
{
    ui.setupUi(this);

	setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::WindowMinimizeButtonHint);
	setAttribute(Qt::WA_TranslucentBackground);

	//���ֲ���ģ��
	initMusicPlayer();

	connect(ui.pushButton_min, &QPushButton::clicked, this, &MusicPlayer::showMinimized);
	connect(ui.pushButton_close, &QPushButton::clicked, this, &MusicPlayer::close);
	/*connect(ui.pushButton_close, &QPushButton::clicked, this, [=] {
		exit(0);
	});*/
	connect(ui.pushButton_max, &QPushButton::clicked, this, &MusicPlayer::sltMaxOrNormal);

	//����slider_progress�������źŲ�(������)
	//ui.slider_progress->setEnabled(false);
	connect(ui.slider_progress,	&CustomSlider::costomSliderClicked,	this,	&MusicPlayer::sltSliderProgressClicked);
	connect(ui.slider_progress,	&CustomSlider::sliderReleased,	this,	&MusicPlayer::sltSliderProgressReleased);
	//���ſؼ�
	connect(ui.pushButton_play, &QPushButton::clicked, this, &MusicPlayer::sltMusicPlayOrPause);
	connect(ui.pushButton_next, &QPushButton::clicked, this, &MusicPlayer::sltNextMusicPlay);
	connect(ui.pushButton_prev, &QPushButton::clicked, this, &MusicPlayer::sltPrevMusicPlay);
	//���ظ���
	connect(ui.pushButton_local, &QPushButton::clicked, this, [=]() {
		ui.stackedWidget->setCurrentWidget(ui.page_localList);
	});
	//�������
	connect(ui.pushButton_open, &QPushButton::clicked, this, &MusicPlayer::sltOpenLocalMusicList);
	//����˳��
	ui.pushButton_cycle->setProperty(Property_id, E_SequentialPlay);
	connect(ui.pushButton_cycle, &QPushButton::clicked, this, &MusicPlayer::sltSetPlayCycle);
	//����������
	connect(ui.pushButton_return, &QPushButton::clicked, this, &MusicPlayer::sltReturnPanel);
	//����
	connect(ui.pushButton_search, &QPushButton::clicked, this, &MusicPlayer::sltSearchMusic);
	//MV
	connect(ui.pushButton_mv, &QPushButton::clicked, this, &MusicPlayer::sltSearchMV);
	//���������б���ĸ�
	connect(&musicManager, &MusicManager::sigMouseDoubleClicked, this, &MusicPlayer::sltMouseDoubleClicked);
	//���ű��صĸ�
	connect(&musicManager, &MusicManager::sigMouseDoubleClicked_local, this, &MusicPlayer::sltPlayListClicked);
	//���粥��ģ��
	m_netWorkMusicPlay = new QNetworkAccessManager(this);
	connect(m_netWorkMusicPlay, &QNetworkAccessManager::finished, this, &MusicPlayer::sltNetWorkMusicPlay, Qt::DirectConnection);
	//��������ģ��
	m_netWorkDownLoad = new QNetworkAccessManager(this);
	connect(m_netWorkDownLoad, &QNetworkAccessManager::finished, this, &MusicPlayer::sltDownLoadByNetWork, Qt::DirectConnection);
	//�����������
	connect(ui.label_image, &MusicImage::sigMouseClicked, this, [=]() {
		ui.page_lrc->setMusicInfo();
		ui.stackedWidget->setCurrentWidget(ui.page_lrc);
	});
	//���ؽӿ�
	connect(ui.pushButton_download, &QPushButton::clicked, this, &MusicPlayer::sltDownLoadButtonClick);
	//mv�鿴
	connect(ui.pushButton_lookMv, &QPushButton::clicked, this, &MusicPlayer::sltLookMv);
	m_netWorkMv = new QNetworkAccessManager(this);
	connect(m_netWorkMv, &QNetworkAccessManager::finished, this, &MusicPlayer::sltNetWorkMvInfo, Qt::DirectConnection);

	//������������ʼ��
	initVolumeSlider();

	//���ģ���ʼ��
	initLrcModel();

	//webģ���ʼ��
	initWebModel();
}

MusicPlayer::~MusicPlayer()
{
}

void MusicPlayer::initMusicPlayer()
{
	//��ʼ������������
	m_musicPlayer = new QMediaPlayer(this);
	//��ʼ���б�
	m_musicPlayList = new QMediaPlaylist(this);
	m_musicPlayList->setPlaybackMode(QMediaPlaylist::Sequential);
	//m_musicPlayList->setPlaybackMode(QMediaPlaylist::Loop);
	m_musicPlayer->setVolume(50);
	m_musicPlayer->setPlaylist(m_musicPlayList);
	//����positionChanged�źŷ���Ƶ�ʣ����뼶��
	m_musicPlayer->setNotifyInterval(500);
	//����ͬ��
	connect(m_musicPlayer, SIGNAL(durationChanged(qint64)), this, SLOT(sltDurationChanged(qint64)));
	connect(m_musicPlayer, SIGNAL(positionChanged(qint64)), this, SLOT(sltPositionChanged(qint64)));
	//�Ż�������
	QFile QSS1(":/qss/image/qss/whiteScrollbar.qss");
	if (QSS1.open(QIODevice::ReadOnly)) {
		QString strStyle = QSS1.readAll();
		ui.listView->verticalScrollBar()->setStyleSheet(strStyle);
	}
	//�б���ʽ
	QFile QSS2(":/qss/image/qss/listViews.qss");
	if (QSS2.open(QIODevice::ReadOnly)) {
		QString strStyle = QSS2.readAll();
		ui.listView->setStyleSheet(strStyle);
	}
	//���ݰ�
	musicManager.getLocalMusicInfoModel().clear();
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
	//����webEngine����
	//��ֹ�Ҽ��˵�
	ui.webEngineView->setContextMenuPolicy(Qt::NoContextMenu);
	//����ҳ��͸��
	ui.webEngineView->page()->setBackgroundColor(Qt::transparent);
	//���ػ�����
	ui.webEngineView->page()->settings()->setAttribute(QWebEngineSettings::ShowScrollBars, false);
	//д�뱾��html
	//��html�ļ��ж�ȡ���ݺ�д��webview
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
					//����
					QString url = musicManager.getJsonData(valuedataObject, "play_url");
					m_musicPlayList->addMedia(QUrl::fromLocalFile(url));
					m_musicPlayList->setCurrentIndex(m_musicPlayList->mediaCount() - 1);
					m_musicPlayer->play();
					//��Ƶ������ʾ
					QString name = musicManager.getJsonData(valuedataObject, "audio_name");
					ui.label_name->setText(name);
					//����,���֣�ר��
					MusicInfo info;
					info.musicName = musicManager.getJsonData(valuedataObject, "song_name");
					info.musicPlayer = musicManager.getJsonData(valuedataObject, "author_name");
					info.musicAlbum = musicManager.getJsonData(valuedataObject, "album_name");
					//�Ƿ���ʾmv
					info.hash = musicManager.getJsonData(valuedataObject, "hash");
					info.mvHash = musicManager.getMusicInfoModel().getMvHash(info.hash);
					ui.pushButton_lookMv->setVisible(!info.mvHash.isEmpty());
					info.url = url;
					musicManager.savePlayingMusicInfo(info);	
					//���ò���/��ͣ��ť���������ı�����Ϊ����ͣ��
					ui.pushButton_play->setChecked(true);
					//��ʻ�ȡ
					musicManager.getLrcJsonData(valuedataObject, "lyrics");
					//ͼƬ��ʾ
					auto imageUrl = musicManager.getJsonData(valuedataObject, "img");
					ui.label_image->requestImage(imageUrl);
				}
				else
				{
					qDebug() << "����";
				}
			}
		}
	}
}

void MusicPlayer::parseJsonMvInfo(const QString & json)
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
								//������
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
		QStringLiteral("ѡ����Ƶ�ļ�"),
		".",
		QStringLiteral("��Ƶ�ļ�(*.mp3 *.wav *.wma)mp3�ļ�(*.mp3);;wav�ļ�(*.wav);;wma�ļ�(*.wma);;�����ļ�(*.*)"));

	if (fileList.isEmpty()) return;
	for (auto & each : fileList) {
		QFileInfo fileInfo(each);
		if (fileInfo.exists()) {
			//��ӵ��б�
			m_musicPlayList->addMedia(QUrl::fromLocalFile(each));
			MusicInfo info;
			info.musicName = fileInfo.fileName();
			m_filePath = each.replace(info.musicName, "");
			musicManager.getLocalMusicInfoModel().addInfoToModel(info);
		}
	}
	////��ֹͣ���������
	//m_musicPlayer->stop();
	////�����б�ģ��
	//m_musicPlayList->clear();
}

void MusicPlayer::sltPlayListClicked(int row)
{
    m_musicPlayList->setCurrentIndex(row);
    m_musicPlayer->play();

    //���ò���/��ͣ��ť���������ı�����Ϊ����ͣ��
	ui.pushButton_play->setChecked(true);
	//����
	auto name = m_musicPlayList->currentMedia().canonicalUrl().fileName();
	ui.label_name->setText(name);
	//�����Ϣ
	QString path = m_filePath + name.split(".").front() + ".lrc";
	musicManager.getLoaclLrcData(path);
}

void MusicPlayer::sltDurationChanged(qint64 duration)
{
	//���ý�����
	ui.slider_progress->setMaximum(duration);
	//��ʱ��
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
	//���У��
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
		//��ǰ˳�򲥷ţ��л����������
		ui.pushButton_cycle->setProperty(Property_id, E_RandomPlay);
		m_musicPlayList->setPlaybackMode(QMediaPlaylist::Random);
		ui.pushButton_cycle->setStyleSheet("border-image: url(:/musicPlayer/image/musicPlayer/suiji.png);");
		break;
	}
	case E_RandomPlay: {
		//��ǰ������ţ��л����б�ѭ��
		ui.pushButton_cycle->setProperty(Property_id, E_LoopPlay);
		m_musicPlayList->setPlaybackMode(QMediaPlaylist::Loop);
		ui.pushButton_cycle->setStyleSheet("border-image: url(:/musicPlayer/image/musicPlayer/danquxunhuan.png);");
		break;
	}
	case E_LoopPlay: {
		//��ǰ�б�ѭ�����ţ��л�������ѭ��
		ui.pushButton_cycle->setProperty(Property_id, E_CurrentItemInLoopPlay);
		m_musicPlayList->setPlaybackMode(QMediaPlaylist::CurrentItemInLoop);
		ui.pushButton_cycle->setStyleSheet("border-image: url(:/musicPlayer/image/musicPlayer/danqu.png);");
		break;
	}
	case E_CurrentItemInLoopPlay:{
		//��ǰ����ѭ�����ţ��л���˳�򲥷�
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

void MusicPlayer::sltSearchMV()
{
	ui.stackedWidget->setCurrentWidget(ui.page_mv);
}

void MusicPlayer::sltMouseDoubleClicked(int row)
{
	auto hash = musicManager.getMusicInfoModel().getHash(row);
	auto albumId = musicManager.getMusicInfoModel().getAlbumId(row);
	QString url = QString("http://www.kugou.com/yy/index.php?r=play/getdata"
		"&hash=%1&album_id=%2&_=1497972864535").arg(hash).arg(albumId);
	//������������
	QNetworkRequest request;
	request.setUrl(QUrl(url));
	//����ͷ�޷��õ�json��������Ϊ�˷�ֹ������ȡ
	request.setRawHeader("Cookie", "kg_mid=2333");
	request.setHeader(QNetworkRequest::CookieHeader, 2333);
	m_netWorkMusicPlay->get(request);
}

void MusicPlayer::sltNetWorkMusicPlay(QNetworkReply * reply)
{
	//��ȡ��Ӧ����Ϣ��״̬��Ϊ200��ʾ����
	QVariant status_code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
	//�޴��󷵻�
	if (reply->error() == QNetworkReply::NoError){
		QByteArray bytes = reply->readAll();  //��ȡ�ֽ�
		QString result(bytes);  //ת��Ϊ�ַ���
		parseJsonSongInfo(result);
	}
}

void MusicPlayer::sltDownLoadButtonClick()
{
	ui.pushButton_download->setEnabled(false);
	QNetworkRequest request;
	//������������
	auto info = musicManager.getPlayingMusicInfo();
	request.setUrl(QUrl(info.url));
	request.setHeader(QNetworkRequest::UserAgentHeader, "RT-Thread ART");
	m_netWorkDownLoad->get(request);
}

void MusicPlayer::sltDownLoadByNetWork(QNetworkReply * reply)
{
	//��ȡ��Ӧ����Ϣ��״̬��Ϊ200��ʾ����
	QVariant status_code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
	//�޴��󷵻�
	if (reply->error() == QNetworkReply::NoError) {
		QByteArray bytes = reply->readAll();  //��ȡ�ֽ�
		//����·���������ȹ̶�·��,������չ��
		auto info = musicManager.getPlayingMusicInfo();
		//ƴ��
		auto destFileName = "Music/" + info.musicName + ".mp3";
		QFileInfo fileinfo(destFileName);
		if (!fileinfo.exists()) {
			QDir().mkpath(fileinfo.absolutePath());
		}
		QFile file(destFileName);
		if (!file.open(QFile::WriteOnly)) {
			return;
		}
		file.write(bytes);
		file.close();
	}
	ui.pushButton_download->setEnabled(true);
}

void MusicPlayer::sltLookMv()
{
	if (m_musicPlayer->state() == QMediaPlayer::PlayingState) {
		m_musicPlayer->pause();
	}
	ui.pushButton_lookMv->setEnabled(false);
	auto info = musicManager.getPlayingMusicInfo();
	QString url = QString("http://m.kugou.com/app/i/mv.php"
		"?cmd=100&hash=%1&ismp3=1&ext=mp4").arg(info.mvHash);
	QNetworkRequest request;
	//������������
	request.setUrl(QUrl(url));
	request.setHeader(QNetworkRequest::UserAgentHeader, "RT-Thread ART");
	m_netWorkMv->get(request);
}

void MusicPlayer::sltNetWorkMvInfo(QNetworkReply * reply)
{
	//��ȡ��Ӧ����Ϣ��״̬��Ϊ200��ʾ����
	QVariant status_code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
	//�޴��󷵻�
	if (reply->error() == QNetworkReply::NoError) {
		QByteArray bytes = reply->readAll();  //��ȡ�ֽ�
		QString result(bytes);                //ת��Ϊ�ַ���
		parseJsonMvInfo(result);
	}
	ui.pushButton_lookMv->setEnabled(true);
}

void MusicPlayer::mouseMoveEvent(QMouseEvent * event)
{
	//�ж�����Ƿ񱻰��£�ֻ����������ˣ��䷵��ֵ����1(true)
	if ((event->buttons() & Qt::LeftButton) && m_bMove)
	{
		move(event->globalPos() - m_point);
	}
	QWidget::mouseMoveEvent(event);
}

void MusicPlayer::mousePressEvent(QMouseEvent * event)
{
	//����¼��а�������ס���������
	if ((event->button() == Qt::LeftButton) && (event->pos().y() < 56) && (event->pos().y() > 0))
	{
		m_bMove = true;
		//��ȡ�ƶ���λ����
		m_point = event->globalPos() - frameGeometry().topLeft();
	}
}

void MusicPlayer::mouseReleaseEvent(QMouseEvent * event)
{
	m_bMove = false;
}
