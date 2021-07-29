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
	//�����ƶ�����ֵ
	QPoint m_point;
	volatile bool m_bMove = false;
	//����������
	QMediaPlayer* m_videoPlayer = nullptr;
	//���Ŵ���
	QVideoWidget* m_videoPlayWidget = nullptr;
	//������
	QVBoxLayout* layout_video = nullptr;
};

#endif // START_H
