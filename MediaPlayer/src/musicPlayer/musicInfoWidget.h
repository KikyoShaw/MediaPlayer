#pragma once

#include <QWidget>
#include "ui_musicInfoWidget.h"

class MusicInfoWidget : public QWidget
{
	Q_OBJECT
public:
	MusicInfoWidget(QWidget *parent = Q_NULLPTR);
	~MusicInfoWidget();

	//��ʾ������Ϣ
	void setMusicInfo();
	//��ʾ���
	void setLrcInfo(int position);

private:
	Ui::musicWidget ui;
};



