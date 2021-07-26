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
	//���������
	m_musicLrcMap.clear();
	QString lrc = getJsonData(Object, key);
	//���
	QStringList lrcList = lrc.split("\n");
	for (int i = 3; i < lrcList.size() - 1; i++){
		QString line = lrcList.value(i);
		QRegExp ipRegExp = QRegExp("\\[\\d\\d\\S\\d\\d\\S\\d\\d\\]");
		bool match = ipRegExp.indexIn(line);
		if (!match){
			////ʱ�������ʽ(��*60+��)*100+����
			//int s_1 = line.mid(1, 2).toInt();      //��
			//int s_2 = line.mid(4, 2).toInt();      //��
			//int s_3 = line.mid(7, 2).toInt();      //����
			//int lrctime = (s_1 * 60 + s_2) * 100 + s_3;   //�涨д��
			QString timestr = line.mid(0, 10);
			QString lrcstr = line.mid(10);
			m_musicLrcMap.insert(timestr, lrcstr);
		}
	}
}

void MusicManager::getLoaclLrcData(const QString& filePath)
{
	//���������
	m_musicLrcMap.clear();
	//���У��
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
	////Ԫ���ݵĽ�����Ҫʱ�䣬����������Ҫѭ���ȴ�����ͬʱ��Ҫ����Qt�¼�������������У�
	//while (!m_musicPlayer->isMetaDataAvailable()) {
	//	QCoreApplication::processEvents();
	//}
	//QStringList list = m_musicPlayer->availableMetaData();//����ʱ�鿴����ЩԪ���ݿ���
	//if (m_musicPlayer->isMetaDataAvailable()) {
	//	//������Ϣ
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
	//����������ݻ��ߴ�������
	if (srcPixMap.isNull()) {
		return srcPixMap;
	}

	//��ȡͼƬ�ߴ�
	int imageWidth = size.width();
	int imageHeight = size.height();

	//�����ߴ��ͼƬ,��֤ͼƬ��ʾ��������
	QPixmap newPixMap = srcPixMap.scaled(imageWidth, (imageHeight == 0 ? imageWidth : imageHeight), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

	QPixmap destImage(imageWidth, imageHeight);
	destImage.fill(Qt::transparent);
	QPainter painter(&destImage);
	// �����
	painter.setRenderHints(QPainter::Antialiasing, true);
	// ͼƬƽ������
	painter.setRenderHints(QPainter::SmoothPixmapTransform, true);
	// ��ͼƬ�ü�ΪԲ��
	QPainterPath path;
	QRect rect(0, 0, imageWidth, imageHeight);
	path.addRoundedRect(rect, radius, radius);
	painter.setClipPath(path);
	painter.drawPixmap(0, 0, imageWidth, imageHeight, newPixMap);
	return destImage;
}
