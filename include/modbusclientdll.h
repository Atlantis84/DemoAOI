#ifndef MODBUSCLIENTDLL_H
#define MODBUSCLIENTDLL_H

#include "modbusclientdll_global.h"
#include <QObject>

#define MODBUS_READ_SIGN    (1)
#define MODBUS_WRITE_SIGN   (2)

enum MODBUS_STATUS{
    RES_OK,
    RES_TIMEOUT,
    CONNECT_ERR
};

enum MODBUS_TYPE{
    MODBUS_TCP,
    MODBUS_COM
};

class MODBUSCLIENTDLLSHARED_EXPORT ModbusClientDll : public QObject
{
    Q_OBJECT
public:
    ModbusClientDll();

    void init_ip_port(const MODBUS_TYPE config, const QString RemoteIPOrPort,const int RemotPortOrBaud);

    MODBUS_STATUS sendMessageToPLC(int rw_ctrl,
                                   int starting_address,
                                   int num_registers,
                                   uint16_t* value);//Modbus发送指令
private:
    QString m_remote_ip_port;
    int m_remote_port_baud;
    MODBUS_TYPE modbus_mode;
};

#endif // MODBUSCLIENTDLL_H
