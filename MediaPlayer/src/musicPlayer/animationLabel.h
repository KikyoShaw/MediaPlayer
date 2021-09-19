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
	//���ƽ��ȶ���
	QVariantAnimation *m_vAnimation = Q_NULLPTR;
	//����
	int m_nAtIndex;
	//�ı�����
	QString m_pText;
};
