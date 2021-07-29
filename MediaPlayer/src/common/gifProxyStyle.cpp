#include "gifProxyStyle.h"
#include <QPainter>

static QPixmap scaledPixmap(const QPixmap & src, int width, int height)
{
	return src.scaled(width, (height == 0 ? width : height),
		Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
}

static QPixmap generatePixmap(const QPixmap & src, const QSize & size, const int & radius)
{
	// ÎÞÐ§Í¼Æ¬²»´¦Àí
	if (src.isNull()) {
		return src;
	}

	//»ñÈ¡Í¼Æ¬³ß´ç
	int imageWidth = size.width();
	int imageHeight = size.height();

	// Ëõ·ÅÍ¼Æ¬
	QPixmap pixmap = scaledPixmap(src, imageWidth, imageHeight);

	QPixmap destImage(imageWidth, imageHeight);
	destImage.fill(Qt::transparent);
	QPainter painter(&destImage);
	// ¿¹¾â³Ý + Æ½»¬±ßÔµ´¦Àí
	painter.setRenderHints(QPainter::Antialiasing, true);
	painter.setRenderHints(QPainter::SmoothPixmapTransform, true);

	// ²Ã¼ôÎªÔ²½Ç
	QPainterPath path;
	QRect rect(0, 0, imageWidth, imageHeight);
	path.addRoundedRect(rect, radius, radius);
	painter.setClipPath(path);
	painter.drawPixmap(0, 0, imageWidth, imageHeight, pixmap);
	return destImage;
}

GifProxyStyle::GifProxyStyle(QSize gifSize, int radius, QStyle * style)
	: QProxyStyle(style)
{
	m_gifSize = gifSize;
	m_radius = radius;
}

GifProxyStyle::~GifProxyStyle()
{
}

void GifProxyStyle::drawItemPixmap(QPainter * painter, const QRect & rect, int alignment, const QPixmap & pixmap) const
{
	painter->save();
	// Ã¿ÕÅÍ¼×öÔ²½Ç
	QPixmap destImage = generatePixmap(pixmap, m_gifSize, m_radius);
	// »­Ô²½ÇÍ¼
	painter->drawPixmap(0, 0, m_gifSize.width(), m_gifSize.height(), destImage);
	painter->restore();
}
