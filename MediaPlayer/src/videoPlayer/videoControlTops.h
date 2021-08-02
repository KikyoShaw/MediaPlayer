#pragma once

#include <QWidget>
#include "ui_videoControlTops.h"

class VideoControlTop : public QWidget
{
	Q_OBJECT
public:
	VideoControlTop(QWidget *parent = Q_NULLPTR);
	~VideoControlTop();

	void setTitle(const QString& title);

signals:
	void sigClose();

private:
	Ui::controlTops ui;
};