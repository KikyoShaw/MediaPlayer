#include "animationLabel.h"
#include <QVariantAnimation>
#include <QPainter>
#include <QStyleOption>
#include "musicManager.h"
#include <QDebug>

#define FONTSIZE 48

animationLabel::animationLabel(QWidget *parent)
	:QLabel(parent), m_nAtIndex(0), 
	m_pText(QString())
{
	m_vAnimation = new QVariantAnimation(this);
	if (m_vAnimation) {
		connect(m_vAnimation, &QVariantAnimation::valueChanged, this, &animationLabel::sltAtIndexChanged);
	}
}

animationLabel::~animationLabel()
{
}

void animationLabel::setNoLrc()
{
	m_pText = QStringLiteral("聆听世界的美");
	m_vAnimation->setDuration(1000);
	m_vAnimation->setStartValue(0);
	auto index = m_pText.size() + 1;
	m_vAnimation->setEndValue(index);
	m_vAnimation->start();
}

void animationLabel::setTimerAndStartAnimation(QString text, int position)
{
	if (text.isEmpty())
		return;

	if (m_pText == text)
		return;

	if (Q_NULLPTR == m_vAnimation)
		return;

	if (QAbstractAnimation::Running == m_vAnimation->state()) {
		m_vAnimation->stop();
	}

	if (QAbstractAnimation::Stopped == m_vAnimation->state()) {
		m_pText = text;
		//获取下一句歌词时间
		auto pDuration = musicManager.getNextLrcTime(text, position);
		auto timeNum = abs(pDuration - position - 100);
		m_vAnimation->setDuration(timeNum);
		//qInfo() << pDuration << position << timeNum;
		auto index = text.size() + 1;
		m_vAnimation->setStartValue(0);
		m_vAnimation->setEndValue(index);
		m_vAnimation->start();
		setFixedWidth(text.size() * FONTSIZE);
		update();
	}
}

void animationLabel::paintEvent(QPaintEvent * event)
{
	QPainter painter(this);
	// 抗锯齿
	painter.setRenderHints(QPainter::Antialiasing, true);
	// 平滑处理
	painter.setRenderHints(QPainter::SmoothPixmapTransform, true);
	QFont font(QStringLiteral("华文彩云"));
	font.setPixelSize(FONTSIZE);
	painter.setFont(font);
	//绘制文本
	int x = 0;
	for (int i = 0; i < m_pText.size(); i++) {
		if (i < m_nAtIndex) {
			painter.setPen(QPen(Qt::green));
		}
		else {
			painter.setPen(QPen(Qt::white));
		}
		QString each = m_pText[i];
		painter.drawText(QRect(x, 0, width(), height()), each);
		x += FONTSIZE;
	}
	QLabel::paintEvent(event);
}

void animationLabel::sltAtIndexChanged(QVariant value)
{
	m_nAtIndex = value.toDouble();
	update();
}
