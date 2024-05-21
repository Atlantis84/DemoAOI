/**
  @file
  @author Stefan Frings
*/

#include "httprequesthandler.h"
#include "gdatafactory.h"
#include "QsLog.h"
using namespace stefanfrings;

HttpRequestHandler::HttpRequestHandler(QObject* parent)
    : QObject(parent)
{}

HttpRequestHandler::~HttpRequestHandler()
{}

void HttpRequestHandler::service(HttpRequest& request, HttpResponse& response)
{
    QJsonDocument jsonDocument = QJsonDocument::fromJson(request.getBody().data());
    QJsonObject jsonObject = jsonDocument.object();
    //rev and process data from registerstation\AOI\visuallead-lilei20211112
    if(jsonObject.contains("reType"))
    {
        QJsonValue instructionValue = jsonObject.value("reType");
//        QLOG_WARN()<<"instruction value is:"<<instructionValue.toString();
        if(GDataFactory::get_instance()->get_http_process_obj(instructionValue.toString()) == nullptr)
        {
            QLOG_WARN()<<"get http obj failed!";
            QJsonObject reJson;
            reJson.insert("responseCode","ERROR");
            reJson.insert("responseInfo",u8"get http obj failed in central control!");
            QJsonDocument doc_data(reJson);
            QByteArray response_data = doc_data.toJson(QJsonDocument::Compact);
            response.write(response_data,true);
        }
        else
        {
            GDataFactory::get_instance()->
                    get_http_process_obj(instructionValue.toString())->
                    process_http_msg(jsonObject,response);
        }
    }
    else
    {
        QLOG_WARN()<<"the client request exist ERROR!";
        QJsonObject reJson;
        reJson.insert("responseCode","ERROR");
        reJson.insert("responseInfo","the info from client exist ERROR!");

        QJsonDocument doc_data(reJson);
        QByteArray response_data = doc_data.toJson(QJsonDocument::Compact);
        response.write(response_data,true);
    }
}
