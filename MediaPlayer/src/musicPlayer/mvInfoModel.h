#pragma once

#include <QAbstractListModel>

struct MvInfo
{
	operator QVariant() const
	{
		return QVariant::fromValue(*this);
	}

	QString mvID;
	QString mvName; //mv名
	QString mvPlayer; //歌手
	QString fileName; // 文件名
	QString pic; //封面
	QString ThumbGif; //gif封面
	int64_t historyHeat = 0; // 观看数量
	int mvHot; //热度
	QString mvHash; //mv hash
	QString fileHash; // hash
	QString albumId;//album_id
	QString publishDate; //上传时间
};
Q_DECLARE_METATYPE(MvInfo);

class MvInfoModel : public QAbstractListModel
{
	Q_OBJECT
public:
	MvInfoModel();
	void clear();
	void addInfoToModel(MvInfo pInfo);
	const QVector<MvInfo>& getList();
	//获取hash
	QString getMvHash(int row);
	//  model/view override函数
	int rowCount(const QModelIndex &parent = QModelIndex()) const;
	QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;
	void setPageNum();
	// 获取页数
	int getPageNum();

protected:
	QVector<MvInfo> m_listInfo;
	//分页数
	int m_page = 0;
};

