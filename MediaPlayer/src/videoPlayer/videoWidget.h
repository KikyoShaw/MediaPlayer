#pragma once

#include <QWidget>
#include "ui_videoWidget.h"

class QMediaPlayer;
class QMediaPlaylist;
class QVideoWidget;
class QVBoxLayout;
class QNetworkAccessManager;
class QNetworkReply;
class VideoControls;
class VideoControlTop;

struct MvInfoPlayer
{
	QString hash; //mv��ϣ
	QString mvName; //mv����
	QString url; //����
	QString path;
};

class VideoWidget : public QWidget
{
    Q_OBJECT

public:
    explicit VideoWidget(QWidget *parent = 0);
    ~VideoWidget();

	void setVideoPlay(const QString &filePath);

	void setMvPlay(MvInfoPlayer info);

private:
	void initVideoControls();
	void locateWidgets();

private slots:
	void sltNetWorkMvDownLoad(QNetworkReply *reply);
	void sltVideoPlayOrPause(bool state);
	void sltSoundVoiceValue(int value);
	void sltSetPosition(int value);
	void sltDurationChanged(qint64 duration);
	void sltPositionChanged(qint64 position);

protected:
	virtual void mouseMoveEvent(QMouseEvent *event);
	virtual void mousePressEvent(QMouseEvent *event);
	virtual void mouseReleaseEvent(QMouseEvent *event);
	virtual void showEvent(QShowEvent *event);
	virtual void closeEvent(QCloseEvent *event);
	virtual void moveEvent(QMoveEvent *event);
	virtual void hideEvent(QHideEvent *event);
	virtual void resizeEvent(QResizeEvent *event);
	virtual bool eventFilter(QObject *obj, QEvent *event);

private:
    Ui::mainVideo ui;
	//�����ƶ�����ֵ
	QPoint m_point;
	volatile bool m_bMove = false;
	//����������
	QMediaPlayer* m_videoPlayer = nullptr;
	//�б�
	QMediaPlaylist *m_videoPlayList = nullptr;
	//���Ŵ���
	QVideoWidget* m_videoPlayWidget = nullptr;
	//������
	QVBoxLayout* layout_video = nullptr;
	//MV����
	QNetworkAccessManager *m_netWorkMvDownLoad = nullptr;
	//�������ݶ������
	bool m_isWorking = false;
	//���ݻ���
	MvInfoPlayer m_mvInfoPlayer;
	//��Ƶ�ؼ�
	QSharedPointer<VideoControls> m_videoControls = nullptr;
	//��Ƶʱ���ַ���
	QString m_videoTime;
	//�رհ�ť
	QSharedPointer<VideoControlTop> m_videoControlTops = nullptr;
};
