#include "musicManager.h"
#include <QJsonObject>
#include <QFile>
#include <QTextStream>
#include <QPainter>

MusicManager & MusicManager::getInstance()
{
	static MusicManager m_instance;
	return m_instance;
}

MusicManager::MusicManager(QObject *parent)
	: QObject(parent), m_imagePixMap(QPixmap())
{
}

MusicManager::~MusicManager()
{
}

QString MusicManager::getJsonData(QJsonObject Object, const QString & key)
{
	if (Object.contains(key)) {
		QJsonValue value = Object.take(key);
		if (value.isString()) {
			return value.toString();
		}
	}
	return QString();
}

void MusicManager::getLrcJsonData(QJsonObject Object, const QString & key)
{
	//先清除数据
	m_musicLrcMap.clear();
	QString lrc = getJsonData(Object, key);
	//拆分
	QStringList lrcList = lrc.split("\n");
	for (int i = 3; i < lrcList.size() - 1; i++){
		QString line = lrcList.value(i);
		QRegExp ipRegExp = QRegExp("\\[\\d\\d\\S\\d\\d\\S\\d\\d\\]");
		bool match = ipRegExp.indexIn(line);
		if (!match){
			////时间解析格式(分*60+秒)*100+厘秒
			//int s_1 = line.mid(1, 2).toInt();      //分
			//int s_2 = line.mid(4, 2).toInt();      //秒
			//int s_3 = line.mid(7, 2).toInt();      //厘秒
			//int lrctime = (s_1 * 60 + s_2) * 100 + s_3;   //规定写法
			QString timestr = line.mid(0, 10);
			QString lrcstr = line.mid(10);
			m_musicLrcMap.insert(timestr, lrcstr);
		}
	}
}

void MusicManager::getLoaclLrcData(const QString& filePath)
{
	//先清除数据
	m_musicLrcMap.clear();
	//歌词校验
	QFile file(filePath);
	//qDebug() << ss +playList->currentMedia().canonicalUrl().fileName().split(".").front() + ".lrc";
	if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
		QTextStream stream(&file);
		QString line;
		while (!stream.atEnd()) {
			line = stream.readLine();
			QRegExp ipRegExp = QRegExp("\\[\\d\\d\\S\\d\\d\\S\\d\\d\\]");
			bool match = ipRegExp.indexIn(line);
			if (!match) {
				QString timestr = line.mid(0, 10);
				QString lrcstr = line.mid(10);
				m_musicLrcMap.insert(timestr, lrcstr);
			}
			file.close();
		}
	}
}

MusicInfoModel & MusicManager::getLocalMusicInfoModel()
{
	return m_localMusicInfoModel;
}

MusicInfoModel & MusicManager::getMusicInfoModel()
{
	return m_musicInfoModel;
}

MusicInfo & MusicManager::getPlayingMusicInfo()
{
	return m_playingMusicInfo;
}

QMap<QString, QString> & MusicManager::getMusicLrcMap()
{
	return m_musicLrcMap;
}

void MusicManager::initMediaPlayInfo()
{
	////元数据的解析需要时间，所以这里需要循环等待（但同时需要保持Qt事件处理机制在运行）
	//while (!m_musicPlayer->isMetaDataAvailable()) {
	//	QCoreApplication::processEvents();
	//}
	//QStringList list = m_musicPlayer->availableMetaData();//调试时查看有哪些元数据可用
	//if (m_musicPlayer->isMetaDataAvailable()) {
	//	//歌曲信息
	//	QString author = m_musicPlayer->metaData(QStringLiteral("Author")).toStringList().join(",");
	//	QString title = m_musicPlayer->metaData(QStringLiteral("Title")).toString();
	//	QString albumTitle = m_musicPlayer->metaData(QStringLiteral("AlbumTitle")).toString();
	//	int audioBitRate = m_musicPlayer->metaData(QStringLiteral("AudioBitRate")).toInt();
	//	qint64 duration = m_musicPlayer->duration();
	//}
}

QString MusicManager::getLrcByTime(int time)
{
	QString lrc = QString();
	QMap<QString, QString>::iterator iter = m_musicLrcMap.begin();
	while (iter != m_musicLrcMap.end())
	{
		QString key = iter.key();
		if (key.mid(1, 2).toInt() == (time / 60000) && (key.mid(4, 2)).toInt() == (time % 60000 / 1000)) {
			lrc = iter.value();
		}
		iter++;
	}
	return lrc;
}

void MusicManager::saveMusicImage(QPixmap pixmap)
{
	m_imagePixMap = pixmap;
}

QPixmap &MusicManager::getMusicImage()
{
	return m_imagePixMap;
}

void MusicManager::savePlayingMusicInfo(MusicInfo info)
{
	m_playingMusicInfo = info;
}

QPixmap MusicManager::getRoundRectPixmap(QPixmap srcPixMap, const QSize & size, int radius)
{
	//不处理空数据或者错误数据
	if (srcPixMap.isNull()) {
		return srcPixMap;
	}

	//获取图片尺寸
	int imageWidth = size.width();
	int imageHeight = size.height();

	//处理大尺寸的图片,保证图片显示区域完整
	QPixmap newPixMap = srcPixMap.scaled(imageWidth, (imageHeight == 0 ? imageWidth : imageHeight), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

	QPixmap destImage(imageWidth, imageHeight);
	destImage.fill(Qt::transparent);
	QPainter painter(&destImage);
	// 抗锯齿
	painter.setRenderHints(QPainter::Antialiasing, true);
	// 图片平滑处理
	painter.setRenderHints(QPainter::SmoothPixmapTransform, true);
	// 将图片裁剪为圆角
	QPainterPath path;
	QRect rect(0, 0, imageWidth, imageHeight);
	path.addRoundedRect(rect, radius, radius);
	painter.setClipPath(path);
	painter.drawPixmap(0, 0, imageWidth, imageHeight, newPixMap);
	return destImage;
}
