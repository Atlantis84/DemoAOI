#include "mainwindow.h"
#include <QApplication>
#include <QFile>
#include <QTextCodec>
#include "gdatafactory.h"
#include <QDir>
#include "QsLog.h"
#include "QsLogDest.h"
#include <QDateTime>
#include <aoi_zebraprinterdll.h>
using namespace QsLogging;
void set_system_style()
{
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
    QString qssFile = ":/icon/darkgray.qss";
    QFile file(qssFile);

    if (file.open(QFile::ReadOnly)) {
        QString qss = QLatin1String(file.readAll());
        QString paletteColor = qss.mid(20, 7);
        qApp->setPalette(QPalette(QColor(paletteColor)));
        qApp->setStyleSheet(qss);
        file.close();
    }
}

void init_config_file()
{
    QString binDir=QApplication::applicationDirPath();
    QString fileName("config.json");

    QStringList searchList;
    searchList.append(binDir);

    foreach (QString dir, searchList)
    {
        QFile file(dir+"/"+fileName);
        if (file.exists())
        {
            fileName=QDir(file.fileName()).canonicalPath();
            QLOG_INFO()<<"Using json config file"<<fileName;
            GDataFactory::get_instance()->load_json_config_1(QByteArray(fileName.toLatin1()).data());
        }
        else
        {
            QLOG_WARN()<<"Json config file not exist"<<fileName;
            QApplication::exit(1);
        }
    }
}

void init_qslog()
{
    Logger& logger = Logger::instance();

    QString ttl = QDateTime::currentDateTime().toString("yyyy-MM-dd HH-mm-ss");
    QString tmpStr = "\Log\\log.txt";
    const QString sLogPath(QDir(QApplication::applicationDirPath()).filePath(tmpStr));
    DestinationPtr fileDestination(DestinationFactory::MakeFileDestination(sLogPath, EnableLogRotation, MaxSizeBytes(256*1024), MaxOldLogCount(5000)));
    logger.addDestination(fileDestination);

    DestinationPtr debugDestination(DestinationFactory::MakeDebugOutputDestination());
    logger.addDestination(debugDestination);

    DestinationPtr controlDestination(DestinationFactory::MakeFunctorDestination(GDataFactory::get_central_wgt(),SLOT(slot_rev_logs(const QString))));
    logger.addDestination(controlDestination);

    QString logLevel = GDataFactory::get_instance()->get_config_para("LOG_LEVEL");
    if(logLevel == "0")
        logger.setLoggingLevel(QsLogging::TraceLevel);
    else if(logLevel == "1")
        logger.setLoggingLevel(QsLogging::DebugLevel);
    else if(logLevel == "2")
        logger.setLoggingLevel(QsLogging::InfoLevel);
    else if(logLevel == "3")
        logger.setLoggingLevel(QsLogging::WarnLevel);
    else if(logLevel == "4")
        logger.setLoggingLevel(QsLogging::ErrorLevel);
    else if(logLevel == "5")
        logger.setLoggingLevel(QsLogging::FatalLevel);
    else
        logger.setLoggingLevel(QsLogging::WarnLevel);

    QLOG_INFO()<<u8"QsLog init SUCCESS";
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    init_config_file();
    init_qslog();
    set_system_style();
    GDataFactory::get_instance()->connect_common_signal_slot();
    GDataFactory::get_instance()->init_http_service();
    MainWindow w;
    w.show();


//    AOI_ZebraPrinterDll dd;
//    QString msg;
//    dd.printExcuteFunc("34VVVVVVVVVVVVVV12",msg);

    return a.exec();
}
