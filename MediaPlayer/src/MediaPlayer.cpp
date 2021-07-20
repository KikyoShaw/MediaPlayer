#include "mediaplayer.h"
#include "ui_mediaplayer.h"

//����״̬��trueΪ���ţ�falseΪ��ͣ
bool play_state;

//��Slider�йصĲ��ſ��Ʊ���
QTimer * timer;
int maxValue = 1000;//���ý����������ֵ

//��������slider
CustomSlider * slider_volume;

MediaPlayer::MediaPlayer(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MediaPlayer)
{
    ui->setupUi(this);

    //���ô���ͼ��
    QIcon icon;
    icon.addFile(QStringLiteral(":/new/image/MediaPlayer.ico"), QSize(), QIcon::Normal, QIcon::Off);
    this->setWindowIcon(icon);
    //���ر߿�
  //  this->setWindowFlags(Qt::FramelessWindowHint);
    ui->pushButton_play_and_pause->setEnabled(false);
    ui->pushButton_volume->setEnabled(false);
    //����slider_progress�������źŲ�
    ui->slider_progress->setEnabled(false);
    connect(ui->slider_progress,&CustomSlider::costomSliderClicked,this,&MediaPlayer::slider_progress_clicked);
    connect(ui->slider_progress,&CustomSlider::sliderMoved,this,&MediaPlayer::slider_progress_moved);
    connect(ui->slider_progress,&CustomSlider::sliderReleased,this,&MediaPlayer::slider_progress_released);
    //�ֶ�����slider_volume ������ʼ�� ���� ���ã��Լ��ۺ���
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
    //���ڲ��漰��sliderֵ��ˢ�£����ֻ���move���Զ���click�����źŽ��д������ҿ��Թ���һ���ۺ���
    connect(slider_volume,&CustomSlider::costomSliderClicked,this,&MediaPlayer::slider_volume_changed);
    connect(slider_volume,&CustomSlider::sliderMoved,this,&MediaPlayer::slider_volume_changed);
    connect(slider_volume,&CustomSlider::sliderReleased,this,&MediaPlayer::slider_released);


    //ʵ��������ȫ�ֱ���
    layout_video = new QVBoxLayout;
   // layout_video->setMargin(1);
    player = new QMediaPlayer;
    vWidget = new QVideoWidget;
    //���ö�ʱ��
    timer = new QTimer();
    timer->setInterval(1000);
    //��timer������onTimerOut�ۺ���
    connect(timer, SIGNAL(timeout()), this, SLOT(onTimerOut()));

    ui->pushButton->hide();

}

int MediaPlayer::play_flag = 0;

//���Ž��ȿ���Slider�Ĳۺ���
void MediaPlayer::slider_progress_clicked()
{
    player->setPosition(ui->slider_progress->value()*player->duration()/maxValue);
}

void MediaPlayer::slider_progress_moved()
{
    //��ʱֹͣ��ʱ�������û��϶������в��޸�slider��ֵ
    timer->stop();
    player->setPosition(ui->slider_progress->value()*player->duration()/maxValue);
}

void MediaPlayer::slider_progress_released()
{
    //�û��ͷŻ����������ʱ��
    timer->start();
}

//��������Slider�Ĳۺ���
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
    //����label_player�ߴ����ò�������
    vWidget->resize(ui->label_player->size());
    layout_video->addWidget(vWidget);
    ui->label_player->setLayout(layout_video);
    player->setVideoOutput(vWidget);

    //ѡ����Ƶ�ļ�
    QString filename = QFileDialog::getOpenFileName(this,tr("ѡ����Ƶ�ļ�"),".",tr("��Ƶ��ʽ(*.avi *.mp4 *.flv *.mkv)"));
    QFile file(filename);
    if(!file.open(QIODevice::ReadOnly))
    {
        QMessageBox::information(NULL, "��ʾ", "������ѡ����Ƶ�ļ�", QMessageBox::Ok, QMessageBox::Ok);
        return;
    }

    //���ò�����
    player->setMedia(QUrl::fromLocalFile(filename));
    //play_stateΪtrue��ʾ���ţ�false��ʾ��ͣ
    play_state = true;
    //���ò���/��ͣ��ť���������ı�����Ϊ����ͣ��
    ui->pushButton_play_and_pause->setEnabled(true);
    ui->pushButton_play_and_pause->setText("��ͣ");
    //����������
    player->play();
    play_flag=1;

    //����slider�����÷�Χ
    ui->slider_progress->setEnabled(true);
    ui->slider_progress->setRange(0,maxValue);
    timer->start();

    //������������slider
    slider_volume->setEnabled(true);
    slider_volume->setRange(0, 100);
    //����������ť
    ui->pushButton_volume->setEnabled(true);
}

void MediaPlayer::on_pushButton_play_and_pause_clicked()
{
    //��ת����״̬
    if(play_state)
    {
        player->pause();
        ui->pushButton_play_and_pause->setText("����");
    }
    else
    {
        player->play();
        ui->pushButton_play_and_pause->setText("��ͣ");
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
    //ͨ��hide()��show()������ʵ����������Slider�Ļ���������
    if(state_slider_volume)
    {
        slider_volume->hide();
    }
    else
    {
        slider_volume->setValue(player->volume());
        //����λ�ã�ʹ��λ���������ư�ť���Ϸ�
        slider_volume->setGeometry(QRect(ui->pushButton_volume->pos().rx()+0.5*ui->pushButton_volume->width()-15,
                                         ui->pushButton_volume->y()-100 , 30, 102));
        slider_volume->show();
    }
    state_slider_volume = !state_slider_volume;
}
/*
void MediaPlayer::mouseMoveEvent(QMouseEvent *e)
{
    if(e->buttons() & Qt::LeftButton)//����������Ч��
        move(e->globalPos()-point);//�ƶ�����
}

void MediaPlayer::mousePressEvent(QMouseEvent *e)
{
    //����¼��а�������ס���������
    if(e->button()==Qt::LeftButton)
    {
        //��ȡ�ƶ���λ����
        point = e->globalPos()-frameGeometry().topLeft();
    }
}

void MediaPlayer::keyPressEvent(QKeyEvent *e)
{
    int key = e->key();
    if(key==Qt::Key_Escape)
    {
        int temp = QMessageBox::question(this,"��ʾ","ȷ���˳���",
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

//����
void MediaPlayer::on_pushButton_live_clicked()
{
    if(liveStatus==0)
    {
        player->stop();
        ui->pushButton_play_and_pause->setText("����");
        //���粥�ų�ʼ��
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
