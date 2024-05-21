#ifndef HTTPDEMOAOIMSG_H
#define HTTPDEMOAOIMSG_H

#include <QObject>
#include "httpmsgprocess.h"
#include <QNetworkAccessManager>
class HttpDemoAOIMsg : public HttpMsgProcess
{
public:
    HttpDemoAOIMsg();
    virtual void process_http_msg(QJsonObject httpJsonObj,HttpResponse& response);

private:
    QNetworkAccessManager* m_pAccessManager;
};

#endif // HTTPDEMOAOIMSG_H
