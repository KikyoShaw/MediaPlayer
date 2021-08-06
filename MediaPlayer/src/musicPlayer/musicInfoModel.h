#pragma once

#include <QAbstractListModel>

struct MusicInfo
{
	operator QVariant() const
	{
		return QVariant::fromValue(*this);
	}

	QString musicName; //歌曲名
	QString musicPlayer; //歌手
	QString musicAlbum; // 专辑
	int musicDuration = 0; // 时间
	QString mvHash; //mv hash
	QString hash; // hash
	QString albumId;//album_id
	QString path; //文件所在路径位置（不包含文件名）
	QString url; //下载链接
};
Q_DECLARE_METATYPE(MusicInfo);

class MusicInfoModel : public QAbstractListModel
{
	Q_OBJECT
public:
	MusicInfoModel();
	void clear();
	void addInfoToModel(MusicInfo pInfo);
	//获取hash
	QString getHash(int row);
	//根据hash获取mvhash
	QString getMvHash(const QString& hash);
	//获取album_id
	QString getAlbumId(int row);
	//  model/view override函数
	int rowCount(const QModelIndex &parent = QModelIndex()) const;
	QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;
	void setPageNum();
	// 获取页数
	int getPageNum();

protected:
	QVector<MusicInfo> m_listInfo;
	//根据hash存储数据
	QHash<QString, MusicInfo> m_listHash;
	//分页数
	int m_page = 0;
};

