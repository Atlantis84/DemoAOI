#include "httpdemoaoimsg.h"
#include <QJsonDocument>
#include "QsLog.h"
#include "gdatafactory.h"
#include <QEventLoop>
HttpDemoAOIMsg::HttpDemoAOIMsg()
{
    m_pAccessManager = new QNetworkAccessManager();
}

void HttpDemoAOIMsg::process_http_msg(QJsonObject httpJsonObj, HttpResponse &response)
{
    if(httpJsonObj.value(QStringLiteral("reType")).toString() == "AOI_ZD")
    {
        GDataFactory::get_instance()->rev_info_from_mes(httpJsonObj);
        QJsonObject reJson;
        reJson.insert("responseCode","OK");
        reJson.insert("responseInfo",u8"");
        QJsonDocument doc_data(reJson);
        QByteArray response_data = doc_data.toJson(QJsonDocument::Compact);
        response.write(response_data,true);
    }
    else {
        QJsonObject reJson;
        reJson.insert("responseCode","NG");
        reJson.insert("responseInfo",u8"the msg from ZD exist ERROR");
        QJsonDocument doc_data(reJson);
        QByteArray response_data = doc_data.toJson(QJsonDocument::Compact);
        response.write(response_data,true);
    }
}
