#ifndef MEDIAPLAYER_H
#define MEDIAPLAYER_H

#include <QMainWindow>
#include <QMediaPlayer>
#include <QVBoxLayout>
#include <QVideoWidget>
#include <QFileDialog>
#include <QFile>
#include <QMessageBox>
#include <QTimer>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QWebEngineView>
#include <QWebEngineSettings>
#include "customslider.h"

namespace Ui {
class MediaPlayer;
}

class MediaPlayer : public QMainWindow
{
    Q_OBJECT

public:
    explicit MediaPlayer(QWidget *parent = 0);
    ~MediaPlayer();
    QPoint point;
    //������Ƶ��ȫ�ֱ���
    QVBoxLayout* layout_video;
    QMediaPlayer* player;
    QVideoWidget* vWidget;
    //�л���Ƶ��־λ
    static int play_flag;

    bool status = false;

private slots:
    void on_pushButton_open_file_clicked();
    void on_pushButton_play_and_pause_clicked();
    void onTimerOut();
    void slider_progress_clicked();
    void slider_released();
    void slider_progress_moved();
    void slider_progress_released();
    void on_pushButton_volume_clicked();
    void slider_volume_changed();
    void on_pushButton_flont_clicked();

    void on_pushButton_live_clicked();

signals:
    void mySignal();


protected:
//    void mouseMoveEvent(QMouseEvent *);
//    void mousePressEvent(QMouseEvent *);
//    void keyPressEvent(QKeyEvent *);

private:
    Ui::MediaPlayer *ui;
    int liveStatus=0;//����״̬
    QWebEngineView *LiveView;//���߲���
};

#endif // MEDIAPLAYER_H
