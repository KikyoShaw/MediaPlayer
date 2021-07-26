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
	QString hash; // hash
	QString albumId;//album_id
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
	//分页数
	int m_page = 0;
};

