#include "musicInfoWidget.h"
#include "musicManager.h"

MusicInfoWidget::MusicInfoWidget(QWidget *parent)
	:QWidget(parent)
{
	ui.setupUi(this);
}

MusicInfoWidget::~MusicInfoWidget()
{
}

void MusicInfoWidget::setMusicInfo()
{
	//封面
	QPixmap pixmap = musicManager.getMusicImage();
	auto _size = ui.label_img->size();
	pixmap = musicManager.getRoundRectPixmap(pixmap, _size, 4);
	if (!pixmap.isNull()) {
		ui.label_img->setPixmap(pixmap);
	}
	//歌曲名，歌手，专辑名
	auto info = musicManager.getPlayingMusicInfo();
	ui.label_name->setText(info.musicName);
	ui.label_songer->setText(info.musicPlayer);
	ui.label_album->setText(info.musicAlbum);
}

void MusicInfoWidget::setLrcInfo(int position)
{
	auto beginIter = musicManager.getMusicLrcMap().begin();
	auto endIter = musicManager.getMusicLrcMap().end();
	auto iter = beginIter;
	while (iter != endIter)
	{
		QString key = iter.key();
		if (key.mid(1, 2).toInt() == (position / 60000) && (key.mid(4, 2)).toInt() == (position % 60000 / 1000)) {
			if (iter == beginIter)
				clearUI();
			auto lrc = iter.value();
			//当前歌词
			ui.label_lrc->setText(lrc);
			if (iter != beginIter) {
				//显示前三句
				auto lastIter = iter - 1;
				QString l1 = QString();
				QString l2 = QString();
				QString l3 = QString();
				if (lastIter != beginIter) {
					l1 = lastIter.value();
					lastIter--;
				}
				if (lastIter != beginIter) {
					l2 = lastIter.value();
					lastIter--;
				}
				if (lastIter != beginIter) {
					l3 = lastIter.value();
				}
				ui.label_l1->setText(l3);
				ui.label_l2->setText(l2);
				ui.label_l3->setText(l1);
			}
			if (iter != endIter) {
				//显示后三句
				auto nextIter = iter + 1;
				QString n1 = QString();
				QString n2 = QString();
				QString n3 = QString();
				if (nextIter != endIter) {
					n1 = nextIter.value();
					nextIter++;
				}
				if (nextIter != endIter) {
					n2 = nextIter.value();
					nextIter++;
				}
				if (nextIter != endIter) {
					n3 = nextIter.value();
				}
				ui.label_n1->setText(n1);
				ui.label_n2->setText(n2);
				ui.label_n3->setText(n3);
			}
		}
		iter++;
	}
}

void MusicInfoWidget::clearUI()
{
	ui.label_n1->clear();
	ui.label_n2->clear();
	ui.label_n3->clear();
	ui.label_l1->clear();
	ui.label_l2->clear();
	ui.label_l3->clear();
}
