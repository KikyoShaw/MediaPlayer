#include "mainwidget.h"
#include <QMouseEvent>
#include <QMovie>
#include <QMediaPlayer>
#include "mediaplayer.h"
#include "musicplayer.h"

MainWidget::MainWidget(QWidget *parent) :
    QWidget(parent)
{
    ui.setupUi(this);

    //ȥ���߿�
    setWindowFlags(Qt::FramelessWindowHint);

    //���ö�̬����
	m_bgMovie = new QMovie(this);
	m_bgMovie->setFileName(":/new/image/125.gif");
    ui.label->setMovie(m_bgMovie);
	m_bgMovie->start();
    ui.label->setScaledContents(true);

    //���ñ�������
	m_bgPlayer = new QMediaPlayer(this);
	m_bgPlayer->setMedia(QUrl::fromLocalFile("./star.mp3"));
	m_bgPlayer->setVolume(100);
	m_bgPlayer->play();
}

MainWidget::~MainWidget()
{
}

//��Ƶ������
void MainWidget::on_vedio_clicked()
{
	m_videoPlayer = QSharedPointer<MediaPlayer>(new MediaPlayer());
	if (m_videoPlayer) {
		connect(m_videoPlayer.data(), SIGNAL(mySignal()), this, SLOT(show()));
		connect(m_videoPlayer.data(), SIGNAL(mySignal()), this, SLOT(my_player()));
		m_videoPlayer->show();
	}
	m_bgPlayer->stop();
    close();
}

//���ֲ�����
void MainWidget::on_music_clicked()
{
	m_musicPlayer = QSharedPointer<MusicPlayer>(new MusicPlayer());
	if (m_musicPlayer) {
		connect(m_musicPlayer.data(), SIGNAL(mySignal()), this, SLOT(show()));
		connect(m_musicPlayer.data(), SIGNAL(mySignal()), this, SLOT(my_player()));
		m_musicPlayer->show();
	}
	m_bgPlayer->stop();
    close();
}

//�˳�
void MainWidget::on_exit_clicked()
{
	m_bgPlayer->stop();
	close();
}

//������һ��
void MainWidget::on_help_clicked()
{
	m_bgPlayer->stop();
    close();
    emit MySig();
}

void MainWidget::my_player()
{
	m_bgPlayer->setVolume(100);
	m_bgPlayer->play();
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


















