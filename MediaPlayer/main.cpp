#include "mainwidget.h"
#include <QApplication>
#include <QQuickWindow>
#include "sql.h"

int main(int argc, char *argv[])
{
	//web����Ⱦ�ӿڣ���������web͸������
	//QQuickWindow::setSceneGraphBackend(QSGRendererInterface::Software);
	//QWebengineView���Զ˿ڣ���Ҫ�ڴ���qtӦ��ǰ����
	//qputenv("QTWEBENGINE_REMOTE_DEBUGGING", "11234");
	QApplication a(argc, argv);
	//���ó���ico
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
