#include "mediaplayer.h"
#include "ui_mediaplayer.h"

//播放状态，true为播放，false为暂停
bool play_state;

//与Slider有关的播放控制变量
QTimer * timer;
int maxValue = 1000;//设置进度条的最大值

//音量控制slider
CustomSlider * slider_volume;

MediaPlayer::MediaPlayer(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MediaPlayer)
{
    ui->setupUi(this);

    //设置窗口图标
    QIcon icon;
    icon.addFile(QStringLiteral(":/new/image/MediaPlayer.ico"), QSize(), QIcon::Normal, QIcon::Off);
    this->setWindowIcon(icon);
    //隐藏边框
  //  this->setWindowFlags(Qt::FramelessWindowHint);
    ui->pushButton_play_and_pause->setEnabled(false);
    ui->pushButton_volume->setEnabled(false);
    //禁用slider_progress，连接信号槽
    ui->slider_progress->setEnabled(false);
    connect(ui->slider_progress,&CustomSlider::costomSliderClicked,this,&MediaPlayer::slider_progress_clicked);
    connect(ui->slider_progress,&CustomSlider::sliderMoved,this,&MediaPlayer::slider_progress_moved);
    connect(ui->slider_progress,&CustomSlider::sliderReleased,this,&MediaPlayer::slider_progress_released);
    //手动设置slider_volume 包括初始化 方向 禁用，以及槽函数
    slider_volume = new CustomSlider(this);
    slider_volume->setOrientation(Qt::Vertical);
    slider_volume->setEnabled(false);
    /*slider_volume->setStyleSheet("QSlider {background:rgba(52,7,91,100%);border-color:rgba(52,7,91,100%)}"
                                 "QSlider::groove:vertical {background:#cbcbcb;width:6px;border-radius:1px;padding-left:-1px;padding-right:-1px;padding-top:-1px;padding-bottom:-1px;}"
                                 "QSlider::sub-page:vertical{ background:#cbcbcb;border-radius:2px;}"
                                 "QSlider::add-page:vertical {  background: qlineargradient(x1 : 0, y1 : 0, x2 : 0, y2 : 1,stop : 0 #439cf4, stop:1 #439cf4);background: qlineargradient(x1 : 0, y1 : 0.2, x2 : 1, y2 : 1,stop : 0 #439cf4, stop: 1 #439cf4);width:10px;border-radius:2px;}"
                                 "QSlider::handle:vertical {margin: -2px - 7px - 2px - 7px;height: 17px;}"
                             );*/
    slider_volume->hide();
    //由于不涉及到slider值的刷新，因此只需对move和自定义click两个信号进行处理，并且可以共用一个槽函数
    connect(slider_volume,&CustomSlider::costomSliderClicked,this,&MediaPlayer::slider_volume_changed);
    connect(slider_volume,&CustomSlider::sliderMoved,this,&MediaPlayer::slider_volume_changed);
    connect(slider_volume,&CustomSlider::sliderReleased,this,&MediaPlayer::slider_released);


    //实例化三个全局变量
    layout_video = new QVBoxLayout;
   // layout_video->setMargin(1);
    player = new QMediaPlayer;
    vWidget = new QVideoWidget;
    //设置定时器
    timer = new QTimer();
    timer->setInterval(1000);
    //将timer连接至onTimerOut槽函数
    connect(timer, SIGNAL(timeout()), this, SLOT(onTimerOut()));

    ui->pushButton->hide();

}

int MediaPlayer::play_flag = 0;

//播放进度控制Slider的槽函数
void MediaPlayer::slider_progress_clicked()
{
    player->setPosition(ui->slider_progress->value()*player->duration()/maxValue);
}

void MediaPlayer::slider_progress_moved()
{
    //暂时停止计时器，在用户拖动过程中不修改slider的值
    timer->stop();
    player->setPosition(ui->slider_progress->value()*player->duration()/maxValue);
}

void MediaPlayer::slider_progress_released()
{
    //用户释放滑块后，重启定时器
    timer->start();
}

//音量控制Slider的槽函数
void MediaPlayer::slider_volume_changed()
{
    qDebug() << "slider_volume->value() = " << slider_volume->value();
    if(status == true)
    {
        ui->pushButton->show();
    }
    else
    {
       ui->pushButton->hide();
    }
    player->setVolume(slider_volume->value());
    ui->pushButton->setText(QString("%1").arg(slider_volume->value()));
}

void MediaPlayer::slider_released()
{
    status == false;
    if(status == true)
    {
        ui->pushButton->show();
    }
    else
    {
       ui->pushButton->hide();
    }
}


MediaPlayer::~MediaPlayer()
{
    delete ui;
}

void MediaPlayer::on_pushButton_open_file_clicked()
{
    if(play_flag==1)
    {
        player->stop();
        timer->stop();
    }

    layout_video->setMargin(1);
    //根据label_player尺寸设置播放区域
    vWidget->resize(ui->label_player->size());
    layout_video->addWidget(vWidget);
    ui->label_player->setLayout(layout_video);
    player->setVideoOutput(vWidget);

    //选择视频文件
    QString filename = QFileDialog::getOpenFileName(this,tr("选择视频文件"),".",tr("视频格式(*.avi *.mp4 *.flv *.mkv)"));
    QFile file(filename);
    if(!file.open(QIODevice::ReadOnly))
    {
        QMessageBox::information(NULL, "提示", "请重新选择视频文件", QMessageBox::Ok, QMessageBox::Ok);
        return;
    }

    //设置播放器
   // player->setMedia(QUrl::fromLocalFile(filename));
	player->setMedia(QUrl::fromLocalFile("http://fs.mv.web.kugou.com/202107270955/5018e19c140d5adf019a877ae12f31b2/G031/M08/12/1D/_5MEAFXd0KSAZMLpARSo2wZan7k579.mp4"));
    //play_state为true表示播放，false表示暂停
    play_state = true;
    //启用播放/暂停按钮，并将其文本设置为“暂停”
    ui->pushButton_play_and_pause->setEnabled(true);
    ui->pushButton_play_and_pause->setText("暂停");
    //播放器开启
    player->play();
    play_flag=1;

    //启用slider并设置范围
    ui->slider_progress->setEnabled(true);
    ui->slider_progress->setRange(0,maxValue);
    timer->start();

    //启用音量控制slider
    slider_volume->setEnabled(true);
    slider_volume->setRange(0, 100);
    //启用音量按钮
    ui->pushButton_volume->setEnabled(true);
}

void MediaPlayer::on_pushButton_play_and_pause_clicked()
{
    //反转播放状态
    if(play_state)
    {
        player->pause();
        ui->pushButton_play_and_pause->setText("播放");
    }
    else
    {
        player->play();
        ui->pushButton_play_and_pause->setText("暂停");
    }

    play_state = !play_state;
}

void MediaPlayer::onTimerOut()
{
    ui->slider_progress->setValue(player->position()*maxValue/player->duration());
}


bool state_slider_volume = false;
void MediaPlayer::on_pushButton_volume_clicked()
{
    qDebug()<<state_slider_volume;
    //通过hide()和show()方法，实现音量控制Slider的唤出和收起
    if(state_slider_volume)
    {
        slider_volume->hide();
    }
    else
    {
        slider_volume->setValue(player->volume());
        //计算位置，使其位于音量控制按钮的上方
        slider_volume->setGeometry(QRect(ui->pushButton_volume->pos().rx()+0.5*ui->pushButton_volume->width()-15,
                                         ui->pushButton_volume->y()-100 , 30, 102));
        slider_volume->show();
    }
    state_slider_volume = !state_slider_volume;
}
/*
void MediaPlayer::mouseMoveEvent(QMouseEvent *e)
{
    if(e->buttons() & Qt::LeftButton)//鼠标左击才有效果
        move(e->globalPos()-point);//移动窗口
}

void MediaPlayer::mousePressEvent(QMouseEvent *e)
{
    //鼠标事件中包含“按住的是左键”
    if(e->button()==Qt::LeftButton)
    {
        //获取移动的位移量
        point = e->globalPos()-frameGeometry().topLeft();
    }
}

void MediaPlayer::keyPressEvent(QKeyEvent *e)
{
    int key = e->key();
    if(key==Qt::Key_Escape)
    {
        int temp = QMessageBox::question(this,"提示","确定退出？",
                                         QMessageBox::Yes,QMessageBox::No);
        if(temp==QMessageBox::Yes)
        {
            timer->stop();
            player->setVolume(0);
            player->stop();
            this->close();
        }
        else if(temp==QMessageBox::No)
        {
            return;
        }
    }
}
*/

void MediaPlayer::on_pushButton_flont_clicked()
{
    if(play_flag==1)
    {
        player->stop();
        timer->stop();
    }
    this->close();
    emit mySignal();
}

//在线
void MediaPlayer::on_pushButton_live_clicked()
{
    if(liveStatus==0)
    {
        player->stop();
        ui->pushButton_play_and_pause->setText("播放");
        //网络播放初始化
        LiveView = new QWebEngineView;
        LiveView->settings()->setAttribute(QWebEngineSettings::PluginsEnabled, true);
        LiveView->setAttribute(Qt::WA_DeleteOnClose);
        LiveView->load(QUrl("https://v.qq.com/"));
        LiveView->resize(1024, 768);
        LiveView->show();
        liveStatus=1;
    }else if(liveStatus==1)
    {
       // LiveView->reload();
        LiveView->close();
        player->stop();
        liveStatus=0;
    }
}
