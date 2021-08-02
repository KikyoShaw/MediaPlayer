#pragma once

#include <QAbstractListModel>

struct MvInfo
{
	operator QVariant() const
	{
		return QVariant::fromValue(*this);
	}

	QString mvID;
	QString mvName; //mv��
	QString mvPlayer; //����
	QString fileName; // �ļ���
	QString pic; //����
	QString ThumbGif; //gif����
	int64_t historyHeat = 0; // �ۿ�����
	int mvHot; //�ȶ�
	QString mvHash; //mv hash
	QString fileHash; // hash
	QString albumId;//album_id
	QString publishDate; //�ϴ�ʱ��
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
	//��ȡhash
	QString getMvHash(int row);
	//  model/view override����
	int rowCount(const QModelIndex &parent = QModelIndex()) const;
	QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;
	void setPageNum();
	// ��ȡҳ��
	int getPageNum();

protected:
	QVector<MvInfo> m_listInfo;
	//��ҳ��
	int m_page = 0;
};

