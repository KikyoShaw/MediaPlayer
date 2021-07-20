#include "start.h"
#include "ui_start.h"
#include <QIcon>

Start::Start(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Start)
{
    ui->setupUi(this);

    //���ر߿�
    this->setWindowFlags(Qt::FramelessWindowHint);

    //���ô���λ�ã���С
    this->setGeometry(50,50,800,600);

    //���ô���ͼ��
    QIcon icon;
    icon.addFile(QStringLiteral(":/new/image/new.ico"), QSize(), QIcon::Normal, QIcon::Off);
    this->setWindowIcon(icon);

    //������Ƶ����
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
    if(e->buttons() & Qt::LeftButton)//����������Ч��
        move(e->globalPos()-point);//�ƶ�����
}

void Start::mousePressEvent(QMouseEvent *e)
{
    //����¼��а�������ס���������
    if(e->button()==Qt::LeftButton)
    {
        //��ȡ�ƶ���λ����
        point = e->globalPos()-frameGeometry().topLeft();
    }
}

//������Ӧ������������
void Start::keyPressEvent(QKeyEvent *e)
{
    int key = e->key();
    if(key==Qt::Key_Escape)
    {
        int temp = QMessageBox::question(this,"��ʾ","ȷ������MV?",
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
