#pragma once

#include <QLabel>
#include <QVariant>

class QVariantAnimation;

class animationLabel : public QLabel
{
	Q_OBJECT
public:
	explicit animationLabel(QWidget *parent = Q_NULLPTR);
	~animationLabel();

	void setNoLrc();

	void setTimerAndStartAnimation(QString text,int position);

private slots:
	void sltAtIndexChanged(QVariant value);

private:
	virtual void paintEvent(QPaintEvent *event);

private:
	//控制进度动画
	QVariantAnimation *m_vAnimation = Q_NULLPTR;
	//进度
	int m_nAtIndex;
	//文本内容
	QString m_pText;
};
