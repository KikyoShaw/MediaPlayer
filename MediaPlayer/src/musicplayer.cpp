#include "musicplayer.h"
#include <QMouseEvent>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QStandardItemModel>
#include <QFileDialog>
#include <QTimer>
#include <QScrollBar>
#include "vsliderwidget.h"
#include "LrcWidget.h"
#include <QWebEngineSettings>

//���ý����������ֵ
#define MAXVALUE 1000
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
	connect(ui.pushButton_max, &QPushButton::clicked, this, &MusicPlayer::sltMaxOrNormal);

	//����slider_progress�������źŲ�(������)
	ui.slider_progress->setEnabled(false);
	connect(ui.slider_progress,	&CustomSlider::costomSliderClicked,	this,	&MusicPlayer::sltSliderProgressClicked);
	connect(ui.slider_progress,	&CustomSlider::sliderMoved,	this,	&MusicPlayer::sltSliderProgressMoved);
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
	//����ͬ��
	connect(m_musicPlayer, SIGNAL(durationChanged(qint64)), this, SLOT(sltDurationChanged(qint64)));
	connect(m_musicPlayer, SIGNAL(positionChanged(qint64)), this, SLOT(sltPositionChanged(qint64)));
	//����˳��
	ui.pushButton_cycle->setProperty(Property_id, E_SequentialPlay);
	connect(ui.pushButton_cycle, &QPushButton::clicked, this, &MusicPlayer::sltSetPlayCycle);

	//��ʼ����ʱ��
	m_progressTimer = new QTimer(this);
	m_progressTimer->setInterval(1000);
	//��timer������onTimerOut�ۺ���
	connect(m_progressTimer, SIGNAL(timeout()), this, SLOT(sltTimerOut()));

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
	m_listItemModel = new QStandardItemModel(this);
	//�Ż������б���ʾ
	QHeaderView *verticalHeader = ui.localList->verticalHeader();
	verticalHeader->setSectionResizeMode(QHeaderView::Fixed);
	verticalHeader->setDefaultSectionSize(60);
	//�Ż�������
	QFile QSS1(":/qss/image/qss/whiteScrollbar.qss");
	if (QSS1.open(QIODevice::ReadOnly)) {
		QString strStyle = QSS1.readAll();
		ui.localList->verticalScrollBar()->setStyleSheet(strStyle);
	}
	//���ݰ�
	ui.localList->setSelectionMode(QAbstractItemView::SingleSelection);
	ui.localList->setEditTriggers(QAbstractItemView::NoEditTriggers);
	ui.localList->setSelectionBehavior(QAbstractItemView::SelectRows);
	connect(ui.localList, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(sltPlayListClicked(QModelIndex)));
}

void MusicPlayer::initLrcModel()
{
	m_lrcWidget = QSharedPointer<LrcWidget>(new LrcWidget());
	connect(ui.pushButton_lrc, &QPushButton::clicked, this, &MusicPlayer::sltShowLrcModel);
}

void MusicPlayer::initVolumeSlider()
{
	m_volumeSlider = new VSliderWidget(this);
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

void MusicPlayer::sltMaxOrNormal()
{
	if (isMaximized())
		showNormal();
	else
		showMaximized();
}

void MusicPlayer::sltSliderProgressClicked()
{
	m_musicPlayer->setPosition(ui.slider_progress->value()*m_musicPlayer->duration() / MAXVALUE);
}

void MusicPlayer::sltSliderProgressMoved()
{
	//��ʱֹͣ��ʱ�������û��϶������в��޸�slider��ֵ
    m_progressTimer->stop();
    m_musicPlayer->setPosition(ui.slider_progress->value()*m_musicPlayer->duration() /  MAXVALUE);

}

void MusicPlayer::sltSliderProgressReleased()
{
	//�û��ͷŻ����������ʱ��
	m_progressTimer->start();
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
	//��ֹͣ���������
	m_musicPlayer->stop();
	m_progressTimer->stop();
	//�����б�ģ��
	m_musicPlayList->clear();
	//�赥ģ��
    m_listItemModel->clear();
	m_listItemModel->setHorizontalHeaderItem(0, new QStandardItem(QStringLiteral("�����б�")));
	ui.localList->setModel(m_listItemModel);
	ui.localList->setColumnWidth(0, ui.page_localList->width());
	ui.localList->verticalHeader()->hide();

    auto path = QFileDialog::getOpenFileNames(this, QStringLiteral("ѡ�����"), ".", "(*.mp3)");
    int i=0;
    QString addSongs = QString();
    QListIterator<QString>mylist(path);
    while(mylist.hasNext())
    {
        addSongs = mylist.next();
        QFileInfo file;
        file = QFileInfo(addSongs);
        QString fileName = file.fileName();
        m_listItemModel->setItem(i,0,new QStandardItem(fileName));
		m_musicPlayList->addMedia(QUrl::fromLocalFile(addSongs));
        i++;
    }
	m_musicPlayList->setPlaybackMode(QMediaPlaylist::Loop);
    m_musicPlayer->setPlaylist(m_musicPlayList);
    QString PathString = QString();
    for(int j = 0; j < path.size(); ++j){
        PathString = QDir::toNativeSeparators(path.at(j)); //��б��ת���ɷ�б��
        if(!PathString.isEmpty()){
            QString file_name = PathString.split("\\").last();//��б�ֿܷ���ȡ���һ������
        }
    }
    if(PathString.contains("\\")) {
      auto tmpPath = PathString.replace("\\","/",Qt::CaseInsensitive);
      auto list = tmpPath.split("/");
      for(int i = 0;i < list.size();i++){
          if(list.value(i) == tmpPath.split("/").last()){
              for(int j = 0; j < i; j++){
				  m_filePath = m_filePath + list.at(j) + "/";
                }
                return;
          }
        }
    }
}

void MusicPlayer::sltPlayListClicked(QModelIndex index)
{
	//����slider�����÷�Χ
    ui.slider_progress->setEnabled(true);
    ui.slider_progress->setRange(0, MAXVALUE);
	m_progressTimer->start();
    m_musicPlayList->setCurrentIndex(index.row());
    m_musicPlayer->play();

	////Ԫ���ݵĽ�����Ҫʱ�䣬����������Ҫѭ���ȴ�����ͬʱ��Ҫ����Qt�¼�������������У�
	//while (!m_musicPlayer->isMetaDataAvailable()) {
	//	QCoreApplication::processEvents();
	//}
	//QStringList list = m_musicPlayer->availableMetaData();//����ʱ�鿴����ЩԪ���ݿ���
	//if (m_musicPlayer->isMetaDataAvailable()) {
	//	//������Ϣ
	//	QString author = m_musicPlayer->metaData(QStringLiteral("Author")).toStringList().join(",");
	//	QString title = m_musicPlayer->metaData(QStringLiteral("Title")).toString();
	//	QString albumTitle = m_musicPlayer->metaData(QStringLiteral("AlbumTitle")).toString();
	//	int audioBitRate = m_musicPlayer->metaData(QStringLiteral("AudioBitRate")).toInt();
	//	qint64 duration = m_musicPlayer->duration();
	//}

    //���ò���/��ͣ��ť���������ı�����Ϊ����ͣ��
	ui.pushButton_play->setChecked(true);

	//��������
	m_volumeSlider->setVoiceValue(m_musicPlayer->volume());
}

void MusicPlayer::sltTimerOut()
{
	ui.slider_progress->setValue(m_musicPlayer->position()*MAXVALUE / m_musicPlayer->duration());
}

void MusicPlayer::sltDurationChanged(qint64 duration)
{
	ui.label_name->setText(m_musicPlayList->currentMedia().canonicalUrl().fileName());
	//��ʱ��
	int sec_all = (m_musicPlayer->duration()) / 60000;
	int minute_all = (m_musicPlayer->duration()) % 60000 / 1000;
	if (minute_all < 10 && minute_all > -0){
		m_musicTime = "0" + QString::number(sec_all) + ":" + "0" + QString::number(minute_all);
	}
	else if(minute_all >= 10){
		m_musicTime = "0" + QString::number(sec_all) + ":" + QString::number(minute_all);
	}
}

void MusicPlayer::sltPositionChanged(qint64 position)
{
	//��ǰʱ��
	int sec_now = position / 60000;
    int minute_now = position % 60000 / 1000;
	//���У��
    QFile file(m_filePath + m_musicPlayer->currentMedia().canonicalUrl().fileName().split(".").front() + ".lrc");
    //qDebug() << ss +playList->currentMedia().canonicalUrl().fileName().split(".").front() + ".lrc";
	if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
		QTextStream stream(&file);
		QString line;
		while (!stream.atEnd()) {
			line = stream.readLine();
			// qDebug() << line;
			if (line.mid(1, 2).toInt() == (position / 60000) && (line.mid(4, 2)).toInt() == (position % 60000 / 1000)) {
				m_lrcWidget->setLrc(line.split("]").last());
			}
			file.close();
		}
	}
	 else{
		m_lrcWidget->setLrc(QStringLiteral("��ʱû���ҵ����"));
     }

    if(minute_now < 10 && minute_now > -0)
     {
		auto nowTime = "0" + QString::number(sec_now) + ":" + "0" + QString::number(minute_now);
        ui.label_volumNum->setText(QString("%1/%2").arg(nowTime).arg(m_musicTime));
     }
	else if(minute_now >= 10){
		auto nowTime = "0" + QString::number(sec_now) + ":" + QString::number(minute_now);
		ui.label_volumNum->setText(QString("%1/%2").arg(nowTime).arg(m_musicTime));
	}
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
