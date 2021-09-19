#include "musicManager.h"
#include <QJsonObject>
#include <QFile>
#include <QTextStream>
#include <QPainter>
#include <QDebug>

static int64_t ChangeTimeSpanStringToMillisconds(QString strTime)
{
	//��
	auto mm = strTime.mid(1, 2).toInt();
	auto ss = strTime.mid(4, 2).toInt();
	auto hs = strTime.mid(7, 2).toInt();
	int64_t result = 0;
	result = ((mm * 60) + ss ) * 1000 + hs;
	return result;
 }

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
	m_lrcTimeMap.clear();
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
			auto timerNum = ChangeTimeSpanStringToMillisconds(timestr);
			QString lrcstr = line.mid(10);
			m_musicLrcMap.insert(timestr, lrcstr);
			auto vec = lrcstr + "|" + QString::number(timerNum);
			m_lrcTimeMap.push_back(vec);
		}
	}
}

void MusicManager::getLoaclLrcData(const QString& filePath)
{
	//���������
	m_musicLrcMap.clear();
	m_lrcTimeMap.clear();
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
				auto timerNum = ChangeTimeSpanStringToMillisconds(timestr);
				QString lrcstr = line.mid(10).remove(QRegExp("\\s"));
				m_musicLrcMap.insert(timestr, lrcstr);
				auto vec = lrcstr + "|" + QString::number(timerNum);
				m_lrcTimeMap.push_back(vec);
			}
			file.close();
		}
	}
}

bool MusicManager::isHasLrc()
{
	bool result = false;
	if (!m_musicLrcMap.isEmpty()) {
		result = true;
	}
	return result;
}

int64_t MusicManager::getNextLrcTime(const QString & lrc, int position)
{
	int64_t result = 0;
	for (int i = 0; i < m_lrcTimeMap.size(); i++) {
		auto lrcInfos = m_lrcTimeMap.value(i).split("|");
		if (lrcInfos.size() > 1){
			auto lrcText = lrcInfos.value(0);
			if (lrc == lrcText) {
				auto nextLrcInfos = m_lrcTimeMap.value(i + 1).split("|");
				if (nextLrcInfos.size() > 1) {
					result = (int64_t)nextLrcInfos.value(1).toInt();
					//qInfo() << result << position << nextLrcInfos;
					if (result > position) {
						break;
					}
				}
			}
		}
	}

	return result;
}

MusicInfoModel & MusicManager::getLocalMusicInfoModel()
{
	return m_localMusicInfoModel;
}

MusicInfoModel & MusicManager::getMusicInfoModel()
{
	return m_musicInfoModel;
}

MvInfoModel & MusicManager::getMvInfoModel()
{
	return m_mvInfoModel;
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
		auto num1 = key.mid(1, 2).toInt();
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

QString MusicManager::createNumber(int64_t number)
{
	QString strNumber = QString();
	number = qMax(number, (int64_t)0);

	if (number >= 100000000)
	{
		double numberE = number / (double)100000000.0;
		strNumber += QStringLiteral("%1��").arg(QString::asprintf("%.2f", numberE));
	}
	else if (number >= 10000)
	{
		double numberW = number / (double)10000.0;
		strNumber += QStringLiteral("%1��").arg(QString::asprintf("%.2f", numberW));
	}
	else
	{
		strNumber += QStringLiteral("%1").arg(number);
	}
	return strNumber;
}
