#ifndef USERSQL_H
#define USERSQL_H

#include <QWidget>
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlTableModel>
#include <QMessageBox>
#include <QSqlQuery>
#include <QPoint>
#include <QPainter>
#include <QMouseEvent>
#include <QPaintEvent>

namespace Ui {
class UserSql;
}

class UserSql : public QWidget
{
    Q_OBJECT

public:
    explicit UserSql(QWidget *parent = 0);
    ~UserSql();

signals:
    void mySing(); //返回上一级的信号

private slots:
    void on_pushButton_clicked();

    void on_checkBox_clicked();

    void on_checkBox_2_clicked();

    void on_checkBox_3_clicked();

    void on_pushButton_2_clicked();

protected:
    void paintEvent(QPaintEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void mousePressEvent(QMouseEvent *);

private:
    Ui::UserSql *ui;
    QSqlTableModel *model;
    QSqlQuery* query;
    QPoint point;
};

#endif // USERSQL_H
