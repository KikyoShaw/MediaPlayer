#ifndef START_H
#define START_H

#include <QWidget>
#include "ui_videoWidget.h"

class QMediaPlayer;
class QVideoWidget;
class QVBoxLayout;

class VideoWidget : public QWidget
{
    Q_OBJECT

public:
    explicit VideoWidget(QWidget *parent = 0);
    ~VideoWidget();

	void setVideoPlay(const QString &filePath);

protected:
	virtual void mouseMoveEvent(QMouseEvent *event);
	virtual void mousePressEvent(QMouseEvent *event);
	virtual void mouseReleaseEvent(QMouseEvent *event);

private:
    Ui::mainVideo ui;
	//窗口移动属性值
	QPoint m_point;
	volatile bool m_bMove = false;
	//播放器对象
	QMediaPlayer* m_videoPlayer = nullptr;
	//播放窗口
	QVideoWidget* m_videoPlayWidget = nullptr;
	//布局器
	QVBoxLayout* layout_video = nullptr;
};

#endif // START_H
