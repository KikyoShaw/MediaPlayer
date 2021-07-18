#include "usersql.h"
#include "ui_usersql.h"

int flag;

UserSql::UserSql(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UserSql)
{
    ui->setupUi(this);

    flag = 0;

    //设置窗口图标
    QIcon icon;
    icon.addFile(QStringLiteral(":/new/image/Foldercontact.ico"), QSize(), QIcon::Normal, QIcon::Off);
    this->setWindowIcon(icon);

    //设置密码不可见
    ui->passEdit->setEchoMode(QLineEdit::Password);
    ui->pass2Edit->setEchoMode(QLineEdit::Password);

    //设置非使能
    ui->pushButton->setEnabled(false);

    //设置透明
    ui->pushButton->setFlat(true);
    ui->pushButton_2->setFlat(true);

    //设置一键删除
    ui->userEdit->setClearButtonEnabled(true);
    ui->pass2Edit->setClearButtonEnabled(true);
    ui->passEdit->setClearButtonEnabled(true);

}

UserSql::~UserSql()
{
    delete ui;
}

void UserSql::on_pushButton_clicked()
{
    query=new QSqlQuery;
    query->prepare("INSERT INTO game(number,password)"
                   "VALUES(:number,:password)");
    QString str1 = ui->userEdit->text();
    query->bindValue(":number",str1);
    QString str2 = ui->passEdit->text();
    query->bindValue(":password",str2);
    bool ok = query->exec();
    QMessageBox::information(this,("提示"),("注册成功!"));
    this->close();
    emit mySing();
    if(!ok)
    {
        QMessageBox::critical(this,"Error",query->lastError().text());
        return;
    }
    else
    {
        while(query->next())
        {
           // qDebug()<<query->value("number").toString();
           // qDebug()<<query->value("password").toString();
        }
    }
}

void UserSql::on_checkBox_clicked()
{
    bool radio_sel = ui->checkBox->isChecked();
    if(radio_sel)
    {
        if(ui->userEdit->text()=="")
        {
            ui->checkBox->setStyleSheet("color:red");
            ui->checkBox->setText("用户名不能为空!");
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
            while(query->next())
            {
                qDebug()<<query->value("number").toString();
                QString str1 = ui->userEdit->text();
               // QString str2 = ui->passEdit->text();
                qDebug()<<"2222"<<str1;
                if(str1==(query->value("number").toString()))
                {
                    flag = 1;
                    ui->checkBox->setStyleSheet("color:red");
                    ui->checkBox->setText("当前用户已经存在!");
                    return;
                }

             }
         }
         ui->checkBox->setStyleSheet("color:green");
         ui->checkBox->setText("用户名可用!");
      }
    else
    {
        ui->checkBox->setText("");
    }
}

void UserSql::on_checkBox_2_clicked()
{
    bool radio_sel = ui->checkBox_2->isChecked();
    if(radio_sel)
    {
        if(ui->passEdit->text()=="")
        {
            ui->checkBox_2->setStyleSheet("color:red");
            ui->checkBox_2->setText("密码不能为空!");
        }
        else
        {
            int aa = ui->passEdit->text().length();
            if(aa>10)
            {
                ui->checkBox_2->setStyleSheet("color:red");
                ui->checkBox_2->setText("密码不能超过10个字符!");
            }
            else
            {
                ui->checkBox_2->setStyleSheet("color:green");
                ui->checkBox_2->setText("密码可用!");
            }
        }
    }
    else
    {
        ui->checkBox_2->setText("");
    }
}

void UserSql::on_checkBox_3_clicked()
{
    bool radio_sel = ui->checkBox_3->isChecked();
    if(radio_sel)
    {
        if(ui->pass2Edit->text()=="")
        {
            ui->checkBox_3->setStyleSheet("color:red");
            ui->checkBox_3->setText("密码不能为空!");
        }
        else
        {
           if(ui->passEdit->text()==ui->pass2Edit->text())
           {
               ui->checkBox_3->setStyleSheet("color:green");
               ui->checkBox_3->setText("密码输入正确!");
               ui->pushButton->setEnabled(true);
           }
           else
           {
               ui->checkBox_3->setStyleSheet("color:red");
               ui->checkBox_3->setText("两次输入密码不一致!");
           }
        }
    }
    else
    {
        ui->checkBox_3->setText("");
    }
}

void UserSql::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    p.drawPixmap(0,0,this->width(),this->height(),QPixmap(":/new/image/beijing2.jpeg"));
   // this->setCursor(QCursor(QPixmap(":/new/image/777.png")));
}

void UserSql::mouseMoveEvent(QMouseEvent *e)
{
    if(e->buttons() & Qt::LeftButton)//鼠标左击才有效果
        move(e->globalPos()-point);//移动窗口
}

void UserSql::mousePressEvent(QMouseEvent *e)
{
    //鼠标事件中包含“按住的是左键”
    if(e->button()==Qt::LeftButton)
    {
        //获取移动的位移量
        point = e->globalPos()-frameGeometry().topLeft();
    }
}

//返回
void UserSql::on_pushButton_2_clicked()
{
    this->close();
    emit mySing();
}
