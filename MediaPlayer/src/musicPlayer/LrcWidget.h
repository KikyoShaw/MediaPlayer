#pragma once

#include <QWidget>
#include "ui_LrcWidget.h"

class LrcWidget : public QWidget
{
	Q_OBJECT
public:
	LrcWidget(QWidget *parent = Q_NULLPTR);
	~LrcWidget();

	void setLrc();
	void setLrc(const QString &text, int position);

private:
	virtual void mouseMoveEvent(QMouseEvent *event);
	virtual void mousePressEvent(QMouseEvent *event);
	virtual void mouseReleaseEvent(QMouseEvent *event);

private:
	Ui::lrc ui;
	//�����ƶ�����ֵ
	QPoint m_point;
	volatile bool m_bMove = false;
};
