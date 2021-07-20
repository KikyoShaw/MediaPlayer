#include "sql.h"
#include "ui_sql.h"

int tt=1;

Sql::Sql(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Sql)
{
    ui->setupUi(this);

    //���ô���ͼ��
    QIcon icon;
    icon.addFile(QStringLiteral(":/new/image/Player.ico"), QSize(), QIcon::Normal, QIcon::Off);
    this->setWindowIcon(icon);
    //͸����
    ui->pushButton->setFlat(true);
    ui->pushButton_2->setFlat(true);
    //��ʼ��
    model = new QSqlTableModel(this);
    model->setTable("game");
    user = new UserSql;
    connect(user,SIGNAL(mySing()),this,SLOT(show()));

    //�������벻�ɼ�
    ui->passEdit->setEchoMode(QLineEdit::Password);
    ui->passEdit->setPlaceholderText("����������:");
    ui->lineEdit->setPlaceholderText("�������û���:");

    //����һ��ɾ������
    ui->lineEdit->setClearButtonEnabled(true);
    ui->passEdit->setClearButtonEnabled(true);

    ui->tableView->hide();



}

Sql::~Sql()
{
    delete ui;
    delete model;
}

void Sql::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    p.drawPixmap(0,0,this->width(),this->height(),QPixmap(":/new/image/beijing2.jpeg"));
  //  this->setCursor(QCursor(QPixmap(":/new/image/777.png")));
}

void Sql::mouseMoveEvent(QMouseEvent *e)
{
    if(e->buttons() & Qt::LeftButton)//����������Ч��
         move(e->globalPos()-point);//�ƶ�����
}

void Sql::mousePressEvent(QMouseEvent *e)
{
    //����¼��а�������ס���������
    if(e->button()==Qt::LeftButton)
    {
        //��ȡ�ƶ���λ����
        point = e->globalPos()-frameGeometry().topLeft();
    }
}

//������Ӧ��¼
void Sql::keyPressEvent(QKeyEvent *e)
{
    int key = e->key();
    if(key==16777220)
    {
        if((ui->lineEdit->text())==NULL||(ui->passEdit->text())==NULL)
        {
            QMessageBox::information(this,("��ʾ"),("�û����������벻��Ϊ�գ�"));
        }
        else
        {
            qDebug()<<key;
            query=new QSqlQuery;
            bool ok = query->exec("select * from game");
            if(!ok)
            {
                QMessageBox::critical(this,"Error",query->lastError().text());
                return;
            }
            else
            {
                while(query->next())
                {
                    qDebug()<<query->value("number").toString();
                    qDebug()<<query->value("password").toString();
                    QString str1 = ui->lineEdit->text();
                    QString str2 = ui->passEdit->text();
                    qDebug()<<str1;
                    if(str1==(query->value("number").toString())
                            &&str2==(query->value("password").toString()))
                    {
                        tt=0;
                        int mes = QMessageBox::information(this,("��ʾ"),("��¼�ɹ�!"));
                        if(mes == QMessageBox::Ok)
                        {
                            widget = new MainWidget();
                            widget->show();
                            connect(widget,SIGNAL(MySig()),this,SLOT(show()));
                            this->close();
                            break;
                        }
                    }
                }
                if(tt==1)
                {
                    QMessageBox::information(this,("��ʾ"),("�û����������벻��ȷ!"));
                }
            }
        }
    }
}

//ע�����˺�
void Sql::on_pushButton_2_clicked()
{
    this->close();
    user->show();
}

//��¼
void Sql::on_pushButton_clicked()
{
//    QString str11 = "'"+ui->lineEdit->text()+"'";
//    qDebug()<<str11;
//    model->setFilter("number="+str11);
//    model->select();
//    qDebug()<<model;
//    ui->tableView->show();
//    ui->tableView->setModel(model);
    if((ui->lineEdit->text())==NULL||(ui->passEdit->text())==NULL)
    {
        QMessageBox::information(this,("��ʾ"),("�û����������벻��Ϊ�գ�"));
    }
    else
    {
        query=new QSqlQuery;
        bool ok = query->exec("select * from game");
        if(!ok)
        {
            QMessageBox::critical(this,"Error",query->lastError().text());
            return;
        }
        else
        {
            while(query->next())
            {
                qDebug()<<query->value("number").toString();
                qDebug()<<query->value("password").toString();
                QString str1 = ui->lineEdit->text();
                QString str2 = ui->passEdit->text();
                qDebug()<<str1;
                if(str1==(query->value("number").toString())
                        &&str2==(query->value("password").toString()))
                {
                    tt=0;
                    QMessageBox::information(this,("��ʾ"),("��¼�ɹ�!"));

                    widget = new MainWidget();
                    widget->show();
                    connect(widget,SIGNAL(MySig()),this,SLOT(show()));
                    this->close();
                    break;
                    break;
                }
            }
            if(tt==1)
            {
                QMessageBox::information(this,("��ʾ"),("�û����������벻��ȷ!"));
            }
        }
    }
}

