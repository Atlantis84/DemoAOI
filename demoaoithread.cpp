#include "demoaoithread.h"
#include "gdatafactory.h"
#include <QEventLoop>
#include "QsLog.h"
DemoAOIThread::DemoAOIThread()
{
    m_pStartSign = false;
}

void DemoAOIThread::run()
{
    while (1) {
        QByteArray testArray;
        if(!m_pStartSign)
        {
            QLOG_WARN()<<"step1";
            mutex.lock();
            QLOG_WARN()<<"step2";
            testArray = GDataFactory::get_instance()->alter_order_of_byte_array(GDataFactory::get_plc_obj()->readDataToPLC(PATH_READ,6));
            QLOG_WARN()<<"step3";
            mutex.unlock();
            QLOG_WARN()<<"step4";
            m_pStartSign = true;
            QLOG_WARN()<<"step5";
            emit signal_plc_info_to_ui(testArray);
            QLOG_WARN()<<"step6";
        }
        QEventLoop eventloop;
        QTimer::singleShot(2000,&eventloop,&QEventLoop::quit);
        eventloop.exec();
    }
}
