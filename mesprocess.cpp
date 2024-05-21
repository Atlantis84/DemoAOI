#include "mesprocess.h"
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QFile>
#include "QsLog.h"
#include "gdatafactory.h"
#include <QEventLoop>
#include <QTimer>
MesProcess* MesProcess::m_pInstance=nullptr;
MesProcess::MesProcess(QObject *parent) : QObject(parent)
{
    accessManager = new QNetworkAccessManager(this);
    accessManagerCommon = new QNetworkAccessManager(this);
    connect(accessManager,SIGNAL(finished(QNetworkReply*)),this,SLOT(slot_urlReply(QNetworkReply*)));
    connect(accessManagerCommon,SIGNAL(finished(QNetworkReply*)),this,SLOT(slot_urlReply_common(QNetworkReply*)));
}


void MesProcess::exec_http_post()//0:Mes 1:AOI 2:register 3:visuallead
{
    QString urlDes = "http://127.0.0.1:8888";

    QNetworkRequest request;
    request.setUrl(QUrl(urlDes));
    request.setHeader(QNetworkRequest::ContentTypeHeader,QVariant("application/json"));

    QJsonObject  root, data_value,ats_models;
    QJsonArray   ats_value;

    ats_models.insert("itemId","");
    ats_models.insert("itemName","C1");
    ats_models.insert("itemL_Limit","");
    ats_models.insert("itemH_Limit","");
    ats_models.insert("itemTsValue","");
    ats_models.insert("itemLength","");
    ats_models.insert("itemRes","OK");
    ats_models.insert("errCodes","SX;LB");

    ats_value.append(ats_models);
    root.insert("atsTsItemsModels", ats_value);

    root.insert("iType", "1");
    root.insert("pcbCode", "56CTA69");
    root.insert("workStationSn", "WS101A01");
    root.insert("tsRes","OK");
    root.insert("tsLength", "");
    root.insert("pcName", "sys001");
    root.insert("pcIp", "127.0.0.1");
    root.insert("devName",u8"德律AOI");
    root.insert("devModel", "DL2000");
    root.insert("devNo", "ZD_AOI001");
    root.insert("toolNo", "");
    root.insert("toolBitSn", "");
    root.insert("logData", "");

    data_value.insert("imeiInfo","11");
    data_value.insert("meidInfo","22");
    data_value.insert("macInfo","22");
    root.insert("data",data_value);

    QJsonDocument doc_data(root);
    QByteArray request_data = doc_data.toJson(QJsonDocument::Compact);
    accessManager->post(request, request_data);
}

static int countSign = -1;
void MesProcess::exec_http_post_zd()
{
    QString urlDes = "http://127.0.0.1:8888";

    QNetworkRequest request;
    request.setUrl(QUrl(urlDes));
    request.setHeader(QNetworkRequest::ContentTypeHeader,QVariant("application/json"));

    QJsonObject  root;
    countSign++;
    if(countSign == 4)
        countSign = 0;

    //1-track: refund frame and adjust width
    if(countSign == 0)
    {
        root.insert("reType","AOI_ZD");
        root.insert("lineName","T201");
        root.insert("produceCmdNumber","111111");
        root.insert("productSN","AAAAAA");
        root.insert("productWidth","47.3");
        root.insert("spliceQuantity","4");
        root.insert("materialNumber","00000");
        root.insert("orderNumber","");
        root.insert("boardType","");
        root.insert("trackNumber","1");
    }
    else if(countSign == 1)
    {
        root.insert("reType","AOI_ZD");
        root.insert("lineName","T201");
        root.insert("produceCmdNumber","111111");
        root.insert("productSN","AAAAAA");
        root.insert("productWidth","57.3");
        root.insert("spliceQuantity","4");
        root.insert("materialNumber","00000");
        root.insert("orderNumber","");
        root.insert("boardType","");
        root.insert("trackNumber","1");
    }
    //1-track: refund frame and adjust width
    //1-track: refund frame
    else if(countSign == 2)
    {
        root.insert("reType","AOI_ZD");
        root.insert("lineName","T201");
        root.insert("produceCmdNumber","222222");
        root.insert("productSN","AAAAAA");
        root.insert("productWidth","57.3");
        root.insert("spliceQuantity","4");
        root.insert("materialNumber","00000");
        root.insert("orderNumber","");
        root.insert("boardType","");
        root.insert("trackNumber","1");
    }
    //2-track: refund frame and adjust width
    else if(countSign == 3)
    {
        root.insert("reType","AOI_ZD");
        root.insert("lineName","T201");
        root.insert("produceCmdNumber","222222");
        root.insert("productSN","AAAAAA");
        root.insert("productWidth","48.3");
        root.insert("spliceQuantity","4");
        root.insert("materialNumber","00000");
        root.insert("orderNumber","");
        root.insert("boardType","");
        root.insert("trackNumber","2");
    }
    else if(countSign == 4)
    {
        root.insert("reType","AOI_ZD");
        root.insert("lineName","T201");
        root.insert("produceCmdNumber","222222");
        root.insert("productSN","AAAAAA");
        root.insert("productWidth","58.3");
        root.insert("spliceQuantity","4");
        root.insert("materialNumber","00000");
        root.insert("orderNumber","");
        root.insert("boardType","");
        root.insert("trackNumber","2");
    }
    //2-track: refund frame and adjust width
    //2-track: refund frame
    else if(countSign == 5)
    {
        root.insert("reType","AOI_ZD");
        root.insert("lineName","T201");
        root.insert("produceCmdNumber","333333");
        root.insert("productSN","AAAAAA");
        root.insert("productWidth","58.3");
        root.insert("spliceQuantity","4");
        root.insert("materialNumber","00000");
        root.insert("orderNumber","");
        root.insert("boardType","");
        root.insert("trackNumber","2");
    }

    QJsonDocument doc_data(root);
    QByteArray request_data = doc_data.toJson(QJsonDocument::Compact);
    accessManager->post(request, request_data);
}

void MesProcess::exec_http_get(QString productsn)
{
    if(productsn.contains('/'))
    {
        productsn.remove('\r');
        QStringList lst = productsn.split('/');
        if(lst.size() == 2)
            productsn = lst[1];
        else
        {
            QLOG_WARN()<<"the SN format is ABNORMAL!";
            return;
        }
    }
    else
        productsn.remove('\r');
    QString tmpurl;
    tmpurl = "http://";
    tmpurl.append(GDataFactory::get_instance()->get_config_para("MES_IP"));
    tmpurl.append(":");
    tmpurl.append(GDataFactory::get_instance()->get_config_para("MES_PORT"));
    tmpurl.append("/ProductionForCenterControl/GetProduction?sn=");
    tmpurl.append(productsn);
    QLOG_WARN()<<"the SN after processing is:"<<tmpurl;

    QNetworkReply* reply;
    QEventLoop eventloop;
    QNetworkRequest* request = new QNetworkRequest();
    request->setUrl(QUrl(tmpurl));
    request->setHeader(QNetworkRequest::ContentTypeHeader,QVariant("application/json"));
    request->setRawHeader("appId","CenterController");
    reply = accessManager->get(*request);
    QLOG_INFO()<<"exec serial port http get request";
    connect(reply,SIGNAL(finished()),&eventloop,SLOT(quit()));
    //set get request time out
    QTimer::singleShot(5000,&eventloop,&QEventLoop::quit);
    eventloop.exec();
    if(reply->isFinished())
    {
        QLOG_INFO()<<"serial port http get request reply is NORMAL!";
    }
    else
    {
        QLOG_WARN()<<"serial port http get request reply is TIMEOUT!";;
    }
}

void MesProcess::exec_http_common_get(const QByteArray &data,int sign)
{
    QLOG_INFO() << data;
    QString urlDes;
    if(sign == 1)
        urlDes = "http://10.19.34.40:8082/api/EqProduct/CheckEqPassStationAndAssembly";
    else
        urlDes = "http://10.19.34.40:8082/api/EqProduct/CheckAndSaveEqProductInfo";

    QNetworkRequest request;
    request.setUrl(QUrl(urlDes));
    request.setHeader(QNetworkRequest::ContentTypeHeader,QVariant("application/json"));

    QEventLoop eventloop;
    QNetworkReply* reply = accessManagerCommon->post(request, data);
    QLOG_INFO()<<"exec serial port http get request";
    connect(reply,SIGNAL(finished()),&eventloop,SLOT(quit()));
    //set get request time out
    QTimer::singleShot(5000,&eventloop,&QEventLoop::quit);
    eventloop.exec();
    if(reply->isFinished())
    {
        QLOG_INFO()<<"serial port http get request reply is NORMAL!";
    }
    else
    {
        QLOG_WARN()<<"serial port http get request reply is TIMEOUT!";;
    }
}

void MesProcess::slot_urlReply(QNetworkReply *reply)
{
    if (reply->error() == QNetworkReply::NoError)
    {
        QByteArray responseByte = reply->readAll();
        emit signal_to_ui(responseByte);
    }
    else
    {
        QLOG_WARN()<<"MES status para is error!";
        return;
    }
}

void MesProcess::slot_urlReply_common(QNetworkReply *reply)
{
    if (reply->error() == QNetworkReply::NoError)
    {
        emit signal_to_common(reply->readAll());
    }
    else
    {
        QLOG_WARN()<<reply->errorString();
        QLOG_WARN()<<"Mes reply exist ERROR!";
    }
}
