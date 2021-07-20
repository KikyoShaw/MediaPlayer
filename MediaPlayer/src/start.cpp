#include "start.h"
#include "ui_start.h"
#include <QIcon>

Start::Start(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Start)
{
    ui->setupUi(this);

    //隐藏边框
    this->setWindowFlags(Qt::FramelessWindowHint);

    //设置窗口位置，大小
    this->setGeometry(50,50,800,600);

    //设置窗口图标
    QIcon icon;
    icon.addFile(QStringLiteral(":/new/image/new.ico"), QSize(), QIcon::Normal, QIcon::Off);
    this->setWindowIcon(icon);

    //加载视频操作
    Myplayer=new QMediaPlayer(this);
    video=new QVideoWidget(this);
    video->setAspectRatioMode(Qt::IgnoreAspectRatio);
    Myplayer->setVideoOutput(video);
    video->setFixedSize(800,600);
    Myplayer->setMedia(QUrl::fromLocalFile("./0008.mp4"));
    Myplayer->setVolume(100);
    Myplayer->play();

}

Start::~Start()
{
    delete ui;
    delete Myplayer;
    delete video;
}

void Start::mouseMoveEvent(QMouseEvent *e)
{
    if(e->buttons() & Qt::LeftButton)//鼠标左击才有效果
        move(e->globalPos()-point);//移动窗口
}

void Start::mousePressEvent(QMouseEvent *e)
{
    //鼠标事件中包含“按住的是左键”
    if(e->button()==Qt::LeftButton)
    {
        //获取移动的位移量
        point = e->globalPos()-frameGeometry().topLeft();
    }
}

//键盘响应结束开场动画
void Start::keyPressEvent(QKeyEvent *e)
{
    int key = e->key();
    if(key==Qt::Key_Escape)
    {
        int temp = QMessageBox::question(this,"提示","确定结束MV?",
                                         QMessageBox::Yes,QMessageBox::No);
        if(temp==QMessageBox::Yes)
        {
            Myplayer->setVolume(0);
            Myplayer->stop();
            this->close();
        }
        else if(temp==QMessageBox::No)
        {
            return;
        }
    }
}
