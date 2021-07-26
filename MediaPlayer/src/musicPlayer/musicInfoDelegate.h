#pragma once

#pragma once

#include <QStyledItemDelegate>

enum E_MusicType
{
	E_None = 0, //ÎÞ
	E_Local = 1, //±¾µØ
	E_NetWork = 2, //ÍøÂç
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

