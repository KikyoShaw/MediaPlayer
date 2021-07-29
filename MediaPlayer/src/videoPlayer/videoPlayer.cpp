#include "videoPlayer.h"
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QVideoWidget>
#include <QMouseEvent>
#include <QTime>
#include <QFileDialog>
#include <QScrollBar>

//����״̬��trueΪ���ţ�falseΪ��ͣ
bool play_state;

//��Slider�йصĲ��ſ��Ʊ���
QTimer * timer;
int maxValue = 1000;//���ý����������ֵ

//��������slider
CustomSlider * slider_volume;

VideoPlayer::VideoPlayer(QWidget *parent) :
	QWidget(parent)
{
    ui.setupUi(this);

	setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::WindowMinimizeButtonHint);
	//setAttribute(Qt::WA_TranslucentBackground);

	//��ʼ��������ģ��
	initVideoPlayModel();

	//�Ż�������
	QFile QSS1(":/qss/image/qss/whiteScrollbar.qss");
	if (QSS1.open(QIODevice::ReadOnly)) {
		QString strStyle = QSS1.readAll();
		ui.listWidget_localList->verticalScrollBar()->setStyleSheet(strStyle);
	}

	//�б���ʽ
	QFile QSS2(":/qss/image/qss/listWidget.qss");
	if (QSS2.open(QIODevice::ReadOnly)) {
		QString strStyle = QSS2.readAll();
		ui.listWidget_localList->setStyleSheet(strStyle);
	}

	connect(ui.pushButton_min, &QPushButton::clicked, this, &VideoPlayer::showMinimized);
	connect(ui.pushButton_close, &QPushButton::clicked, this, &VideoPlayer::close);
	connect(ui.pushButton_max, &QPushButton::clicked, this, &VideoPlayer::sltMaxOrNormal);

	//�򿪱����ļ�
	connect(ui.pushButton_open, &QPushButton::clicked, this, &VideoPlayer::sltOpenLocalVideoList);
	//�б����¼�
	connect(ui.listWidget_localList, &QListWidget::itemDoubleClicked, this, &VideoPlayer::sltListWidgetDoubleClicked);

  //  //���ô���ͼ��
  //  //QIcon icon;
  //  //icon.addFile(QStringLiteral(":/new/image/MediaPlayer.ico"), QSize(), QIcon::Normal, QIcon::Off);
  //  //this->setWindowIcon(icon);
  //  //���ر߿�
  ////  this->setWindowFlags(Qt::FramelessWindowHint);
  //  //ui.pushButton_play_and_pause->setEnabled(false);
  //  //ui.pushButton_volume->setEnabled(false);
  //  ////����slider_progress�������źŲ�
  //  //ui.slider_progress->setEnabled(false);
  //  //connect(ui.slider_progress,&CustomSlider::costomSliderClicked,this,&MediaPlayer::slider_progress_clicked);
  //  //connect(ui.slider_progress,&CustomSlider::sliderMoved,this,&MediaPlayer::slider_progress_moved);
  //  //connect(ui.slider_progress,&CustomSlider::sliderReleased,this,&MediaPlayer::slider_progress_released);
  //  //�ֶ�����slider_volume ������ʼ�� ���� ���ã��Լ��ۺ���
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
  //  ////���ڲ��漰��sliderֵ��ˢ�£����ֻ���move���Զ���click�����źŽ��д������ҿ��Թ���һ���ۺ���
  //  //connect(slider_volume,&CustomSlider::costomSliderClicked,this,&MediaPlayer::slider_volume_changed);
  //  //connect(slider_volume,&CustomSlider::sliderMoved,this,&MediaPlayer::slider_volume_changed);
  //  //connect(slider_volume,&CustomSlider::sliderReleased,this,&MediaPlayer::slider_released);


  //  //ʵ��������ȫ�ֱ���
  //  layout_video = new QVBoxLayout;
}

int VideoPlayer::play_flag = 0;

////���Ž��ȿ���Slider�Ĳۺ���
//void MediaPlayer::slider_progress_clicked()
//{
//	m_videoPlayer->setPosition(ui.slider_progress->value()*m_videoPlayer->duration()/maxValue);
//}
//
//void MediaPlayer::slider_progress_moved()
//{
//    //��ʱֹͣ��ʱ�������û��϶������в��޸�slider��ֵ
//    timer->stop();
//	m_videoPlayer->setPosition(ui.slider_progress->value()*m_videoPlayer->duration()/maxValue);
//}
//
//void MediaPlayer::slider_progress_released()
//{
//    //�û��ͷŻ����������ʱ��
//    timer->start();
//}
//
////��������Slider�Ĳۺ���
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
	//��ʼ������������
	m_videoPlayer = new QMediaPlayer(this);
	//��ʼ���б�
	m_listInfo.clear();
	m_videoPlayList = new QMediaPlaylist(this);
	m_videoPlayList->setPlaybackMode(QMediaPlaylist::Sequential);
	m_videoPlayer->setVolume(50);
	m_videoPlayer->setPlaylist(m_videoPlayList);
	//��ʼ�����Ŵ���
	m_videoPlayWidget = new QVideoWidget();
	m_videoPlayWidget->setStyleSheet("border:none;background-color:transparent;");
	layout_video = new QVBoxLayout();
	layout_video->setMargin(0);
	m_videoPlayWidget->resize(ui.label_player->size());
	//m_videoPlayWidget->setAspectRatioMode(Qt::IgnoreAspectRatio);
	layout_video->addWidget(m_videoPlayWidget);
	ui.label_player->setLayout(layout_video);
	m_videoPlayer->setVideoOutput(m_videoPlayWidget);
	//����ͬ��
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
		QStringLiteral("ѡ����Ƶ�ļ�"),
		".",
		QStringLiteral("��Ƶ�ļ�(*.avi *.mp4 *.flv *.mkv);��Ƶ�ļ�(*.mp3 *.wav *.wma)"));

	if (fileList.isEmpty()) return;
	ui.listWidget_localList->clear();
	//�洢����ʵ��
	for (auto &each : fileList) {
		QFileInfo fileInfo(each);
		if (fileInfo.exists()) {
			m_videoPlayList->addMedia(QUrl::fromLocalFile(each));
			auto videoName = fileInfo.fileName();
			m_listInfo.append(videoName);
		}
	}
	//��������ʵ��������б�
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
	//���ý�����
	//ui.slider_progress->setMaximum(duration);
	//��ʱ��
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
//    //����label_player�ߴ����ò�������
//	m_videoPlayWidget->resize(ui.label_player->size());
//    layout_video->addWidget(m_videoPlayWidget);
//    ui.label_player->setLayout(layout_video);
//	m_videoPlayer->setVideoOutput(m_videoPlayWidget);
//
//    //ѡ����Ƶ�ļ�
//    QString filename = QFileDialog::getOpenFileName(this,tr("ѡ����Ƶ�ļ�"),".",tr("��Ƶ��ʽ(*.avi *.mp4 *.flv *.mkv)"));
//    QFile file(filename);
//    if(!file.open(QIODevice::ReadOnly))
//    {
//        QMessageBox::information(NULL, "��ʾ", "������ѡ����Ƶ�ļ�", QMessageBox::Ok, QMessageBox::Ok);
//        return;
//    }
//
//    //���ò�����
//   // m_videoPlayer->setMedia(QUrl::fromLocalFile(filename));
//	m_videoPlayList->addMedia(QUrl("http://fs.mv.web.kugou.com/202107270955/5018e19c140d5adf019a877ae12f31b2/G031/M08/12/1D/_5MEAFXd0KSAZMLpARSo2wZan7k579.mp4"));
//	//m_videoPlayer->setMedia(QUrl::fromLocalFile("http://fs.mv.web.kugou.com/202107270955/5018e19c140d5adf019a877ae12f31b2/G031/M08/12/1D/_5MEAFXd0KSAZMLpARSo2wZan7k579.mp4"));
//    //play_stateΪtrue��ʾ���ţ�false��ʾ��ͣ
//    play_state = true;
//    //���ò���/��ͣ��ť���������ı�����Ϊ����ͣ��
//    ui.pushButton_play_and_pause->setEnabled(true);
//    ui.pushButton_play_and_pause->setText("��ͣ");
//    //����������
//	m_videoPlayer->play();
//    play_flag=1;
//
//    //����slider�����÷�Χ
//    ui.slider_progress->setEnabled(true);
//    ui.slider_progress->setRange(0,maxValue);
//    timer->start();
//
//    //������������slider
//    slider_volume->setEnabled(true);
//    slider_volume->setRange(0, 100);
//    //����������ť
//    ui.pushButton_volume->setEnabled(true);
//}

//void MediaPlayer::on_pushButton_play_and_pause_clicked()
//{
//    //��ת����״̬
//    if(play_state)
//    {
//		m_videoPlayer->pause();
//        ui.pushButton_play_and_pause->setText("����");
//    }
//    else
//    {
//		m_videoPlayer->play();
//        ui.pushButton_play_and_pause->setText("��ͣ");
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
//    //ͨ��hide()��show()������ʵ����������Slider�Ļ���������
//    if(state_slider_volume)
//    {
//        slider_volume->hide();
//    }
//    else
//    {
//        slider_volume->setValue(m_videoPlayer->volume());
//        //����λ�ã�ʹ��λ���������ư�ť���Ϸ�
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
	//�ж�����Ƿ񱻰��£�ֻ����������ˣ��䷵��ֵ����1(true)
	if ((event->buttons() & Qt::LeftButton) && m_bMove)
	{
		move(event->globalPos() - m_point);
	}
	QWidget::mouseMoveEvent(event);
}

void VideoPlayer::mousePressEvent(QMouseEvent * event)
{
	//����¼��а�������ס���������
	if ((event->button() == Qt::LeftButton) && (event->pos().y() < 56) && (event->pos().y() > 0))
	{
		m_bMove = true;
		//��ȡ�ƶ���λ����
		m_point = event->globalPos() - frameGeometry().topLeft();
	}
}

void VideoPlayer::mouseReleaseEvent(QMouseEvent * event)
{
	m_bMove = false;
}