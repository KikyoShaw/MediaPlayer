#pragma once

#include <QAbstractListModel>

struct MusicInfo
{
	operator QVariant() const
	{
		return QVariant::fromValue(*this);
	}

	QString musicName; //������
	QString musicPlayer; //����
	QString musicAlbum; // ר��
	int musicDuration = 0; // ʱ��
	QString mvHash; //mv hash
	QString hash; // hash
	QString albumId;//album_id
	QString path; //�ļ�����·��λ�ã��������ļ�����
	QString url; //��������
};
Q_DECLARE_METATYPE(MusicInfo);

class MusicInfoModel : public QAbstractListModel
{
	Q_OBJECT
public:
	MusicInfoModel();
	void clear();
	void addInfoToModel(MusicInfo pInfo);
	//��ȡhash
	QString getHash(int row);
	//����hash��ȡmvhash
	QString getMvHash(const QString& hash);
	//��ȡalbum_id
	QString getAlbumId(int row);
	//  model/view override����
	int rowCount(const QModelIndex &parent = QModelIndex()) const;
	QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;
	void setPageNum();
	// ��ȡҳ��
	int getPageNum();

protected:
	QVector<MusicInfo> m_listInfo;
	//����hash�洢����
	QHash<QString, MusicInfo> m_listHash;
	//��ҳ��
	int m_page = 0;
};

