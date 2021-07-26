#pragma once

#pragma once

#include <QStyledItemDelegate>

enum E_MusicType
{
	E_None = 0, //��
	E_Local = 1, //����
	E_NetWork = 2, //����
};

class MusicInfoDelegate : public QStyledItemDelegate
{
	Q_OBJECT

public:
	MusicInfoDelegate(E_MusicType type);

protected:
	void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
	bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index);

private:
	E_MusicType m_musicType;
};

