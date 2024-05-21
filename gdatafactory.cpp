#include "gdatafactory.h"
#include "QsLog.h"
#include <QApplication>
#include <QDir>
using namespace QsLogging;
GDataFactory* GDataFactory::m_pInstance = nullptr;
AOI_PLCLinkDll* GDataFactory::m_pPLCObj = nullptr;
ModbusClientDll* GDataFactory::m_pModbusClientPutAway = nullptr;
ModbusClientDll* GDataFactory::m_pModbusClientSort = nullptr;

GDataFactory::GDataFactory(QObject *parent) : QObject(parent)
{
    m_pExOrderA = "";
    m_pExWidthA = "";
    m_pExOrderB = "";
    m_pExWidthB = "";
}

void GDataFactory::connect_common_signal_slot()
{
    connect(this,SIGNAL(signal_mes_info_to_ui(const QJsonObject&)),get_central_wgt(),SLOT(slot_rev_mes_info_to_ui(const QJsonObject&)));
}

void GDataFactory::save_config_file()
{
    m_pConfigFile.setFileName(m_pConfigFileName);
    if(m_pConfigFile.open(QIODevice::ReadWrite | QIODevice::Text))
    {
        m_pConfigFile.resize(0);//this sentence is essential,or the file will be abnormal---20211123lilei
        m_pJsonDocument.setObject(m_pRootObj);
        m_pConfigFile.seek(0);
        m_pConfigFile.write(m_pJsonDocument.toJson());
        m_pConfigFile.close();
        m_pConfigFile.flush();
    }
}

void GDataFactory::load_json_config_1(char *filename)
{
    m_pConfigFile.setFileName(QString(filename));
    m_pConfigFileName=QString(filename);
    QString JsonValue;
    if(m_pConfigFile.open(QIODevice::ReadWrite | QIODevice::Text))
    {
        QLOG_INFO()<<"load json config file success!";
        JsonValue = m_pConfigFile.readAll();
        m_pConfigFile.close();

        QJsonParseError parseJsonErr;
        m_pJsonDocument = QJsonDocument::fromJson(JsonValue.toUtf8(),&parseJsonErr);
        if(!(parseJsonErr.error == QJsonParseError::NoError))
        {
            QLOG_INFO()<<"parse json file failed!";
            return;
        }
        else {
            QLOG_INFO()<<"parse json file success!";
        }

        m_pRootObj = m_pJsonDocument.object();
        if(m_pRootObj.contains("ProductPathInfo"))
        {
            QJsonValue arrayValue = m_pRootObj.value(QStringLiteral("ProductPathInfo"));
            if(arrayValue.isArray())
            {
                QJsonArray array = arrayValue.toArray();
                for (int i=0;i<array.size();i++)
                {
                    if(i==0)
                    {
                        QJsonValue iconArray = array.at(i);
                        QJsonObject icon = iconArray.toObject();

                        //load the pre productA info
                        m_pExOrderA = icon["EX_ORDER_A"].toString();
                        m_pExWidthA = icon["EX_WIDTH_A"].toString();
                        ConfigInfo.insert("EX_ORDER_A",icon["EX_ORDER_A"].toString());
                        ConfigInfo.insert("EX_WIDTH_A",icon["EX_WIDTH_A"].toString());
                        ConfigInfo.insert("EX_BOARD_TYPE_A",icon["EX_BOARD_TYPE_A"].toString());
                        ConfigInfo.insert("EX_LINE_NAME_A",icon["EX_LINE_NAME_A"].toString());
                        ConfigInfo.insert("EX_MATERIAL_NUMBER_A",icon["EX_MATERIAL_NUMBER_A"].toString());
                        ConfigInfo.insert("EX_ORDER_NUMBER_A",icon["EX_ORDER_NUMBER_A"].toString());
                        ConfigInfo.insert("EX_SPLICE_QUANTITY_A",icon["EX_SPLICE_QUANTITY_A"].toString());
                        ConfigInfo.insert("FILE_NAME_A",icon["FILE_NAME_A"].toString());
                        ConfigInfo.insert("PRINTER_NAME_A",icon["PRINTER_NAME_A"].toString());
                        ConfigInfo.insert("PRINTER_PORT_A",icon["PRINTER_PORT_A"].toString());
                    }
                    else
                    {
                        QJsonValue iconArray = array.at(i);
                        QJsonObject icon = iconArray.toObject();

                        //load the pre productB info
                        m_pExOrderB = icon["EX_ORDER_B"].toString();
                        m_pExWidthB = icon["EX_WIDTH_B"].toString();
                        ConfigInfo.insert("EX_ORDER_B",icon["EX_ORDER_B"].toString());
                        ConfigInfo.insert("EX_WIDTH_B",icon["EX_WIDTH_B"].toString());
                        ConfigInfo.insert("EX_BOARD_TYPE_B",icon["EX_BOARD_TYPE_B"].toString());
                        ConfigInfo.insert("EX_LINE_NAME_B",icon["EX_LINE_NAME_B"].toString());
                        ConfigInfo.insert("EX_MATERIAL_NUMBER_B",icon["EX_MATERIAL_NUMBER_B"].toString());
                        ConfigInfo.insert("EX_ORDER_NUMBER_B",icon["EX_ORDER_NUMBER_B"].toString());
                        ConfigInfo.insert("EX_SPLICE_QUANTITY_B",icon["EX_SPLICE_QUANTITY_B"].toString());
                        ConfigInfo.insert("FILE_NAME_B",icon["FILE_NAME_B"].toString());
                        ConfigInfo.insert("PRINTER_NAME_B",icon["PRINTER_NAME_B"].toString());
                        ConfigInfo.insert("PRINTER_PORT_B",icon["PRINTER_PORT_B"].toString());
                    }
                }
            }
        }

        if(m_pRootObj.contains("PortConfigs"))
        {
            QJsonValue portValue = m_pRootObj.value(QStringLiteral("PortConfigs"));
            if(portValue.isObject())
            {
                QJsonObject obj = portValue.toObject();
                if(obj.contains("COM_PORT"))
                    ConfigInfo.insert("COM_PORT",obj.value("COM_PORT").toString());
                if(obj.contains("COM_BARD_RATE"))
                    ConfigInfo.insert("COM_BARD_RATE",obj.value("COM_BARD_RATE").toString());
                if(obj.contains("TCP_IP_ADDRESS_PUT_AWAY"))
                    ConfigInfo.insert("TCP_IP_ADDRESS_PUT_AWAY",obj.value("TCP_IP_ADDRESS_PUT_AWAY").toString());
                if(obj.contains("TCP_IP_PORT_PUT_AWAY"))
                    ConfigInfo.insert("TCP_IP_PORT_PUT_AWAY",obj.value("TCP_IP_PORT_PUT_AWAY").toString());
                if(obj.contains("TCP_IP_ADDRESS_SORT"))
                    ConfigInfo.insert("TCP_IP_ADDRESS_SORT",obj.value("TCP_IP_ADDRESS_SORT").toString());
                if(obj.contains("TCP_IP_PORT_SORT"))
                    ConfigInfo.insert("TCP_IP_PORT_SORT",obj.value("TCP_IP_PORT_SORT").toString());
                if(obj.contains("IS_CONNECT_MODE"))
                    ConfigInfo.insert("IS_CONNECT_MODE",obj.value("IS_CONNECT_MODE").toString());
                if(obj.contains("TRACK_TYPE"))
                    ConfigInfo.insert("TRACK_TYPE",obj.value("TRACK_TYPE").toString());
                if(obj.contains("LINE_PARA"))
                    ConfigInfo.insert("LINE_PARA",obj.value("LINE_PARA").toString());
            }
        }
    }
    else
    {
        QLOG_INFO()<<"load json config file failed!";
        return;
    }
    m_pConfigFile.flush();
}

QString GDataFactory::get_config_para(const QString keyname)
{
    return ConfigInfo[keyname];
}

void GDataFactory::set_config_para_1(const QString keyname, const QString keyvalue)
{
    ConfigInfo[keyname] = keyvalue;//update the config file,at the same time you have to update the info that has been loaded

    QJsonObject subObj;
    QJsonArray subArray;

    if((keyname == "EX_ORDER_A") || (keyname == "EX_WIDTH_A"))
    {
        QJsonValueRef refValue = m_pRootObj.find("ProductPathInfo").value();
        QJsonArray refArray = refValue.toArray();

        QJsonArray::Iterator arrayIterator = refArray.begin();
        QJsonValueRef elementOneValueRef = arrayIterator[0];
        QJsonObject elementOneObject = elementOneValueRef.toObject();
        elementOneObject.insert(keyname,keyvalue);
        elementOneValueRef = elementOneObject;
        refValue = refArray;
    }

    if((keyname == "EX_ORDER_B") || (keyname == "EX_WIDTH_B"))
    {
        QJsonValueRef refValue = m_pRootObj.find("ProductPathInfo").value();
        QJsonArray refArray = refValue.toArray();

        QJsonArray::Iterator arrayIterator = refArray.begin();
        QJsonValueRef elementOneValueRef = arrayIterator[1];
        QJsonObject elementOneObject = elementOneValueRef.toObject();
        elementOneObject.insert(keyname,keyvalue);
        elementOneValueRef = elementOneObject;
        refValue = refArray;
    }

    if(keyname == "COM_BARD_RATE" || keyname == "COM_PORT")
    {
        if(m_pRootObj.contains("PortConfigs") && m_pRootObj["PortConfigs"].isObject())
        {
            QJsonValueRef refValue = m_pRootObj.find("PortConfigs").value();
            QJsonObject obj = refValue.toObject();
            obj[keyname] = keyvalue;
            refValue = obj;
        }
    }
    m_pJsonDocument.setObject(m_pRootObj);
    save_config_file();
}

void GDataFactory::set_ex_order_and_width_A(QString order, QString width)
{
    set_config_para_1("EX_ORDER_A",order);
    set_config_para_1("EX_WIDTH_A",width);
    m_pExOrderA = order;
    m_pExWidthA = width;
}

void GDataFactory::set_ex_order_and_width_B(QString order, QString width)
{
    set_config_para_1("EX_ORDER_B",order);
    set_config_para_1("EX_WIDTH_B",width);
    m_pExOrderB = order;
    m_pExWidthB = width;
}

void GDataFactory::init_http_service()
{
    QString configFileName = search_http_config_file();
    if(configFileName == nullptr)
        QLOG_WARN()<<"search http config file failed!";
    else
    {
        // Configure and start the TCP listener
        QSettings* listenerSettings=new QSettings(configFileName,QSettings::IniFormat,this);
        listenerSettings->beginGroup("listener");
        new HttpListener(listenerSettings,new HttpRequestHandler(this),this);
        QLOG_INFO()<<"init http service success!";
    }
}

QString GDataFactory::search_http_config_file()
{
    QString binDir=QApplication::applicationDirPath();
    QString fileName("httpconfig.ini");

    QStringList searchList;
    searchList.append(binDir);

    foreach (QString dir, searchList)
    {
        QFile file(dir+"/"+fileName);
        if (file.exists())
        {
            fileName=QDir(file.fileName()).canonicalPath();
            QLOG_INFO()<<"Using http ini file "<<fileName;
            return fileName;
        }
    }

    // not found
    foreach (QString dir, searchList)
    {
        QLOG_WARN()<<dir<<fileName<<" not found";
    }
    QLOG_FATAL()<<"Cannot find http ini file "<<fileName;
    return nullptr;
}

QByteArray GDataFactory::alter_order_of_byte_array(QByteArray data)
{
    QByteArray result;

    for(int i =0;i<data.length();i++)
    {
        if(i%2 == 0)
        {
            result.append(data[i+1]);
            result.append(data[i]);
        }
    }

    return result;
}

QString GDataFactory::bytes_to_str(QByteArray data)
{
    QString str_data;
    str_data.clear();
    quint8 n_data = 0;
    for(int i=0; i<data.size(); i++)
    {
        n_data = static_cast<quint8>(data[i]) ;
        if(n_data < 16) str_data.append('0');
        str_data.append(QString::number(n_data, 16));
        str_data.append(' ');
    }
    return str_data;
}
