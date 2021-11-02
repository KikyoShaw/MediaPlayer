#pragma once

#include <QLabel>
#include <QLinearGradient>

class QTimer;

class Lrc : public QLabel
{
    Q_OBJECT

public:
    Lrc(QWidget *parent = Q_NULLPTR);
	~Lrc();

	//���ø�ʿؼ�����
	void setLrcWidth(int w) { m_lrcWidth = w; };
	//���ø�������С
	void setLrcTextFontSize(int size);
	//���ֿ����������ǵ�ǰ��ʾ��ʿ�ʼ������ʱ��͸���ı�����
	void startLrcMask(qint64 intervalTime, const QString& text);
	//�޸��ʱ
	void setNoLrc();
	//�ر�����
	void stopLrcMask();

private slots:
	void sltTimerOut();

private:
	void setLrcInfo(const QString& text);

private:
	virtual void paintEvent(QPaintEvent *event);

private:
	//��ʵ����Խ������
	QLinearGradient m_textLinearGradient;
	//���ֵ����Խ������
	QLinearGradient m_maskLinearGradient;
	//���ֳ���
	int m_lrcMaskWidth;
	//����ÿ�����ӵĳ���
	int m_lrcMaskWidthInterval;
	//��ʿؼ�����
	int m_lrcWidth;
	//��������
	QFont m_font;
	//��ʱ��ʵʱˢ��
	QTimer *m_timer = Q_NULLPTR;
	//�ı�����
	QString m_pText;
	//�����С
	QRect m_deskRect;
};
