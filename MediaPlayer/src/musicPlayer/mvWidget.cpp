#include "mvWidget.h"
#include <QScrollBar>
#include <QFile>
#include <QNetworkRequest>
#include <QNetworkAccessManager>
#include <QJsonParseError>
#include <QJsonObject>
#include <QJsonArray>
#include "musicManager.h"
#include "flowlayout.h"
#include "mvItemWidget.h"

//搜索数据分页
#define PAGE 1
#define PAGESIZE 20

MVWidget::MVWidget(QWidget *parent /* = Q_NULLPTR */)
	:QWidget(parent), m_mvWord(QString()), m_pIndex(0)
{
	ui.setupUi(this);

	//优化滑动条
	QFile QSS1(":/qss/image/qss/whiteScrollbar.qss");
	if (QSS1.open(QIODevice::ReadOnly)) {
		QString strStyle = QSS1.readAll();
		ui.scrollArea_mv->verticalScrollBar()->setStyleSheet(strStyle);
	}
	
	//MV
	connect(ui.pushButton_searchMV, &QPushButton::clicked, this, &MVWidget::sltSearchMV);
	//滑动条拉动刷新
	connect(ui.scrollArea_mv->verticalScrollBar(), &QScrollBar::valueChanged, this, &MVWidget::sltSearchMvResultByScrollBar);

	//网络组件初始化
	m_searchNetWorkManager = new QNetworkAccessManager(this);
	connect(m_searchNetWorkManager, &QNetworkAccessManager::finished, this, &MVWidget::sltSearchMvResult, Qt::DirectConnection);

	//初始化布局器
	m_flowLayout = new FlowLayout(0, 4, 0);
	ui.scrollAreaWidgetContents->setLayout(m_flowLayout);
}

MVWidget::~MVWidget()
{
}

void MVWidget::searchMV(const QString & word, int page, int pageSize)
{
	if (m_isWorking) return;
	m_isWorking = true;
	QString url = QString("http://mvsearch.kugou.com/mv_search?"
		"keyword=%1&page=%2&pagesize=%3&userid=-1&clientver=&platform=WebFilter&tag=em&filter=2&iscorrection=1&privilege_filter=0&_=1515052279917").arg(word).arg(page + PAGE).arg(pageSize + PAGESIZE);
	QNetworkRequest request;
	//设置请求数据
	request.setUrl(QUrl(url));
	request.setHeader(QNetworkRequest::UserAgentHeader, "RT-Thread ART");
	m_searchNetWorkManager->get(request);
}

void MVWidget::parseJson(const QString & json)
{
	QByteArray byte_array;
	QJsonParseError json_error;
	QJsonDocument parse_doucment = QJsonDocument::fromJson(byte_array.append(json), &json_error);
	if (parse_doucment.isNull()) {
		return;
	}
	if (json_error.error == QJsonParseError::NoError) {
		if (parse_doucment.isObject()) {
			QJsonObject rootObj = parse_doucment.object();
			if (rootObj.contains("data")) {
				QJsonValue valuedata = rootObj.value("data");
				if (valuedata.isObject()) {
					QJsonObject valuedataObject = valuedata.toObject();
					if (valuedataObject.contains("lists")) {
						QJsonValue valueArray = valuedataObject.value("lists");
						if (valueArray.isArray()) {
							for (auto & each : valueArray.toArray()) {
								MvInfo info;
								QJsonObject object = each.toObject();
								info.mvName = musicManager.getJsonData(object, "MvName");
								info.mvPlayer = musicManager.getJsonData(object, "SingerName");
								info.fileName = musicManager.getJsonData(object, "FileName");
								info.mvHash = musicManager.getJsonData(object, "MvHash");
								info.fileHash = musicManager.getJsonData(object, "FileHash");
								info.albumId = musicManager.getJsonData(object, "album_id");
								info.publishDate = musicManager.getJsonData(object, "PublishDate");
								info.mvID = musicManager.getJsonData(object, "MvID");
								info.ThumbGif = musicManager.getJsonData(object, "ThumbGif");
								info.pic = musicManager.getJsonData(object, "Pic");
								if (object.contains("MvHot")) {
									info.mvHot = object.take("MvHot").toInt();
								}
								if (object.contains("HistoryHeat")) {
									info.historyHeat = object.take("HistoryHeat").toInt();
								}
								musicManager.getMvInfoModel().addInfoToModel(info);
							}
						}
					}
				}
			}
		}
	}
	setMvModelItem();
}

void MVWidget::setMvModelItem()
{
	int index = 1;
	auto info = musicManager.getMvInfoModel().getList();
	for (auto & each : info){
		if (index <= m_pIndex) {
			index++;
			continue;
		}
		MvItem *item = new MvItem();
		if (item) {
			item->setMvInfo(each);
			m_flowLayout->addWidget(item);
			m_mvWidgetsHandler.add(item);
		}
	}
}

void MVWidget::sltSearchMvResult(QNetworkReply * reply)
{
	//获取响应的信息，状态码为200表示正常
	QVariant status_code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
	if (reply->error() == QNetworkReply::NoError) {
		QByteArray bytes = reply->readAll();  //获取字节
		QString result(bytes);                //转化为字符串
		parseJson(result);
		musicManager.getMvInfoModel().setPageNum();
	}
	//还原请求
	m_isWorking = false;
}

void MVWidget::sltSearchMvResultByScrollBar(int value)
{
	int _value = ui.scrollArea_mv->verticalScrollBar()->maximum();
	if (value == _value) {
		int page = musicManager.getMvInfoModel().getPageNum();
		m_pIndex = musicManager.getMvInfoModel().rowCount();
		searchMV(m_mvWord, page, PAGESIZE);
	}
}

void MVWidget::sltSearchMV()
{
	musicManager.getMvInfoModel().clear();
	m_mvWidgetsHandler.clear();
	m_pIndex = 0;
	m_mvWord = ui.lineEdit_searchMV->text();
	searchMV(m_mvWord, 0, 0);
}
