#include "LrcWidget.h"
#include <QDesktopWidget>
#include <QMouseEvent>

LrcWidget::LrcWidget(QWidget *parent)
	:QWidget(parent)
{
	ui.setupUi(this);

	setWindowFlags(Qt::FramelessWindowHint | Qt::SubWindow | Qt::WindowStaysOnTopHint);
	setAttribute(Qt::WA_TranslucentBackground);

	//������ʽ
	ui.label_lrc->setFont(QFont("���Ĳ���", 32));
	ui.label_lrc->setText(QStringLiteral("�����������"));

	//��׽����λ��
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
	//�ж�����Ƿ񱻰��£�ֻ����������ˣ��䷵��ֵ����1(true)
	if ((event->buttons() & Qt::LeftButton) && m_bMove)
	{
		move(event->globalPos() - m_point);
	}
	QWidget::mouseMoveEvent(event);
}

void LrcWidget::mousePressEvent(QMouseEvent * event)
{
	//����¼��а�������ס���������
	if ((event->button() == Qt::LeftButton) && (event->pos().y() < 56) && (event->pos().y() > 0))
	{
		m_bMove = true;
		//��ȡ�ƶ���λ����
		m_point = event->globalPos() - frameGeometry().topLeft();
	}
}

void LrcWidget::mouseReleaseEvent(QMouseEvent * event)
{
	m_bMove = false;
}
