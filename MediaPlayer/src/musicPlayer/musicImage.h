#pragma once

#include <QLabel>

class QNetworkAccessManager;
class QNetworkReply;

class MusicImage : public QLabel
{
	Q_OBJECT
public:
	MusicImage(QWidget *parent = Q_NULLPTR);
	~MusicImage();

	void requestImage(const QString& url);

signals:
	void sigMouseClicked();

private slots:
	void sltNetWorkMusicImg(QNetworkReply *reply);

private:
	virtual bool eventFilter(QObject *obj, QEvent *event);

private:
	//ÍøÂç¸èÇúÍ¼Æ¬ÇëÇó
	QNetworkAccessManager *m_netWorkMusicImg = nullptr;
};