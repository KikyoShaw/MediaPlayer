#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include <QSharedPointer>
#include "ui_mainwidget.h"

class QMovie;
class QMediaPlayer;
class VideoPlayer;
class MusicPlayer;

class MainWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MainWidget(QWidget *parent = 0);
    ~MainWidget();

private slots:
    void on_vedio_clicked();
    void on_music_clicked();
    void on_exit_clicked();
    void on_help_clicked();
    void my_player();
	void sltOpenWeb(const QString &text);

signals:
    void MySig(); //������һ�����ź�

protected:
	virtual void mouseMoveEvent(QMouseEvent *event);
	virtual void mousePressEvent(QMouseEvent *event);
	virtual void mouseReleaseEvent(QMouseEvent *event);

private:
    Ui::MainWidget ui;
	//�����ƶ�����ֵ
	QPoint m_point;
	volatile bool m_bMove = false;
	//��̬����
    QMovie *m_bgMovie = nullptr;
	//������Ч
    QMediaPlayer *m_bgPlayer = nullptr;
	//��Ƶ������
	QSharedPointer<VideoPlayer> m_videoPlayer = nullptr;
	//���ֲ�����
	QSharedPointer <MusicPlayer> m_musicPlayer = nullptr; 
	//����
	QLabel *m_blogLabel = nullptr;
};

#endif // MAINWIDGET_H
