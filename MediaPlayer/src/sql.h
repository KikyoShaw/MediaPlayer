#ifndef SQL_H
#define SQL_H

#include <QWidget>
#include <QSqlDatabase>
#include <QSqlError>
#include <QDebug>
#include <QSqlTableModel>
#include <QMessageBox>
#include <QSqlQuery>
#include <QPoint>
#include <QPainter>
#include "usersql.h"
#include "mainwidget.h"
namespace Ui {
class Sql;
}

class Sql : public QWidget
{
    Q_OBJECT

public:
    explicit Sql(QWidget *parent = 0);
    ~Sql();

protected:
    void paintEvent(QPaintEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void mousePressEvent(QMouseEvent *);
    void keyPressEvent(QKeyEvent *);

private slots:
    void on_pushButton_2_clicked();
    void on_pushButton_clicked();

private:
    Ui::Sql *ui;
    MainWidget *widget; //用户界面
    UserSql *user; //注册
    QSqlTableModel *model;
    QSqlQuery* query;
    QPoint point;
};

#endif // SQL_H
