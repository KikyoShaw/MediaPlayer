#pragma once

#include <QObject>
#include <QPixmap>
#include "musicInfoModel.h"
#include "mvInfoModel.h"

class MusicManager : public QObject
{
	Q_OBJECT
public:
	static MusicManager& getInstance();
	~MusicManager();

	//��ȡjson����
	QString getJsonData(QJsonObject Object, const QString& key);
	//����json���ݽ������
	void getLrcJsonData(QJsonObject Object, const QString& key);
	//�������ظ��
	void getLoaclLrcData(const QString& filePath);
	//��ȡ���
	QString getLrcByTime(int time);

	//���汾��ͼƬ
	void saveMusicImage(QPixmap pixmap);
	//��ȡ����ͼƬ����
	QPixmap &getMusicImage();

	//���汾�β��Ÿ�����Ϣ
	void savePlayingMusicInfo(MusicInfo info);

	//ͼƬԲ�ǻ�����
	QPixmap getRoundRectPixmap(QPixmap srcPixMap, const QSize & size, int radius);

	//���ִ���
	QString createNumber(int64_t number);

	////�첽get����,��������
	//QtPromise::QPromise<QByteArray> get(const QUrl &url);

public:
	MusicInfoModel &getLocalMusicInfoModel();
	MusicInfoModel &getMusicInfoModel();
	MvInfoModel &getMvInfoModel();
	MusicInfo &getPlayingMusicInfo();
	QMap<QString, QString> &getMusicLrcMap();

signals:
	void sigMouseDoubleClicked(int row);
	void sigMouseDoubleClicked_local(int row);

private:
	void initMediaPlayInfo();

private:
	//���ظ�����Ϣ
	MusicInfoModel m_localMusicInfoModel;
	//������Ϣ
	MusicInfoModel m_musicInfoModel;
	//mv��Ϣ
	MvInfoModel m_mvInfoModel;
	//��ǰ���ŵĸ�����Ϣ
	MusicInfo m_playingMusicInfo;
	//���
	QMap<QString, QString> m_musicLrcMap;
	//ͼƬ
	QPixmap m_imagePixMap;
	//����·��
	QString m_downLoadFilePath;

private:
	MusicManager(QObject *parent = Q_NULLPTR);
	Q_DISABLE_COPY(MusicManager)

};

#define musicManager MusicManager::getInstance()
