#include "mainwidget.h"
#include <QMouseEvent>
#include <QMovie>
#include <QMediaPlayer>
#include "videoPlayer.h"
#include "musicplayer.h"
#include "gifProxyStyle.h"
#include <QDesktopServices>

MainWidget::MainWidget(QWidget *parent) :
    QWidget(parent)
{
    ui.setupUi(this);

    //ȥ���߿�
    setWindowFlags(Qt::FramelessWindowHint);
	setAttribute(Qt::WA_TranslucentBackground);

	//����gif���ڵ�label������
	auto size = ui.label->size();
	ui.label->setStyle(new GifProxyStyle(size, 6));
    //���ö�̬����
	m_bgMovie = new QMovie(this);
	m_bgMovie->setFileName(":/new/image/1.gif");
	ui.label->setMovie(m_bgMovie);
	m_bgMovie->start();
	//ui.label->setScaledContents(true);

    //���ñ�������
	m_bgPlayer = new QMediaPlayer(this);
	m_bgPlayer->setMedia(QUrl::fromLocalFile("Music/kikyo.mp3"));
	m_bgPlayer->setVolume(100);
	m_bgPlayer->play();

	//��ʼ�������ı�
	m_blogLabel = new QLabel(this);
	if (m_blogLabel) {
		m_blogLabel->setAlignment(Qt::AlignCenter);
		m_blogLabel->setFixedSize(width(), 30);
		m_blogLabel->move(0, (height() - m_blogLabel->height())/2 + 60);
		m_blogLabel->setStyleSheet("font: 14px Microsoft YaHei;");
		QString strText = QStringLiteral("CSDN: <a style='color:#01EEC3;text-decoration:none;'href = 'https://blog.csdn.net/qq_36651243'>%1</a>   GitHub: "
			"<a style='color:#01EEC3;text-decoration:none;'href = 'https://github.com/KikyoShaw'>%1</a>").arg(QStringLiteral("�������"));
		m_blogLabel->setText(strText);
		//�����Ӳ鿴webЭ��
		connect(m_blogLabel, &QLabel::linkActivated, this, &MainWidget::sltOpenWeb);
		m_blogLabel->setVisible(false);
	}
}

MainWidget::~MainWidget()
{
}

//��Ƶ������
void MainWidget::on_vedio_clicked()
{
	m_videoPlayer = QSharedPointer<VideoPlayer>(new VideoPlayer());
	if (m_videoPlayer) {
		connect(m_videoPlayer.data(), SIGNAL(mySignal()), this, SLOT(show()));
		connect(m_videoPlayer.data(), SIGNAL(mySignal()), this, SLOT(my_player()));
		m_videoPlayer->show();
	}
	if (m_bgPlayer->state() != QMediaPlayer::StoppedState) {
		m_bgPlayer->stop();
	}
    close();
}

//���ֲ�����
void MainWidget::on_music_clicked()
{
	m_musicPlayer = QSharedPointer<MusicPlayer>(new MusicPlayer());
	if (m_musicPlayer) {
		connect(m_musicPlayer.data(), SIGNAL(sigReturnPanel()), this, SLOT(my_player()));
		m_musicPlayer->show();
	}
	if (m_bgPlayer->state() != QMediaPlayer::StoppedState) {
		m_bgPlayer->stop();
	}
    close();
}

//�˳�
void MainWidget::on_exit_clicked()
{
	if (m_bgPlayer->state() != QMediaPlayer::StoppedState) {
		m_bgPlayer->stop();
	}
	close();
}

//������һ��
void MainWidget::on_help_clicked()
{
	if (m_blogLabel) {
		auto isVisible = m_blogLabel->isVisible();
		m_blogLabel->setVisible(!isVisible);
	}
}

void MainWidget::my_player()
{
	m_bgPlayer->setVolume(100);
	m_bgPlayer->play();
	show();
}

void MainWidget::sltOpenWeb(const QString & text)
{
	QUrl url = text;
	QDesktopServices::openUrl(url);
}

void MainWidget::mouseMoveEvent(QMouseEvent * event)
{
	//�ж�����Ƿ񱻰��£�ֻ����������ˣ��䷵��ֵ����1(true)
	if ((event->buttons() & Qt::LeftButton) && m_bMove)
	{
		move(event->globalPos() - m_point);
	}
	QWidget::mouseMoveEvent(event);
}

void MainWidget::mousePressEvent(QMouseEvent * event)
{
	if (event->button() == Qt::LeftButton)
	{
		m_bMove = true;
		m_point = event->globalPos() - frameGeometry().topLeft();
	}
	QWidget::mousePressEvent(event);
}

void MainWidget::mouseReleaseEvent(QMouseEvent * event)
{
	m_bMove = false;
	QWidget::mouseReleaseEvent(event);
}


















