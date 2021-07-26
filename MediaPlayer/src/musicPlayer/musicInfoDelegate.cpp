#include "musicInfoDelegate.h"
#include <QPainter>
#include <QMouseEvent>
#include <QApplication>
#include "musicManager.h"

MusicInfoDelegate::MusicInfoDelegate(E_MusicType type)
{
	m_musicType = type;
}

void MusicInfoDelegate::paint(QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index) const
{
	QRect rect = option.rect;

	const QAbstractItemModel *pModel = index.model();
	auto info = pModel->data(index, Qt::DisplayRole).value<MusicInfo>();

	int x = rect.left();
	int y = rect.top();
	int width = rect.width();
	int height = rect.height();
	int pWidth = width / 4;

	const QWidget *widget = option.widget;
	auto style = widget ? widget->style() : QApplication::style();
	//先画背景
	style->drawPrimitive(QStyle::PE_PanelItemViewItem, &option, painter, option.widget);

	painter->save();

	painter->setRenderHint(QPainter::Antialiasing);  // 反锯齿;
	painter->setRenderHints(QPainter::SmoothPixmapTransform, true); //平滑边缘处理	

	QFont font(QStringLiteral("微软雅黑"));
	font.setPixelSize(14);
	QFontMetrics fm(font);
	painter->setFont(font);
	painter->setPen(QPen(QColor(255, 255, 255)));

	//歌名
	if (!info.musicName.isEmpty()) {
		QString strElidedText = fm.elidedText(info.musicName, Qt::ElideRight, pWidth, Qt::TextShowMnemonic);
		painter->drawText(QRect(x + 20, y + 20, width, height), strElidedText);
	}

	//歌手
	if (!info.musicPlayer.isEmpty()) {
		QString strElidedText = fm.elidedText(info.musicPlayer, Qt::ElideRight, pWidth, Qt::TextShowMnemonic);
		painter->drawText(QRect(x + pWidth + 40, y + 20, width, height), strElidedText);
	}

	//时长
	int timeWidth = 0;
	if(0 != info.musicDuration)
	{
		auto sec = info.musicDuration % 60;
		QString duration = "00:00";
		if (sec < 10 && sec > -0) {
			duration = QString("0%1:0%2").arg(info.musicDuration / 60).arg(sec);
		}
		else if (sec >= 10) {
			duration = QString("0%1:%2").arg(info.musicDuration / 60).arg(sec);
		}
		QString strElidedText = fm.elidedText(duration, Qt::ElideRight, pWidth, Qt::TextShowMnemonic);
		painter->drawText(QRect(-20, y + 20, width, height), Qt::AlignRight, strElidedText);
		timeWidth = fm.width(strElidedText);
	}

	//专辑名
	if (!info.musicAlbum.isEmpty()) {
		//根据宽度动态调整
		int posWidth = width - (x + pWidth + pWidth + 40) - timeWidth - 20 - 20;
		QString strElidedText = fm.elidedText(info.musicAlbum, Qt::ElideRight, posWidth, Qt::TextShowMnemonic);
		painter->drawText(QRect(x + pWidth + pWidth + 40, y + 20, width, height), strElidedText);
	}

	painter->restore();
}

bool MusicInfoDelegate::editorEvent(QEvent * event, QAbstractItemModel * model, const QStyleOptionViewItem & option, const QModelIndex & index)
{
	const QAbstractItemModel *pModel = index.model();
	int row = index.row();
	auto mouseEvent = dynamic_cast<QMouseEvent*>(event);
	switch (event->type())
	{
	case QEvent::MouseButtonDblClick:
	{
		if (Q_NULLPTR == mouseEvent) {
			break;
		}
		if (option.rect.contains(mouseEvent->pos())) {
			if (E_MusicType::E_NetWork == m_musicType) {
				emit musicManager.sigMouseDoubleClicked(row);
			}
			else if (E_MusicType::E_Local == m_musicType) {
				emit musicManager.sigMouseDoubleClicked_local(row);
			}
		}
		break;
	}
	default:
		break;
	}
	return false;
}
