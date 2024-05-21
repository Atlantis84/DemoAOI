#include "httpmsgprocess.h"
#include <QJsonDocument>
#include <QTextCodec>
#include "gdatafactory.h"
#include "QsLog.h"
#include "httpdemoaoimsg.h"
HttpMsgProcess::HttpMsgProcess(QObject *parent) : QObject(parent)
{

}

HttpMsgProcess *HttpMsgProcess::get_interface_obj(QString objType)
{
    HttpMsgProcess* tmpProcess;
    if(objType == "AOI_ZD")
        tmpProcess = new HttpDemoAOIMsg();
    else
        return nullptr;
    return tmpProcess;
}
