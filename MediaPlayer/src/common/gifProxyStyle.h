#pragma once

#include <QProxyStyle>

// gif动态图的圆角样式
class GifProxyStyle : public QProxyStyle
{
public:
	explicit GifProxyStyle(QSize gifSize, int radius, QStyle* style = Q_NULLPTR);
	~GifProxyStyle();

protected:
	//绘制事件
	virtual void drawItemPixmap(QPainter *painter, const QRect &rect, int alignment, const QPixmap &pixmap) const;

private:
	int m_radius;
	QSize m_gifSize = QSize(400, 400);
};

