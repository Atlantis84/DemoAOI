#ifndef AOI_PLCLINKDLL_H
#define AOI_PLCLINKDLL_H

#include <QObject>
#include "aoi_plclinkdll_global.h"
#include <QtSerialPort/QSerialPort>

#define RESULT_TRANS_DEC 1


class AOI_PLCLINKDLLSHARED_EXPORT AOI_PLCLinkDll : public QObject
{
    Q_OBJECT
public:
    AOI_PLCLinkDll();

    void connectToPLC(QString portName, QString portRate);

    QByteArray readDataToPLC(QString startAdress, int Length);
    bool writeDataToPLC(QString startAdress, int length, QByteArray data);
    QString QByteArrayToValue(QByteArray array, int flags);

private:
    QSerialPort* m_serial;

    const uchar CMD_END[1] = {0x0d};//帧尾

    bool checkReadData(QByteArray data);
    bool checkWriteData(QByteArray data);

    QString checkXorH(QByteArray data);

};

#endif // AOI_PLCLINKDLL_H
