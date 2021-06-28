#include "mainwidget.h"
#include "ui_mainwidget.h"

QString sstr;

MainWidget::MainWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWidget)
{
    ui->setupUi(this);

    //去除边框
    this->setWindowFlags(Qt::FramelessWindowHint);

    //设置动态背景
    movie1 = new QMovie;
    movie1->setFileName(":/new/image/125.gif");
    ui->label->setMovie(movie1);
    movie1->start();
    ui->label->setScaledContents(true);

    //设置背景音乐
    player = new QMediaPlayer;
    player->setMedia(QUrl::fromLocalFile("./star.mp3"));
    player->setVolume(100);
    player->play();
}

MainWidget::~MainWidget()
{
    delete ui;
    delete movie1;
    delete player;
}

//视频播放器
void MainWidget::on_vedio_clicked()
{
    media = new MediaPlayer();
    connect(media,SIGNAL(mySignal()),this,SLOT(show()));
    connect(media,SIGNAL(mySignal()),this,SLOT(my_player()));
    media->show();
    player->stop();
    this->close();
}

//音乐播放器
void MainWidget::on_music_clicked()
{
    music = new musicplayer();
    connect(music,SIGNAL(mySignal()),this,SLOT(show()));
    connect(music,SIGNAL(mySignal()),this,SLOT(my_player()));
    music->show();
    player->stop();
    this->close();
}

//退出
void MainWidget::on_exit_clicked()
{
    int mess = QMessageBox::question(this,"提示","确定要退出？",QMessageBox::Yes,QMessageBox::No);
    if(mess==QMessageBox::Yes)
    {
        player->stop();
        exit(1);
    }
    else if(mess==QMessageBox::No)
    {
        return;
    }
}

//返回上一级
void MainWidget::on_help_clicked()
{
    player->stop();
    this->close();
    emit MySig();
}

void MainWidget::my_player()
{
    player->setVolume(100);
    player->play();
}

void MainWidget::paintEvent(QPaintEvent *)
{

}

void MainWidget::mouseMoveEvent(QMouseEvent *e)
{
    if(e->buttons() & Qt::LeftButton)//鼠标左击才有效果
         move(e->globalPos()-point);//移动窗口
}

void MainWidget::mousePressEvent(QMouseEvent *e)
{
    //鼠标事件中包含“按住的是左键”
    if(e->button()==Qt::LeftButton)
    {
        //获取移动的位移量
        point = e->globalPos()-frameGeometry().topLeft();
    }
}



















