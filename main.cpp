#include "mediaplayer.h"
#include "mainwidget.h"
#include <QApplication>
#include "sql.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("F:/qt.work/MediaPlayer/song/game.db");
    if(!db.open())
    {
        qDebug()<<db.lastError();
        qDebug()<<"open failed";
    }
    else
    {
        qDebug()<<"open success";
        qDebug()<<db.tables();
    }

  //  Sql sql;
   // sql.show();

    MainWidget w;
    w.show();

    return a.exec();
}
