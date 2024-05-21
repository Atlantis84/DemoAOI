#ifndef GDATAFACTORY_H
#define GDATAFACTORY_H

#include <QObject>
#include "centralwgt.h"
#include <QFile>
#include <QJsonParseError>
#include <QJsonObject>
#include <QJsonArray>
#include "headinfowgt.h"
#include "mesprocess.h"
#include "aoi_plclinkdll.h"
#include "httplistener.h"
#include "httprequesthandler.h"
#include "staticfilecontroller.h"
#include "httpmsgprocess.h"
#include "modbusclientdll.h"
#define  PATH_A_WRITE   "32570"
#define  PATH_B_WRITE   "32576"
#define  PATH_READ      "32200"

class GDataFactory : public QObject
{
    Q_OBJECT
public:
    static GDataFactory* get_instance()
    {
        if(m_pInstance == nullptr)
            m_pInstance = new GDataFactory();
        return m_pInstance;
    }

    void load_json_config_1(char *filename);
    QString get_config_para(const QString keyname);
    void set_config_para_1(const QString keyname, const QString keyvalue);

    void set_ex_order_and_width_A(QString order,QString width);
    void set_ex_order_and_width_B(QString order,QString width);
    void connect_common_signal_slot();
    void init_http_service();

    void send_zd_msg_to_ui(const QByteArray data);
private:
    explicit GDataFactory(QObject *parent = nullptr);

    QString search_http_config_file();

    static GDataFactory* m_pInstance;
    static AOI_PLCLinkDll* m_pPLCObj;
    static ModbusClientDll* m_pModbusClientPutAway;
    static ModbusClientDll* m_pModbusClientSort;

    QFile m_pConfigFile;
    QJsonDocument m_pJsonDocument;
    QJsonObject m_pRootObj;
    QString m_pConfigFileName;
    QMap<QString,QString> ConfigInfo;
    void save_config_file();

    QString m_pExOrderA;
    QString m_pExWidthA;
    QString m_pExOrderB;
    QString m_pExWidthB;

public:
    static CentralWgt* get_central_wgt()
    {
        return CentralWgt::get_instance();
    }

    static HeadInfoWgt* get_head_info_wgt()
    {
        return HeadInfoWgt::get_instance();
    }

    static MesProcess* get_mes_process()
    {
        return MesProcess::get_instance();
    }

    static AOI_PLCLinkDll* get_plc_obj()
    {
        if(m_pPLCObj == nullptr)
            m_pPLCObj = new AOI_PLCLinkDll();
        return m_pPLCObj;
    }

    static ModbusClientDll* get_modbus_client()
    {
        if(m_pModbusClientPutAway == nullptr)
            m_pModbusClientPutAway = new ModbusClientDll();
        return m_pModbusClientPutAway;
    }

    static ModbusClientDll* get_modbus_client_sort()
    {
        if(m_pModbusClientSort == nullptr)
            m_pModbusClientSort = new ModbusClientDll();
        return m_pModbusClientSort;
    }

    HttpMsgProcess* get_http_process_obj(QString msgType)
    {
        HttpMsgProcess* httpMsgProcess = HttpMsgProcess::get_interface_obj(msgType);
        return httpMsgProcess;
    }

    QByteArray alter_order_of_byte_array(QByteArray data);
    QString bytes_to_str(QByteArray data);

    void rev_info_from_mes(const QJsonObject& httpJsonObj)
    {
        emit signal_mes_info_to_ui(httpJsonObj);
    }

signals:
    void signal_mes_info_to_ui(const QJsonObject& obj);
public slots:
};

#endif // GDATAFACTORY_H
