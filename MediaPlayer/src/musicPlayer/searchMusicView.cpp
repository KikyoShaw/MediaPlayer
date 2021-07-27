#include "searchMusicView.h"
#include <QNetworkRequest>
#include <QNetworkAccessManager>
#include <QJsonParseError>
#include <QJsonObject>
#include <QJsonArray>
#include <QScrollBar>
#include <QFile>
#include "musicManager.h"
#include "musicInfoDelegate.h"

//搜索数据分页
#define PAGE 1
#define PAGESIZE 20

SearchMusic::SearchMusic(QWidget *parent)
	:QWidget(parent), m_searchWord(QString())
{
	ui.setupUi(this);

	//优化滑动条
	QFile QSS1(":/qss/image/qss/whiteScrollbar.qss");
	if (QSS1.open(QIODevice::ReadOnly)) {
		QString strStyle = QSS1.readAll();
		ui.listView->verticalScrollBar()->setStyleSheet(strStyle);
	}
	//列表样式
	QFile QSS2(":/qss/image/qss/listViews.qss");
	if (QSS2.open(QIODevice::ReadOnly)) {
		QString strStyle = QSS2.readAll();
		ui.listView->setStyleSheet(strStyle);
	}
	//数据绑定
	ui.listView->setModel(&musicManager.getMusicInfoModel());
	ui.listView->setSelectionMode(QAbstractItemView::SingleSelection);
	ui.listView->setEditTriggers(QAbstractItemView::NoEditTriggers);
	ui.listView->setUpdatesEnabled(true);
	ui.listView->setItemDelegate(new MusicInfoDelegate(E_NetWork));

	//滑动条拉动刷新
	connect(ui.listView->verticalScrollBar(), &QScrollBar::valueChanged, this, &SearchMusic::sltSearchMusicResultByScrollBar);

	//网络组件初始化
	m_searchNetWorkManager = new QNetworkAccessManager(this);
	connect(m_searchNetWorkManager, &QNetworkAccessManager::finished, this, &SearchMusic::sltSearchMusicResult, Qt::DirectConnection);
}

SearchMusic::~SearchMusic()
{
}

void SearchMusic::searchMusic(const QString & word)
{
	musicManager.getMusicInfoModel().clear();
	m_searchWord = word;
	_searchMusic(word, 0, 0);
}

void SearchMusic::sltSearchMusicResultByScrollBar(int value)
{
	int _value = ui.listView->verticalScrollBar()->maximum();
	if (value == _value) {
		int page = musicManager.getMusicInfoModel().getPageNum();
		_searchMusic(m_searchWord, page, PAGESIZE);
	}
}

void SearchMusic::_searchMusic(const QString & word, int page, int pageSize)
{
	if (m_isWorking) return;
	m_isWorking = true;
	QString url = QString("http://mobilecdn.kugou.com/api/v3/search/song?format=json"
		"&keyword=%1&page=%2&pagesize=%3").arg(word).arg(page + PAGE).arg(pageSize + PAGESIZE);
	QNetworkRequest request;
	//设置请求数据
	request.setUrl(QUrl(url));
	request.setHeader(QNetworkRequest::UserAgentHeader, "RT-Thread ART");
	m_searchNetWorkManager->get(request);
}

void SearchMusic::parseJson(const QString & json)
{
	QByteArray byte_array;
	QJsonParseError json_error;
	QJsonDocument parse_doucment = QJsonDocument::fromJson(byte_array.append(json), &json_error);
	if (parse_doucment.isNull()) {
		return;
	}
	if (json_error.error == QJsonParseError::NoError){
		if (parse_doucment.isObject()){
			QJsonObject rootObj = parse_doucment.object();
			if (rootObj.contains("data")){
				QJsonValue valuedata = rootObj.value("data");
				if (valuedata.isObject()){
					QJsonObject valuedataObject = valuedata.toObject();
					if (valuedataObject.contains("info")){
						QJsonValue valueArray = valuedataObject.value("info");
						if (valueArray.isArray()){
							for (auto & each : valueArray.toArray()) {
								MusicInfo info;
								QJsonObject object = each.toObject();
								info.musicName = musicManager.getJsonData(object, "songname_original");
								info.musicPlayer = musicManager.getJsonData(object, "singername");
								info.musicAlbum = musicManager.getJsonData(object, "album_name");
								info.mvHash = musicManager.getJsonData(object, "mvhash");
								info.hash = musicManager.getJsonData(object, "hash");
								info.albumId = musicManager.getJsonData(object, "album_id");
								if (object.contains("duration")) {
									info.musicDuration = object.take("duration").toInt();
								}
								musicManager.getMusicInfoModel().addInfoToModel(info);
							}
						}
					}
				}
			}
		}
	}
}

void SearchMusic::sltSearchMusicResult(QNetworkReply *reply)
{
	//还原请求
	m_isWorking = false;
	//获取响应的信息，状态码为200表示正常
	QVariant status_code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
	if (reply->error() == QNetworkReply::NoError){
		QByteArray bytes = reply->readAll();  //获取字节
		QString result(bytes);                //转化为字符串
		parseJson(result);
		musicManager.getMusicInfoModel().setPageNum();
	}
}
