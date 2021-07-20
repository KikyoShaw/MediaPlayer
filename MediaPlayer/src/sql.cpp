#include "sql.h"
#include "ui_sql.h"

int tt=1;

Sql::Sql(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Sql)
{
    ui->setupUi(this);

    //设置窗口图标
    QIcon icon;
    icon.addFile(QStringLiteral(":/new/image/Player.ico"), QSize(), QIcon::Normal, QIcon::Off);
    this->setWindowIcon(icon);
    //透明化
    ui->pushButton->setFlat(true);
    ui->pushButton_2->setFlat(true);
    //初始化
    model = new QSqlTableModel(this);
    model->setTable("game");
    user = new UserSql;
    connect(user,SIGNAL(mySing()),this,SLOT(show()));

    //设置密码不可见
    ui->passEdit->setEchoMode(QLineEdit::Password);
    ui->passEdit->setPlaceholderText("请输入密码:");
    ui->lineEdit->setPlaceholderText("请输入用户名:");

    //设置一键删除按键
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
    if(e->buttons() & Qt::LeftButton)//鼠标左击才有效果
         move(e->globalPos()-point);//移动窗口
}

void Sql::mousePressEvent(QMouseEvent *e)
{
    //鼠标事件中包含“按住的是左键”
    if(e->button()==Qt::LeftButton)
    {
        //获取移动的位移量
        point = e->globalPos()-frameGeometry().topLeft();
    }
}

//键盘响应登录
void Sql::keyPressEvent(QKeyEvent *e)
{
    int key = e->key();
    if(key==16777220)
    {
        if((ui->lineEdit->text())==NULL||(ui->passEdit->text())==NULL)
        {
            QMessageBox::information(this,("提示"),("用户名或者密码不能为空！"));
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
                        int mes = QMessageBox::information(this,("提示"),("登录成功!"));
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
                    QMessageBox::information(this,("提示"),("用户名或者密码不正确!"));
                }
            }
        }
    }
}

//注册新账号
void Sql::on_pushButton_2_clicked()
{
    this->close();
    user->show();
}

//登录
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
        QMessageBox::information(this,("提示"),("用户名或者密码不能为空！"));
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
                    QMessageBox::information(this,("提示"),("登录成功!"));

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
                QMessageBox::information(this,("提示"),("用户名或者密码不正确!"));
            }
        }
    }
}

