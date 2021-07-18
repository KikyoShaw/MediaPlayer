#ifndef MUSICPLAYER_H
#define MUSICPLAYER_H

#include <QWidget>
#include <QMessageBox>
#include <QMediaPlayer>
#include <QMovie>
#include <QFileDialog>
#include <QFile>
#include <QFileInfo>
#include <QPainter>
#include <QTimer>
#include <QMediaPlaylist>
#include <QStringListIterator>
#include <QStandardItemModel>
#include <QDesktopWidget>
#include <QListWidget>
#include <QFileInfo>
#include <QLineEdit>
#include <QWebEngineView>
#include "customslider.h"
#include "start.h"

namespace Ui {
class musicplayer;
}

class MusicPlayer : public QWidget
{
    Q_OBJECT

public:
    explicit MusicPlayer(QWidget *parent = 0);
    ~MusicPlayer();

private slots:
    void on_flont_clicked();
    void on_open_clicked();
    void on_play_clicked();
    void slider_volume_changed();
    void slider_progress_clicked();
    void slider_progress_moved();
    void slider_progress_released();
    void onTimerOut();
    void myPlayerSlot(QModelIndex index);
    void on_List_clicked();
    void on_last_clicked();
    void on_next_clicked();
    void on_volum_clicked();
    void slotDurationChanged(qint64 duration);
    void slotPositionChanged(qint64 position);
    void on_lrc_clicked();
    void on_comboBox_activated(const QString name);
    void find();
    void findClick();
    void nofindClick();
    void on_find_clicked();
    void on_find_2_clicked();

signals:
    void mySignal();

protected:
    void paintEvent(QPaintEvent *);

private:
    Ui::musicplayer *ui;
    int play_state=0;//播放状态
    int lrcStatus; //歌词状态
    int findStatus=0; //查找状态
    int mvStatus=0; //mv播放状态
    int liveStatus=0;//在线状态
    QDialog *findDia; //查找框
    QDialog *lrcDia;//歌词框
    QPushButton *bu,*bu2,*bu3,*bu4;
    QPushButton *bu1,*bu12,*bu13,*bu14;
    QLineEdit *lineEdit,*lineEdit2,*lineEdit3,*lineEdit4;
    QLabel *lrclabel; //歌词label
    QString tmpPath;//记录当前添加歌曲的路径
    QString ss="";//记录当前目录
    QStringList list;
    QString file_name="abc";
    QFileInfo fi;
    QListWidget* listWidget;
    QListWidgetItem* lst1;
    CustomSlider * slider_volume;//音量控制slider
    bool state_slider_volume = false;//音量状态
    QStringList path;
    QStandardItemModel *songList;
    QMediaPlaylist *playList;
    QMediaPlayer *player;
    QTimer * timer;
    QMovie *movie1;
    Start *mv; //播放mv
    QWebEngineView *LiveView;//在线播放
};

#endif // MUSICPLAYER_H
