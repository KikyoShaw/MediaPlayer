#include "mvInfoModel.h"

MvInfoModel::MvInfoModel()
{

}

void MvInfoModel::clear()
{
	//  清除映射和标记
	m_listInfo.swap(QVector<MvInfo>());
	m_page = 0;
}

QVariant MvInfoModel::data(const QModelIndex & index, int role) const
{
	if (index.row() < 0 || index.row() >= m_listInfo.count()) {
		return QVariant();
	}
	if (Qt::DisplayRole == role) {
		MvInfo pInfo = m_listInfo.at(index.row());
		return pInfo;
	}
	return QVariant();
}

void MvInfoModel::setPageNum()
{
	m_page += 1;
}

int MvInfoModel::getPageNum()
{
	return m_page;
}

void MvInfoModel::addInfoToModel(MvInfo pInfo)
{
	beginInsertRows(QModelIndex(), rowCount(), rowCount());
	m_listInfo.push_back(pInfo);
	endInsertRows();
}

const QVector<MvInfo>& MvInfoModel::getList()
{
	return m_listInfo;
}

QString MvInfoModel::getMvHash(int row)
{
	auto size = m_listInfo.count();
	if (size < row) return QString();
	return m_listInfo.value(row).mvHash;
}

int MvInfoModel::rowCount(const QModelIndex & parent) const
{
	Q_UNUSED(parent);
	return m_listInfo.count();
}
