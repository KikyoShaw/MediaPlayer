#pragma once

#include <QProxyStyle>

// gif��̬ͼ��Բ����ʽ
class GifProxyStyle : public QProxyStyle
{
public:
	explicit GifProxyStyle(QSize gifSize, int radius, QStyle* style = Q_NULLPTR);
	~GifProxyStyle();

protected:
	//�����¼�
	virtual void drawItemPixmap(QPainter *painter, const QRect &rect, int alignment, const QPixmap &pixmap) const;

private:
	int m_radius;
	QSize m_gifSize = QSize(400, 400);
};

