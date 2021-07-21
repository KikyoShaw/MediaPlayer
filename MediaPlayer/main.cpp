#include "mainwidget.h"
#include <QApplication>
#include <QQuickWindow>
#include "sql.h"

int main(int argc, char *argv[])
{
	//web端渲染接口，用于设置web透明背景
	//QQuickWindow::setSceneGraphBackend(QSGRendererInterface::Software);
	//QWebengineView调试端口，需要在创建qt应用前设置
	//qputenv("QTWEBENGINE_REMOTE_DEBUGGING", "11234");
	QApplication a(argc, argv);
	//设置程序ico
	QApplication::setWindowIcon(QIcon(":/new/image/MediaPlayer.ico"));

	//QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
	//db.setDatabaseName("F:/qt.work/MediaPlayer/song/game.db");
	//if (!db.open())
	//{
	//	qDebug() << db.lastError();
	//	qDebug() << "open failed";
	//}
	//else
	//{
	//	qDebug() << "open success";
	//	qDebug() << db.tables();
	//}

	//  Sql sql;
	 // sql.show();
	MainWidget w;
	w.show();

	return a.exec();
}
