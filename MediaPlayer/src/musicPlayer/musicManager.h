#pragma once

#include <QObject>
#include <QPixmap>
//#include "qtpromise\qpromise.h"
#include "musicInfoModel.h"

class MusicManager : public QObject
{
	Q_OBJECT
public:
	static MusicManager& getInstance();
	~MusicManager();

	//获取json数据
	QString getJsonData(QJsonObject Object, const QString& key);
	//根据json数据解析歌词
	void getLrcJsonData(QJsonObject Object, const QString& key);
	//解析本地歌词
	void getLoaclLrcData(const QString& filePath);
	//读取歌词
	QString getLrcByTime(int time);

	//缓存本次图片
	void saveMusicImage(QPixmap pixmap);
	//拿取本次图片缓存
	QPixmap &getMusicImage();

	//缓存本次播放歌曲信息
	void savePlayingMusicInfo(MusicInfo info);

	//图片圆角化处理
	QPixmap getRoundRectPixmap(QPixmap srcPixMap, const QSize & size, int radius);

	////异步get请求,用于下载
	//QtPromise::QPromise<QByteArray> get(const QUrl &url);

public:
	MusicInfoModel &getLocalMusicInfoModel();
	MusicInfoModel &getMusicInfoModel();
	MusicInfo &getPlayingMusicInfo();
	QMap<QString, QString> &getMusicLrcMap();

signals:
	void sigMouseDoubleClicked(int row);
	void sigMouseDoubleClicked_local(int row);

private:
	void initMediaPlayInfo();

private:
	//本地歌曲信息
	MusicInfoModel m_localMusicInfoModel;
	//歌曲信息
	MusicInfoModel m_musicInfoModel;
	//当前播放的歌曲信息
	MusicInfo m_playingMusicInfo;
	//歌词
	QMap<QString, QString> m_musicLrcMap;
	//图片
	QPixmap m_imagePixMap;

private:
	MusicManager(QObject *parent = Q_NULLPTR);
	Q_DISABLE_COPY(MusicManager)

};

#define musicManager MusicManager::getInstance()
