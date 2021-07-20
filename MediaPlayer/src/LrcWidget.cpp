#include "LrcWidget.h"
#include <QDesktopWidget>
#include <QMouseEvent>

LrcWidget::LrcWidget(QWidget *parent)
	:QWidget(parent)
{
	ui.setupUi(this);

	setWindowFlags(Qt::FramelessWindowHint | Qt::SubWindow | Qt::WindowStaysOnTopHint);
	setAttribute(Qt::WA_TranslucentBackground);

	//设置样式
	ui.label_lrc->setFont(QFont("华文彩云", 32));
	ui.label_lrc->setText(QStringLiteral("聆听世界的美"));

	//捕捉桌面位置
	QDesktopWidget* desktopWidget = QApplication::desktop();
	QRect deskRect = desktopWidget->availableGeometry();
	setFixedSize(deskRect.width(), 100);
	move(0, deskRect.height() - 160);
}

LrcWidget::~LrcWidget()
{
}

void LrcWidget::setLrc(const QString & text)
{
	ui.label_lrc->setText(text);
}

void LrcWidget::mouseMoveEvent(QMouseEvent * event)
{
	//判断左键是否被按下，只有左键按下了，其返回值就是1(true)
	if ((event->buttons() & Qt::LeftButton) && m_bMove)
	{
		move(event->globalPos() - m_point);
	}
	QWidget::mouseMoveEvent(event);
}

void LrcWidget::mousePressEvent(QMouseEvent * event)
{
	//鼠标事件中包含“按住的是左键”
	if ((event->button() == Qt::LeftButton) && (event->pos().y() < 56) && (event->pos().y() > 0))
	{
		m_bMove = true;
		//获取移动的位移量
		m_point = event->globalPos() - frameGeometry().topLeft();
	}
}

void LrcWidget::mouseReleaseEvent(QMouseEvent * event)
{
	m_bMove = false;
}
