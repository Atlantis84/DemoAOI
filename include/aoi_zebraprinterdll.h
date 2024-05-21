#ifndef AOI_ZEBRAPRINTERDLL_H
#define AOI_ZEBRAPRINTERDLL_H

#include "aoi_zebraprinterdll_global.h"
#include <QObject>
#include <QSettings>

class AOI_ZEBRAPRINTERDLLSHARED_EXPORT AOI_ZebraPrinterDll : public QObject
{
    Q_OBJECT
public:
    AOI_ZebraPrinterDll();

    int printExcuteFunc(QString printerSN,QString &msg);

private:
    QString m_qstrFileName;
    QSettings *m_psetting;

    typedef struct print_config{
        QString template_path;//打印模板文件地址
        QString printer_device_name;//打印机设备名称
        QString match_ch_str;//打印匹配字符
    }struct_print_config;

    struct_print_config m_print_config;

    void createIniFileConfig();
    void ReadIniFileConfig();//读取配置文件信息

};

#endif // AOI_ZEBRAPRINTERDLL_H
