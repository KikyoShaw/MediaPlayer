#pragma once

#include <QWidget>
#include "ui_musicInfoWidget.h"

class MusicInfoWidget : public QWidget
{
	Q_OBJECT
public:
	MusicInfoWidget(QWidget *parent = Q_NULLPTR);
	~MusicInfoWidget();

	//显示歌曲信息
	void setMusicInfo();
	//显示歌词
	void setLrcInfo(int position);

private:
	void clearUI();

private:
	Ui::musicWidget ui;
};



