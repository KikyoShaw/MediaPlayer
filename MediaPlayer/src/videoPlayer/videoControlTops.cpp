#include "videoControlTops.h"

VideoControlTop::VideoControlTop(QWidget *parent)
	:QWidget(parent)
{
	ui.setupUi(this);
	setWindowFlags(Qt::FramelessWindowHint | Qt::Tool);
	setAttribute(Qt::WA_TranslucentBackground);

	connect(ui.pushButton_closeMv, &QPushButton::clicked, this, &VideoControlTop::sigClose);

}

VideoControlTop::~VideoControlTop()
{
}

void VideoControlTop::setTitle(const QString & title)
{
	ui.label_name_mv->setText(title);
}
