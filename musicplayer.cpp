#include "musicplayer.h"
#include "ui_musicplayer.h"

int MaxValue = 1000;//设置进度条的最大值
int num;

musicplayer::musicplayer(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::musicplayer)
{
    ui->setupUi(this);

    QIcon icon;
    icon.addFile(QStringLiteral(":/new/image/MusicPlayer.ico"), QSize(), QIcon::Normal, QIcon::Off);
    this->setWindowIcon(icon);

    //设置动态背景
    movie1 = new QMovie;
    movie1->setFileName(":/new/image/kkk.gif");
    ui->textlabel->setMovie(movie1);
    movie1->start();
    ui->textlabel->setScaledContents(true);

    //刚进去禁用按钮
    ui->volum->setEnabled(false);
    ui->last->setEnabled(false);
    ui->next->setEnabled(false);
    ui->find_2->setEnabled(false);
    ui->find->setEnabled(false);
    ui->lrc->setEnabled(false);
    ui->comboBox->setEnabled(false);
    ui->play->setEnabled(false);

    //手动设置slider_volume 包括初始化 方向 禁用，以及槽函数
    slider_volume = new CustomSlider(this);
    slider_volume->setOrientation(Qt::Vertical);
    slider_volume->setEnabled(false);
    slider_volume->hide();
    //由于不涉及到slider值的刷新，因此只需对move和自定义click两个信号进行处理，并且可以共用一个槽函数
    connect(slider_volume,&CustomSlider::costomSliderClicked,this,&musicplayer::slider_volume_changed);
    connect(slider_volume,&CustomSlider::sliderMoved,this,&musicplayer::slider_volume_changed);

    //禁用slider_progress，连接信号槽(进度条)
    ui->slider_progress->setEnabled(false);
    connect(ui->slider_progress,&CustomSlider::costomSliderClicked,this,&musicplayer::slider_progress_clicked);
    connect(ui->slider_progress,&CustomSlider::sliderMoved,this,&musicplayer::slider_progress_moved);
    connect(ui->slider_progress,&CustomSlider::sliderReleased,this,&musicplayer::slider_progress_released);

    //设置播放器
    player = new QMediaPlayer;

    //设置定时器
    timer = new QTimer();
    timer->setInterval(1000);
    //将timer连接至onTimerOut槽函数
    connect(timer, SIGNAL(timeout()), this, SLOT(onTimerOut()));

    //初始化
    playList = new QMediaPlaylist;
    songList = new QStandardItemModel;

    //歌词显示模块
     QDesktopWidget* desktopWidget = QApplication::desktop();
      //获取可用桌面大小
    QRect deskRect = desktopWidget->availableGeometry();
    lrcDia = new QDialog();
    lrclabel = new QLabel(lrcDia);
    lrcDia->setWindowFlags(Qt::FramelessWindowHint);  //把窗口设置成没框架的
    lrcDia->setAttribute(Qt::WA_TranslucentBackground, true);
    lrcDia->setGeometry(deskRect.width()/2-286,deskRect.height()-160,572,100);
    lrclabel->setGeometry(0,0,572,100);
    lrcDia->setStyleSheet("background-color: rgba(255, 255, 255, 0);");
    lrclabel->setStyleSheet("background-color: rgba(255, 255, 255, 0);color: rgb(30, 64, 255);");
    lrclabel->setFont(QFont("华文彩云",32));
    lrclabel->setAlignment(Qt::AlignCenter);
    lrclabel->setText("杨桃音乐  倾听世界的美");
    lrclabel->show();

    connect(player,SIGNAL(durationChanged(qint64)),this,SLOT(slotDurationChanged(qint64)));
    connect(player,SIGNAL(positionChanged(qint64)),this,SLOT(slotPositionChanged(qint64)));

    //隐藏歌单
    ui->sList->hide();
    ui->textlabel->hide();
}

musicplayer::~musicplayer()
{
    delete ui;
    delete player;
    delete timer;
    delete lrcDia;
    delete lrclabel;
    delete playList;
    delete songList;
    delete findDia;
    delete LiveView;
}

/*
void musicplayer::loadLink(const QUrl &url){
    ui->webView->load(url);
}
*/

void musicplayer::slotDurationChanged(qint64 duration){
    int second = duration/60000;
    int miao = duration%60000/1000;
    if(miao<10&&miao>-0)
     {
        ui->AllTime->setText("0"+QString::number(second)+":"+"0"+QString::number(miao));
     }
    else
        ui->AllTime->setText("0"+QString::number(second)+":"+QString::number(miao));
}

void musicplayer::slotPositionChanged(qint64 position){
    int sec = position/60000;
    int mia = position%60000/1000;

    ui->label->setText( playList->currentMedia().canonicalUrl().fileName());

    QFile file( ss +playList->currentMedia().canonicalUrl().fileName().split(".").front() + ".lrc");
    //qDebug() << ss +playList->currentMedia().canonicalUrl().fileName().split(".").front() + ".lrc";
     if (file.open(QIODevice::ReadOnly|QIODevice::Text)) {
        QTextStream stream(&file);
        QString line;
         while ( !stream.atEnd()) {
            line = stream.readLine();
           // qDebug() << line;
            if(line.mid(1,2).toInt()==(position/60000)&&(line.mid(4,2)).toInt()==(position%60000/1000))
            {
               lrclabel->setText(line.split("]").last());
            }
            file.close();
     }
 }else{
         lrclabel->setText("未找到相应歌词！");
     }

    if(mia<10&&mia>-0)
     {
        ui->StartTime->setText("0"+QString::number(sec)+":"+"0"+QString::number(mia));
     }
    else
        ui->StartTime->setText("0"+QString::number(sec)+":"+QString::number(mia));
}

//返回上一级
void musicplayer::on_flont_clicked()
{
    player->stop();
    this->close();
    emit mySignal();
}

//网络播放
void musicplayer::on_open_clicked()
{
    if(liveStatus==0)
    {
        player->pause();
        ui->play->setText("播放");
        //网络播放初始化
        LiveView = new QWebEngineView;
        LiveView->setAttribute(Qt::WA_DeleteOnClose);
        LiveView->load(QUrl("http://music.163.com"));
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

void musicplayer::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    p.drawPixmap(0,0,this->width(),this->height(),QPixmap(":/new/image/114.jpg"));
}

//播放
void musicplayer::on_play_clicked()
{
    if(play_state==0)
    {
        player->pause();
        ui->play->setText("播放");
        play_state=1;
    }
    else
    {
        player->play();
        ui->play->setText("暂停");
        play_state=0;
    }

}

//音量
void musicplayer::on_volum_clicked()
{
    //通过hide()和show()方法，实现音量控制Slider的唤出和收起
    qDebug()<<state_slider_volume;
    if(state_slider_volume)
    {
        slider_volume->hide();
    }
    else
    {
        slider_volume->setValue(player->volume());
        //计算位置，使其位于音量控制按钮的上方

        slider_volume->setGeometry(QRect(ui->volum->pos().rx()+0.5*ui->volum->width()-15,
                                         ui->volum->y()-100 , 30, 100));
        slider_volume->show();
    }
    state_slider_volume = !state_slider_volume;
}

//音量控制Slider的槽函数
void musicplayer::slider_volume_changed()
{
    player->setVolume(slider_volume->value());
}

//播放进度控制Slider的槽函数
void musicplayer::slider_progress_clicked()
{
    player->setPosition(ui->slider_progress->value()*player->duration()/MaxValue);
}

void musicplayer::slider_progress_moved()
{
    //暂时停止计时器，在用户拖动过程中不修改slider的值
    timer->stop();
    player->setPosition(ui->slider_progress->value()*player->duration()/MaxValue);
}

void musicplayer::slider_progress_released()
{
    //用户释放滑块后，重启定时器
    timer->start();
}

void musicplayer::onTimerOut()
{
    ui->slider_progress->setValue(player->position()*MaxValue/player->duration());
}

void musicplayer::myPlayerSlot(QModelIndex index)
{
    //启用slider并设置范围
    ui->slider_progress->setEnabled(true);
    ui->slider_progress->setRange(0,MaxValue);
    timer->start();
    int m = index.row();
    playList->setCurrentIndex(m);
    player->play();

    //启用播放/暂停按钮，并将其文本设置为“暂停”
    ui->play->setEnabled(true);
    ui->play->setText("暂停");
    play_state = 0;

    //启用音量控制slider
    slider_volume->setEnabled(true);
    slider_volume->setRange(0, 100);

    //启用按钮
    ui->volum->setEnabled(true);
    ui->last->setEnabled(true);
    ui->next->setEnabled(true);
    ui->find_2->setEnabled(true);
    ui->find->setEnabled(true);
    ui->lrc->setEnabled(true);
    ui->comboBox->setEnabled(true);
    ui->play->setEnabled(true);

}

//显示歌单导入歌曲
void musicplayer::on_List_clicked()
{
    //歌单模块
    songList->clear();
    songList->setHorizontalHeaderItem(0,new QStandardItem(QObject::tr("歌曲列表")));
    ui->sList->setModel(songList);
    ui->sList->setColumnWidth(0,390);
    ui->sList->verticalHeader()->hide();
    ui->sList->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->sList->setSelectionBehavior(QAbstractItemView::SelectRows);
    connect(ui->sList,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(myPlayerSlot(QModelIndex)));

    //播放列表模块
    playList->clear();

    timer->stop();
    player->stop();
    path = QFileDialog::getOpenFileNames(this,"选择歌曲",".","(*.mp3)");
    int i=0;
    QString addSongs;
    QListIterator<QString>mylist(path);
    while(mylist.hasNext())
    {
        addSongs = mylist.next();
        QFileInfo file;
        file = QFileInfo(addSongs);
        QString fileName = file.fileName();
        songList->setItem(i,0,new QStandardItem(fileName));
        playList->addMedia(QUrl::fromLocalFile(addSongs));
        i++;
        num++;
    }
    playList->setPlaybackMode(QMediaPlaylist::Loop);
    player->setPlaylist(playList);
    ui->sList->show();
    ui->textlabel->show();
    QString PathString;
    for(int j=0; j<path.size(); ++j)
    {
        PathString=QDir::toNativeSeparators(path.at(j)); //把斜杠转化成反斜杠
        if(!PathString.isEmpty())
        {
            file_name=PathString.split("\\").last();//用斜杠分开，取最后一个名字
        }
    }
    if(PathString.contains("\\"))
    {
      tmpPath = PathString.replace("\\","/",Qt::CaseInsensitive);
      list = tmpPath.split("/");
      for(int i = 0;;i++){
          if(list.at(i)==tmpPath.split("/").last()){
              for(int j = 0; j < i; j++){
                  ss = ss + list.at(j) + "/";
                }
                return;
          }
        }
    }
}

//上一曲
void musicplayer::on_last_clicked()
{
    if(playList->currentIndex()!=0)
    {
        playList->setCurrentIndex(playList->previousIndex());
        player->play();
    }
    else
    {
        playList->setCurrentIndex(playList->mediaCount()-1);
        player->play();
    }
}

//下一曲
void musicplayer::on_next_clicked()
{
    if(playList->mediaCount()-playList->currentIndex()!=1)
    {
        playList->setCurrentIndex(playList->nextIndex());
        player->play();
    }
    else
    {
        playList->setCurrentIndex(0);
        player->play();
    }
}

//歌词显示
void musicplayer::on_lrc_clicked()
{
    if(lrcStatus==0){
        lrcDia->show();
        lrcStatus=1;
    }else{
        lrcDia->close();
        lrcStatus=0;
    }
}

//播放模式
void musicplayer::on_comboBox_activated(const QString name)
{
    if(name==tr("顺序播放"))
        {
            playList->setPlaybackMode(QMediaPlaylist::Sequential);//顺序播放
        }
    else if(name==tr("随机播放"))
        {
            playList->setPlaybackMode(QMediaPlaylist::Random); //随机播放
        }
    else if(name==tr("列表循环"))
    {
             playList->setPlaybackMode(QMediaPlaylist::Loop); //列表循环
    }
    else if(name==tr("单曲循环"))
    {
            playList->setPlaybackMode(QMediaPlaylist::CurrentItemInLoop); //单曲循环
    }
}

//查找(用来搜索歌曲列表的歌)
void musicplayer::on_find_clicked()
{
    if(findStatus==0){
        find();
        findStatus++;
    }else{
        if(findDia->isVisible()!=true){
            find();
        }
    }
}

void musicplayer::find()
{
    findDia = new QDialog();
    bu = new QPushButton("查找",findDia);
    bu1 = new QPushButton("取消",findDia);
    lineEdit = new QLineEdit("请输入歌曲名",findDia);

    findDia->setGeometry(538,400,291,61);
    bu->setGeometry(-6,30,151,31);
    bu1->setGeometry(144,30,151,31);
    lineEdit->setGeometry(0,0,291,31);
    findDia->show();
    lineEdit->show();
    connect(bu,SIGNAL(clicked()),this,SLOT(findClick()));
    connect(bu1,SIGNAL(clicked()),this,SLOT(nofindClick()));
}

//查找功能实现
void musicplayer::findClick()
{
    int go = 0;
    if(lineEdit->text()!="请输入歌曲名"){
        for(int i=0 ; i < num ; i++){
            if(lineEdit->text()==songList->item(i)->text().split(".").front()){
                findDia->close();
                go ++;
                QMessageBox::information(this,"提示","您查找的歌曲在歌曲列表!");
                return;
                }
            }
                if(go==0)
            {
                QMessageBox::information(this,"提示","没有找到您要查找的歌曲,请重试!");
                lineEdit->setText("请输入歌曲名!");
                return;
            }
    }
}

//取消查找
void musicplayer::nofindClick()
{
    findDia->close();
}

//播放mv
void musicplayer::on_find_2_clicked()
{

    if(mvStatus==0)
    {
        player->pause();
        ui->play->setText("播放");
        mv = new Start();
        mv->setAttribute(Qt::WA_DeleteOnClose);
        mv->show();
        mvStatus=1;
    }else if(mvStatus==1)
    {
        mv->Myplayer->stop();
        mv->close();
        player->play();
        mvStatus=0;
    }
}
