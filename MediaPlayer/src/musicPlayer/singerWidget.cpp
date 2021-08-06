#include "singerWidget.h"
#include <QScrollBar>
#include <QFile>
#include <QNetworkRequest>
#include <QNetworkAccessManager>
#include <QJsonParseError>
#include <QJsonObject>
#include <QJsonArray>
#include "flowlayout.h"
#include "singerItem.h"
#include "musicManager.h"

SingerWidget::SingerWidget(QWidget *parent)
	:QWidget(parent)
{
	ui.setupUi(this);

	//�Ż�������
	QFile QSS1(":/qss/image/qss/whiteScrollbar.qss");
	if (QSS1.open(QIODevice::ReadOnly)) {
		QString strStyle = QSS1.readAll();
		ui.scrollArea_singer->verticalScrollBar()->setStyleSheet(strStyle);
	}

	//���������ʼ��
	m_netWorkManager = new QNetworkAccessManager(this);
	connect(m_netWorkManager, &QNetworkAccessManager::finished, this, &SingerWidget::sltSingerResult, Qt::DirectConnection);

	//��ʼ��������
	m_flowLayout = new FlowLayout(0, 7, 0);
	ui.scrollAreaWidgetContents->setLayout(m_flowLayout);

}

SingerWidget::~SingerWidget()
{
}

void SingerWidget::requestSinger()
{
	if (m_isWorking) return;
	m_isWorking = true;
	QString url = QString("http://m.kugou.com/singer/class&json=true");
	QNetworkRequest request;
	//������������
	request.setUrl(QUrl(url));
	request.setHeader(QNetworkRequest::UserAgentHeader, "RT-Thread ART");
	m_netWorkManager->get(request);
}

void SingerWidget::setSingerItem(int classId, const QString & className, const QString & url)
{
	SingerItem *item = new SingerItem();
	if (item) {
		item->setSingerItem(classId, className, url);
		m_flowLayout->addWidget(item);
		m_mvWidgetsHandler.add(item);
	}
}

void SingerWidget::parseJson(const QString & json)
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
			if (rootObj.contains("list")) {
				QJsonValue valueArray = rootObj.value("list");
				if (valueArray.isArray()) {
					for (auto & each : valueArray.toArray()) {
						QJsonObject object = each.toObject();
						int classId = 0;
						if (object.contains("classid")) {
							classId = object.take("classid").toInt();
						}
						auto className = musicManager.getJsonData(object, "classname");
						auto url = musicManager.getJsonData(object, "imgurl");
						setSingerItem(classId, className, url);
					}
				}
			}
		}
	}
}

void SingerWidget::sltSingerResult(QNetworkReply *reply)
{
	//��ȡ��Ӧ����Ϣ��״̬��Ϊ200��ʾ����
	QVariant status_code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
	if (reply->error() == QNetworkReply::NoError) {
		QByteArray bytes = reply->readAll();  //��ȡ�ֽ�
		QString result(bytes);//ת��Ϊ�ַ���
		m_mvWidgetsHandler.clear();
		parseJson(result);
	}
	//��ԭ����
	m_isWorking = false;
}
