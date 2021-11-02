#include "Lrc.h"
#include <QPainter>
#include <QTimer>
#include <QDesktopWidget>
#include <QApplication>

Lrc::Lrc(QWidget *parent)
    : QLabel(parent), m_lrcWidth(1000), m_pText(QString()),
	m_lrcMaskWidth(0), m_lrcMaskWidthInterval(0)
{
	//背景透明去除边框
	//setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
	//setAttribute(Qt::WA_TranslucentBackground);

	////设置大小
	////捕捉桌面位置
	//QDesktopWidget* desktopWidget = QApplication::desktop();
	//m_deskRect = desktopWidget->availableGeometry();

	//初始化字体
	m_font.setFamily(QStringLiteral("微软雅黑"));
	m_font.setBold(true);
	m_font.setPointSize(36);

	//初始化歌词的线性渐变填充类
	m_textLinearGradient.setStart(0, 10);
	m_textLinearGradient.setFinalStop(0, 40);
	m_textLinearGradient.setColorAt(0.1, QColor(14, 179, 255));
	m_textLinearGradient.setColorAt(0.5, QColor(114, 232, 255));
	m_textLinearGradient.setColorAt(0.9, QColor(14, 179, 255));

	//初始化遮罩的线性渐变填充类
	m_maskLinearGradient.setStart(0, 10);
	m_maskLinearGradient.setFinalStop(0, 40);
	m_maskLinearGradient.setColorAt(0.1, QColor(222, 54, 4));
	m_maskLinearGradient.setColorAt(0.5, QColor(255, 72, 16));
	m_maskLinearGradient.setColorAt(0.9, QColor(222, 54, 4));

	// 定时器初始化
	m_timer = new QTimer(this);
	if (m_timer) {
		connect(m_timer, &QTimer::timeout, this, &Lrc::sltTimerOut);
	}
}

Lrc::~Lrc()
{
	stopLrcMask();
}

void Lrc::setLrcTextFontSize(int size)
{
	m_font.setPointSize(size);
}

void Lrc::startLrcMask(qint64 intervalTime, const QString& text)
{
	if (m_pText == text)
		return;
	//更换文本内容
	setLrcInfo(text);
	// 这里设置每隔30毫秒更新一次遮罩的宽度，因为如果更新太频繁
	// 会增加CPU占用率，而如果时间间隔太大，则动画效果就不流畅了
	qreal count = intervalTime / 30;
	// 获取遮罩每次需要增加的宽度，这里的m_lrcWidth是部件的固定宽度
	m_lrcMaskWidthInterval = m_lrcWidth / count;
	m_lrcMaskWidth = 0;
	if (m_timer) {
		m_timer->start(30);
	}
}

void Lrc::setNoLrc()
{
	startLrcMask(2000, QStringLiteral("暂时没有找到歌词!"));
}

void Lrc::stopLrcMask()
{
	if (m_timer) {
		m_timer->stop();
	}
	m_lrcMaskWidth = 0;
	update();
}

void Lrc::sltTimerOut()
{
	m_lrcMaskWidth += m_lrcMaskWidthInterval;
	update();
}

void Lrc::setLrcInfo(const QString& text)
{
	text.simplified();
	//setText(text);
	m_pText = text;
	//adjustSize();
	int a = this->font().pointSize();
	int b = strlen(reinterpret_cast<char*>(this->text().data()));
	m_lrcWidth = static_cast<int>(strlen(reinterpret_cast<char*>(this->text().data()))) * 36;
	setFixedSize(m_lrcWidth, 160);
	update();
	/*move((m_deskRect.width() - m_lrcMaskWidth) / 2, m_deskRect.height() - 160);*/
}

void Lrc::paintEvent(QPaintEvent * event)
{
	QPainter painter(this);
	painter.setFont(m_font);
	// 先绘制底层文字，作为阴影，这样会使显示效果更加清晰，且更有质感
	painter.setPen(QColor(0, 0, 0, 200));
	painter.drawText(1, 1, m_lrcWidth, 160, Qt::AlignLeft, m_pText);
	// 再在上面绘制渐变文字
	painter.setPen(QPen(m_textLinearGradient, 0));
	painter.drawText(0, 0, m_lrcWidth, 160, Qt::AlignLeft, m_pText);
	// 设置歌词遮罩
	painter.setPen(QPen(m_maskLinearGradient, 0));
	painter.drawText(0, 0, m_lrcMaskWidth, 160, Qt::AlignLeft, m_pText);
	QLabel::paintEvent(event);
}
