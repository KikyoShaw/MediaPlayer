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
	QString hash; //mv哈希
	QString mvName; //mv名称
	QString url; //链接
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
	//窗口移动属性值
	QPoint m_point;
	volatile bool m_bMove = false;
	//播放器对象
	QMediaPlayer* m_videoPlayer = nullptr;
	//列表
	QMediaPlaylist *m_videoPlayList = nullptr;
	//播放窗口
	QVideoWidget* m_videoPlayWidget = nullptr;
	//布局器
	QVBoxLayout* layout_video = nullptr;
	//MV下载
	QNetworkAccessManager *m_netWorkMvDownLoad = nullptr;
	//避免数据多次请求
	bool m_isWorking = false;
	//数据缓存
	MvInfoPlayer m_mvInfoPlayer;
	//视频控件
	QSharedPointer<VideoControls> m_videoControls = nullptr;
	//视频时间字符串
	QString m_videoTime;
	//关闭按钮
	QSharedPointer<VideoControlTop> m_videoControlTops = nullptr;
};
