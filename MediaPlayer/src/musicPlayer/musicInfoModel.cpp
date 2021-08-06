#include "musicInfoModel.h"

MusicInfoModel::MusicInfoModel()
{

}

void MusicInfoModel::clear()
{
	//  清除映射和标记
	m_listHash.clear();
	m_listInfo.swap(QVector<MusicInfo>());
	m_page = 0;
}

QVariant MusicInfoModel::data(const QModelIndex & index, int role) const
{
	if (index.row() < 0 || index.row() >= m_listInfo.count()) {
		return QVariant();
	}
	if (Qt::DisplayRole == role) {
		MusicInfo pInfo = m_listInfo.at(index.row());
		return pInfo;
	}
	return QVariant();
}

void MusicInfoModel::setPageNum()
{
	m_page += 1;
}

int MusicInfoModel::getPageNum()
{
	return m_page;
}

void MusicInfoModel::addInfoToModel(MusicInfo pInfo)
{
	m_listHash.insert(pInfo.hash, pInfo);
	beginInsertRows(QModelIndex(), rowCount(), rowCount());
	m_listInfo.push_back(pInfo);
	endInsertRows();
}

QString MusicInfoModel::getHash(int row)
{
	auto size = m_listInfo.count();
	if (size < row) return QString();
	return m_listInfo.value(row).hash;
}

QString MusicInfoModel::getMvHash(const QString & hash)
{
	QString mvHash = QString();
	if (m_listHash.contains(hash)) {
		mvHash = m_listHash.value(hash).mvHash;
	}
	return mvHash;
}

QString MusicInfoModel::getAlbumId(int row)
{
	auto size = m_listInfo.count();
	if (size < row) return QString();
	return m_listInfo.value(row).albumId;
}

int MusicInfoModel::rowCount(const QModelIndex & parent) const
{
	Q_UNUSED(parent);
	return m_listInfo.count();
}
