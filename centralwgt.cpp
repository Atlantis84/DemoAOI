#include "centralwgt.h"
#include <QVBoxLayout>
#include <QGroupBox>
#include <QLabel>
#include <QList>
#include "gdatafactory.h"
#include <QTimerEvent>
#include "QsLog.h"
#include "aoi_zebraprinterdll.h"
#include <QMessageBox>
#include <QPushButton>
#include <QEventLoop>
#include "aoi_zebraprinterdll.h"
#include "CodeSoftPrinter.h"
#include <QApplication>
#include <QTextCodec>
#include <QDate>
CentralWgt* CentralWgt::m_pInstance = nullptr;
CentralWgt::CentralWgt(QWidget *parent) : QWidget(parent)
{
    QVBoxLayout* vAll = new QVBoxLayout();

    m_pLineEditExInfoA = new QLineEdit();
    m_pLineEditCurrentInfoA = new QLineEdit();
    m_pLineEditExWidthA = new QLineEdit();
    m_pLineEditCurrentWidthA = new QLineEdit();

    m_pLineEditExInfoB = new QLineEdit();
    m_pLineEditCurrentInfoB = new QLineEdit();
    m_pLineEditExWidthB = new QLineEdit();
    m_pLineEditCurrentWidthB = new QLineEdit();

    m_pLineEditLineName = new QLineEdit();

    m_pTextEditLogInfo = new QTextEdit();
    m_pTextEditLogInfo->setReadOnly(true);
    m_pTextEditLogInfo->document()->setMaximumBlockCount(1000);
    m_pTextEditLogInfo->setStyleSheet("border:1px solid rgba(0,255,0,100);color:rgb(0,0,0);background-color:rgb(0,0,0);font: 10pt 'LiSu'");

    QList<QLineEdit*> lst;
    lst<<m_pLineEditExInfoA<<m_pLineEditCurrentInfoA<<m_pLineEditExWidthA<<m_pLineEditCurrentWidthA<<
         m_pLineEditExInfoB<<m_pLineEditCurrentInfoB<<m_pLineEditExWidthB<<m_pLineEditCurrentWidthB;
    for (int i=0;i<lst.size();i++) {
        lst[i]->setStyleSheet("QLineEdit{border:1px solid rgba(0,255,0,100);font-family:LiSu;font-size:20px;"
                                          "color:rgb(0,255,0);background-color:rgba(0,0,0,0);font-weight:bold;}"
                                          "QLineEdit:hover{border:2px solid rgba(0,255,0,150);}");
//        lst[i]->setText(u8"test");
        lst[i]->setAlignment(Qt::AlignCenter);
        lst[i]->setReadOnly(true);
    }

    QList<QLabel*> lstLabel;
    QHBoxLayout* hBox1 = new QHBoxLayout();
    QGroupBox* grpBox1 = new QGroupBox(u8"A轨产品信息");
    grpBox1->setStyleSheet("QGroupBox{border:1px solid rgba(0,255,0,100);color:rgb(0,255,0);font-family:LiSu;}");
    QLabel* labelExOrder = new QLabel(u8"  前一制令单:");
    QLabel* labelCurrentOrder = new QLabel(u8"  当前制令单:");
    hBox1->addWidget(labelExOrder);
    hBox1->addWidget(m_pLineEditExInfoA);
    hBox1->addWidget(labelCurrentOrder);
    hBox1->addWidget(m_pLineEditCurrentInfoA);

    QHBoxLayout* hBox2 = new QHBoxLayout();
    QLabel* labelExWidth = new QLabel(u8"前一产品宽度:");
    QLabel* labelCurrentWidth = new QLabel(u8"当前产品宽度:");
    hBox2->addWidget(labelExWidth);
    hBox2->addWidget(m_pLineEditExWidthA);
    hBox2->addWidget(labelCurrentWidth);
    hBox2->addWidget(m_pLineEditCurrentWidthA);

    QVBoxLayout* vBox1 = new QVBoxLayout();
    vBox1->addLayout(hBox1);
    vBox1->addLayout(hBox2);
    grpBox1->setLayout(vBox1);

    QHBoxLayout* hBox4 = new QHBoxLayout();
    QGroupBox* grpBox3 = new QGroupBox(u8"B轨产品信息");
    grpBox3->setStyleSheet("QGroupBox{border:1px solid rgba(0,255,0,100);color:rgb(0,255,0);font-family:LiSu;}");
    QLabel* labelExOrderB = new QLabel(u8"  前一制令单:");
    QLabel* labelCurrentOrderB = new QLabel(u8"  当前制令单:");
    hBox4->addWidget(labelExOrderB);
    hBox4->addWidget(m_pLineEditExInfoB);
    hBox4->addWidget(labelCurrentOrderB);
    hBox4->addWidget(m_pLineEditCurrentInfoB);

    QHBoxLayout* hBox5 = new QHBoxLayout();
    QLabel* labelExWidthB = new QLabel(u8"前一产品宽度:");
    QLabel* labelCurrentWidthB = new QLabel(u8"当前产品宽度:");
    hBox5->addWidget(labelExWidthB);
    hBox5->addWidget(m_pLineEditExWidthB);
    hBox5->addWidget(labelCurrentWidthB);
    hBox5->addWidget(m_pLineEditCurrentWidthB);

    QVBoxLayout* vBox2 = new QVBoxLayout();
    vBox2->addLayout(hBox4);
    vBox2->addLayout(hBox5);
    grpBox3->setLayout(vBox2);

    QGroupBox* grpBox2 = new QGroupBox(u8"日志信息");
    grpBox2->setStyleSheet("QGroupBox{border:1px solid rgba(0,255,0,100);color:rgb(0,255,0);font-family:LiSu;}");
    QVBoxLayout* vBox3 = new QVBoxLayout();
    vBox3->addWidget(m_pTextEditLogInfo);
    QPushButton* btnRead = new QPushButton();
    btnRead->setText(u8"读测试");
    QPushButton* btnWrite = new QPushButton();
    btnWrite->setText(u8"写测试");
    connect(btnRead,SIGNAL(clicked()),this,SLOT(slot_btn_read_click()));
    connect(btnWrite,SIGNAL(clicked()),this,SLOT(slot_btn_write_click()));
//    vBox3->addWidget(btnRead);
//    vBox3->addWidget(btnWrite);
    QHBoxLayout* hBox6 = new QHBoxLayout();
    QLabel* labelLineName = new QLabel(u8"线体名称:");
    QPushButton* btnSetPara = new QPushButton(u8"修改线体名称");
    connect(btnSetPara,SIGNAL(clicked()),this,SLOT(slot_btn_set_para_click()));
    hBox6->addWidget(labelLineName);
    m_pLineEditLineName->setStyleSheet("color:rgb(0,255,0)");
    m_pLineEditLineName->setAlignment(Qt::AlignCenter);
    hBox6->addWidget(m_pLineEditLineName);
    hBox6->addWidget(btnSetPara);
//    vBox3->addLayout(hBox6);
    grpBox2->setLayout(vBox3);

    lstLabel<<labelExOrder<<labelExWidth<<labelExOrderB<<labelExWidthB<<labelCurrentOrder<<
              labelCurrentWidth<<labelCurrentOrderB<<labelCurrentWidthB<<labelLineName;
    for(int i=0;i<lstLabel.length();i++)
    {
        lstLabel[i]->setStyleSheet("color:rgb(0,255,0)");
    }

    vAll->addWidget(GDataFactory::get_head_info_wgt());
    vAll->addWidget(grpBox1);
    vAll->addWidget(grpBox3);
    vAll->addWidget(grpBox2);
    vAll->setStretch(0,2);
    vAll->setStretch(0,1);
    vAll->setStretch(0,3);
    this->setLayout(vAll);

    QString tmptcpAddress = GDataFactory::get_instance()->get_config_para("TCP_IP_ADDRESS_PUT_AWAY");
    QString tmpTcpport = GDataFactory::get_instance()->get_config_para("TCP_IP_PORT_PUT_AWAY");
    QString tmptcpAddressSort = GDataFactory::get_instance()->get_config_para("TCP_IP_ADDRESS_SORT");
    QString tmpTcpportSort = GDataFactory::get_instance()->get_config_para("TCP_IP_PORT_SORT");
    QString tmpCombaud = GDataFactory::get_instance()->get_config_para("COM_BARD_RATE");
    QString tmpComport = GDataFactory::get_instance()->get_config_para("COM_PORT");
    if(GDataFactory::get_instance()->get_config_para("IS_CONNECT_MODE") == "COM"){
        GDataFactory::get_modbus_client()->init_ip_port(MODBUS_TYPE::MODBUS_COM,tmpComport,tmpCombaud.toInt());
    }else if(GDataFactory::get_instance()->get_config_para("IS_CONNECT_MODE") == "TCP"){
        GDataFactory::get_modbus_client()->init_ip_port(MODBUS_TYPE::MODBUS_TCP,tmptcpAddress,tmpTcpport.toInt());
        GDataFactory::get_modbus_client_sort()->init_ip_port(MODBUS_TYPE::MODBUS_TCP,tmptcpAddressSort,tmpTcpportSort.toInt());
    }

    m_pTimerID = startTimer(2000);
    isRunTimer = true;

//    GDataFactory::get_plc_obj()->connectToPLC(GDataFactory::get_instance()->get_config_para("COM_PORT"),
//                                              GDataFactory::get_instance()->get_config_para("COM_BARD_RATE"));
//    m_pReadThread = new DemoAOIThread();
//    connect(m_pReadThread,SIGNAL(signal_plc_info_to_ui(const QByteArray&)),this,SLOT(slot_rev_from_plc(const QByteArray&)));
//    GDataFactory::get_plc_obj()->moveToThread(m_pReadThread);
//    m_pReadThread->start();
    m_pPrintThread = new PrintThread();
    m_pPrintThread->start();
}

void CentralWgt::showEvent(QShowEvent *event)
{
    Q_UNUSED(event);
    this->m_pLineEditExInfoA->setText(GDataFactory::get_instance()->get_config_para("EX_ORDER_A"));
    m_pExOrderIdA = GDataFactory::get_instance()->get_config_para("EX_ORDER_A");
    this->m_pLineEditExWidthA->setText(GDataFactory::get_instance()->get_config_para("EX_WIDTH_A"));
    m_pExOrderWidthA = GDataFactory::get_instance()->get_config_para("EX_WIDTH_A");
    this->m_pLineEditExInfoB->setText(GDataFactory::get_instance()->get_config_para("EX_ORDER_B"));
    m_pExOrderIdB = GDataFactory::get_instance()->get_config_para("EX_ORDER_B");
    this->m_pLineEditExWidthB->setText(GDataFactory::get_instance()->get_config_para("EX_WIDTH_B"));
    m_pExOrderWidthB = GDataFactory::get_instance()->get_config_para("EX_WIDTH_B");

    m_pExBoardTypeA = GDataFactory::get_instance()->get_config_para("EX_BOARD_TYPE_A");
    m_pExLineNameA = GDataFactory::get_instance()->get_config_para("EX_LINE_NAME_A");
    m_pExMaterialNumberA = GDataFactory::get_instance()->get_config_para("EX_MATERIAL_NUMBER_A");
    m_pExOrderNumberA = GDataFactory::get_instance()->get_config_para("EX_ORDER_NUMBER_A");
    m_pExSpliceQuantityA = GDataFactory::get_instance()->get_config_para("EX_SPLICE_QUANTITY_A");

    m_pExBoardTypeB = GDataFactory::get_instance()->get_config_para("EX_BOARD_TYPE_B");
    m_pExLineNameB = GDataFactory::get_instance()->get_config_para("EX_LINE_NAME_B");
    m_pExMaterialNumberB = GDataFactory::get_instance()->get_config_para("EX_MATERIAL_NUMBER_B");
    m_pExOrderNumberB = GDataFactory::get_instance()->get_config_para("EX_ORDER_NUMBER_B");
    m_pExSpliceQuantityB = GDataFactory::get_instance()->get_config_para("EX_SPLICE_QUANTITY_B");
}

void CentralWgt::timerEvent(QTimerEvent *event)
{
    if(event->timerId() == m_pTimerID)
    {
        if(isRunTimer)
        {
            //query PLC info
            if(GDataFactory::get_instance()->get_config_para("TRACK_TYPE") == "DOUBLE")
            {
                uint16_t data[20]={0};
                GDataFactory::get_modbus_client()->sendMessageToPLC(MODBUS_READ_SIGN,32200,6,data);
                modifyToolsWidthDouble(data);
            }
            else {
                uint16_t data[2]={0};
                GDataFactory::get_modbus_client()->sendMessageToPLC(MODBUS_READ_SIGN,32578,2,data);
                modifyToolsWidthSingle(data);
            }
        }
    }

    if(event->timerId() == m_pTimerMesID)
    {
        //query Mes info
//        this->m_pLineEditExInfoA->setText(m_pExOrderIdA);
//        this->m_pLineEditExWidthA->setText(m_pExOrderWidthA);
//        this->m_pLineEditExInfoB->setText(m_pExOrderIdB);
//        this->m_pLineEditExWidthB->setText(m_pExOrderWidthB);
//        GDataFactory::get_mes_process()->exec_http_post_zd();
    }
}

void CentralWgt::slot_rev_logs(const QString info)
{
    m_pTextEditLogInfo->append(info);
}

std::string qStringToMultiByte(const QString &str)
{
    QTextCodec *codec = QTextCodec::codecForName("UTF-8");

    return std::string(codec->toUnicode(str.toUtf8()).toStdString());
}

void CentralWgt::slot_btn_read_click()
{
    QDate currentDate = QDate::currentDate();
    QDate newDate = currentDate.addDays(7);

    QLOG_WARN()<<currentDate.toString("yyyy-MM-dd");
    QLOG_WARN()<<newDate.toString("yyyy-MM-dd");
    return;
//    QByteArray m_data;
//    m_data.append(static_cast<char>(0xBC));
//    m_data.append(static_cast<char>(0x07));
//    m_data.append(static_cast<char>(0x00));
//    m_data.append(static_cast<char>(0x00));
//    m_data.append(static_cast<char>(0x00));
//    m_data.append(static_cast<char>(0x00));
//    m_data.append(static_cast<char>(0x00));
//    m_data.append(static_cast<char>(0x00));

//    uint16_t data[20];
//    data[0] = m_data.toUShort();
//    GDataFactory::get_modbus_client()->sendMessageToPLC(MODBUS_WRITE_SIGN,32200,1,data);

//    QString binDir=QApplication::applicationDirPath();
//    binDir.append("/");
//    binDir.append(GDataFactory::get_instance()->get_config_para("FILE_NAME_A"));
//    std::vector<std::string> vc;
//    binDir=binDir.replace('/','\\');
//    vc.push_back(binDir.toStdString());
//    vc.push_back(GDataFactory::get_instance()->get_config_para("PRINTER_NAME_A").toStdString());
//    vc.push_back(GDataFactory::get_instance()->get_config_para("PRINTER_PORT_A").toStdString());
//    m_pBoardType = u8"海信测试";
//    m_pLineName = u8"测试线体";
//    QTextCodec *codec = QTextCodec::codecForName("GBK");
//    QByteArray typeArray = codec->fromUnicode(m_pBoardType);
//    QByteArray lineArray = codec->fromUnicode(m_pLineName);
//    vc.push_back(typeArray.toStdString());
//    vc.push_back(lineArray.toStdString());
//    vc.push_back(m_pMaterialNumber.toStdString());
//    vc.push_back(m_pOrderNumber.toStdString());
//    vc.push_back(QString::number(100).toStdString());
//    m_pPrintThread->set_print_info(vc,1);

//    uint16_t data[5];
//    data[0] = 0x1;
//    GDataFactory::get_modbus_client()->sendMessageToPLC(MODBUS_WRITE_SIGN,32210,1,data);


    uint16_t data[2];
//    /***********************单轨调宽********************/
    //****筛选机
    data[0] = 1;
    GDataFactory::get_modbus_client_sort()->sendMessageToPLC(MODBUS_WRITE_SIGN,32530,1,data);
    data[0] = 1200;
    GDataFactory::get_modbus_client_sort()->sendMessageToPLC(MODBUS_WRITE_SIGN,32480,1,data);
    //***筛选机

    //***收板机
    data[0] = 1500;
    GDataFactory::get_modbus_client()->sendMessageToPLC(MODBUS_WRITE_SIGN,32576,1,data);
//    /***********************单轨调宽********************/

    /***********************单轨退框********************/
//    data[0] = 1;
//    GDataFactory::get_modbus_client()->sendMessageToPLC(MODBUS_WRITE_SIGN,32577,1,data);
    /***********************单轨退框********************/


//    /***********************双轨调宽********************/
//    /*筛选机1轨*/
//    data[0] = 1;
//    GDataFactory::get_modbus_client_sort()->sendMessageToPLC(MODBUS_WRITE_SIGN,32590,1,data);

//    data[0] = 1400;
//    GDataFactory::get_modbus_client_sort()->sendMessageToPLC(MODBUS_WRITE_SIGN,32480,1,data);
//    /*筛选机2轨*/
//    data[0] = 1;
//    GDataFactory::get_modbus_client_sort()->sendMessageToPLC(MODBUS_WRITE_SIGN,32592,1,data);

//    data[0] = 1500;
//    GDataFactory::get_modbus_client_sort()->sendMessageToPLC(MODBUS_WRITE_SIGN,32490,1,data);

//    /*收板机1轨*/
//    data[0] = 1600;
//    GDataFactory::get_modbus_client()->sendMessageToPLC(MODBUS_WRITE_SIGN,32572,1,data);
//    /*收板机2轨*/
//    data[0] = 1700;
//    GDataFactory::get_modbus_client()->sendMessageToPLC(MODBUS_WRITE_SIGN,32576,1,data);
//    /***********************双轨调宽********************/

    //双轨退框
//    data[0] = 1;
//    GDataFactory::get_modbus_client()->sendMessageToPLC(MODBUS_WRITE_SIGN,32210,1,data);
//    GDataFactory::get_modbus_client()->sendMessageToPLC(MODBUS_WRITE_SIGN,32211,1,data);
}

static bool writeSign = true;
void CentralWgt::slot_btn_write_click()
{
    uint16_t tmpdata[1];
    if(writeSign)
    {
        tmpdata[0] = 1;
        GDataFactory::get_modbus_client()->sendMessageToPLC(MODBUS_WRITE_SIGN,32578,1,tmpdata);
        writeSign = ~writeSign;
    }
    else {
        tmpdata[0] = 0;
        GDataFactory::get_modbus_client()->sendMessageToPLC(MODBUS_WRITE_SIGN,32578,1,tmpdata);
        writeSign = ~writeSign;
    }
//    uint16_t data[1];
//    data[0] = 1;

//    GDataFactory::get_modbus_client()->sendMessageToPLC(MODBUS_WRITE_SIGN,32210,1,data);

//    uint16_t data[2];
//    data[0] = 1550;
//    GDataFactory::get_modbus_client()->sendMessageToPLC(MODBUS_WRITE_SIGN,32576,1,data);

    //筛选机
//    uint16_t data[2];

//    data[0] = 1;
//    GDataFactory::get_modbus_client()->sendMessageToPLC(MODBUS_WRITE_SIGN,32592,1,data);

//    data[0] = 1450;
//    GDataFactory::get_modbus_client()->sendMessageToPLC(MODBUS_WRITE_SIGN,32490,1,data);


//    QString binDir=QApplication::applicationDirPath();
//    binDir.append("/");
//    binDir.append(GDataFactory::get_instance()->get_config_para("FILE_NAME_B"));
//    std::vector<std::string> vc;
//    binDir=binDir.replace('/','\\');
//    vc.push_back(binDir.toStdString());
//    vc.push_back(GDataFactory::get_instance()->get_config_para("PRINTER_NAME_B").toStdString());
//    vc.push_back(GDataFactory::get_instance()->get_config_para("PRINTER_PORT_B").toStdString());
//    QTextCodec *codec = QTextCodec::codecForName("GBK");
//    QByteArray typeArray = codec->fromUnicode(m_pBoardType);
//    QByteArray lineArray = codec->fromUnicode(m_pLineName);
//    vc.push_back(typeArray.toStdString());
//    vc.push_back(lineArray.toStdString());
//    vc.push_back(m_pMaterialNumber.toStdString());
//    vc.push_back(m_pOrderNumber.toStdString());
//    vc.push_back(QString::number(100).toStdString());
//    m_pPrintThread->set_print_info(vc,2);

//    uint16_t data[5];
//    data[0] = 0x1;
//    GDataFactory::get_modbus_client()->sendMessageToPLC(MODBUS_WRITE_SIGN,32211,1,data);


}

void CentralWgt::slot_btn_set_para_click()
{

}

void CentralWgt::modifyToolsWidthDouble(uint16_t *data)
{
    isRunTimer = false;
    if(data[0] == 1){
        uint16_t tmpdata[1];
        tmpdata[0] = 0;
        GDataFactory::get_modbus_client()->sendMessageToPLC(MODBUS_WRITE_SIGN,32200,1,tmpdata);

        QLOG_WARN()<<u8"1轨出框";
        //the quantity of the first track
        int firstQuantity = data[1];
        QLOG_WARN()<<u8"1轨的数量为:"<<firstQuantity;
        int tmpQ = m_pExSpliceQuantityA.toShort();
        firstQuantity = tmpQ*firstQuantity;

        QString binDir=QApplication::applicationDirPath();
        binDir.append("/");
        binDir.append(GDataFactory::get_instance()->get_config_para("FILE_NAME_A"));
        std::vector<std::string> vc;
        binDir=binDir.replace('/','\\');
        vc.push_back(binDir.toStdString());
        vc.push_back(GDataFactory::get_instance()->get_config_para("PRINTER_NAME_A").toStdString());
        vc.push_back(GDataFactory::get_instance()->get_config_para("PRINTER_PORT_A").toStdString());
        QTextCodec *codec = QTextCodec::codecForName("GBK");
        QByteArray typeArray = codec->fromUnicode(m_pExBoardTypeA);
        QByteArray lineArray = codec->fromUnicode(m_pExLineNameA);
        vc.push_back(typeArray.toStdString());
        vc.push_back(lineArray.toStdString());
        vc.push_back(m_pExMaterialNumberA.toStdString());
        vc.push_back(m_pExOrderNumberA.toStdString());
        vc.push_back(QString::number(firstQuantity).toStdString());
        m_pPrintThread->set_print_info(vc,1);

        m_pExBoardTypeA = m_pBoardTypeA;
        GDataFactory::get_instance()->set_config_para_1("EX_BOARD_TYPE_A",m_pExBoardTypeA);
        m_pExLineNameA = m_pLineNameA;
        GDataFactory::get_instance()->set_config_para_1("EX_LINE_NAME_A",m_pExLineNameA);
        m_pExMaterialNumberA = m_pMaterialNumberA;
        GDataFactory::get_instance()->set_config_para_1("EX_MATERIAL_NUMBER_A",m_pExMaterialNumberA);
        m_pExOrderNumberA = m_pOrderNumberA;
        GDataFactory::get_instance()->set_config_para_1("EX_ORDER_NUMBER_A",m_pExOrderNumberA);
        m_pExSpliceQuantityA = m_pSpliceQuantityA;
        GDataFactory::get_instance()->set_config_para_1("EX_SPLICE_QUANTITY_A",m_pExSpliceQuantityA);

        GDataFactory::get_modbus_client()->sendMessageToPLC(MODBUS_WRITE_SIGN,32200,1,tmpdata);
    }

    if(data[3] == 1){
        uint16_t tmpdata[1];
        tmpdata[0] = 0;
        GDataFactory::get_modbus_client()->sendMessageToPLC(MODBUS_WRITE_SIGN,32203,1,tmpdata);
        QLOG_WARN()<<u8"2轨出框";

        //the quantity of the second track
        int secondQuantity = data[4];
        QLOG_WARN()<<u8"2轨的数量为:"<<secondQuantity;
        int tmpQ = m_pExSpliceQuantityB.toShort();
        secondQuantity = tmpQ*secondQuantity;

        QString binDir=QApplication::applicationDirPath();
        binDir.append("/");
        binDir.append(GDataFactory::get_instance()->get_config_para("FILE_NAME_B"));
        std::vector<std::string> vc;
        binDir=binDir.replace('/','\\');
        vc.push_back(binDir.toStdString());
        vc.push_back(GDataFactory::get_instance()->get_config_para("PRINTER_NAME_B").toStdString());
        vc.push_back(GDataFactory::get_instance()->get_config_para("PRINTER_PORT_B").toStdString());
        QTextCodec *codec = QTextCodec::codecForName("GBK");
        QByteArray typeArray = codec->fromUnicode(m_pExBoardTypeB);
        QByteArray lineArray = codec->fromUnicode(m_pExLineNameB);
        vc.push_back(typeArray.toStdString());
        vc.push_back(lineArray.toStdString());
        vc.push_back(m_pExMaterialNumberB.toStdString());
        vc.push_back(m_pExOrderNumberB.toStdString());
        vc.push_back(QString::number(secondQuantity).toStdString());
        m_pPrintThread->set_print_info(vc,2);

        m_pExBoardTypeB = m_pBoardTypeB;
        GDataFactory::get_instance()->set_config_para_1("EX_BOARD_TYPE_B",m_pExBoardTypeB);
        m_pExLineNameB = m_pLineNameB;
        GDataFactory::get_instance()->set_config_para_1("EX_LINE_NAME_B",m_pExLineNameB);
        m_pExMaterialNumberB = m_pMaterialNumberB;
        GDataFactory::get_instance()->set_config_para_1("EX_MATERIAL_NUMBER_B",m_pExMaterialNumberB);
        m_pExOrderNumberB = m_pOrderNumberB;
        GDataFactory::get_instance()->set_config_para_1("EX_ORDER_NUMBER_B",m_pExOrderNumberB);
        m_pExSpliceQuantityB = m_pSpliceQuantityB;
        GDataFactory::get_instance()->set_config_para_1("EX_SPLICE_QUANTITY_B",m_pExSpliceQuantityB);

        GDataFactory::get_modbus_client()->sendMessageToPLC(MODBUS_WRITE_SIGN,32203,1,tmpdata);
    }
    isRunTimer = true;
}

void CentralWgt::modifyToolsWidthSingle(uint16_t* data)
{
    isRunTimer = false;
    if(data[0] == 1){
        uint16_t tmpdata[1];
        tmpdata[0] = 0;
        GDataFactory::get_modbus_client()->sendMessageToPLC(MODBUS_WRITE_SIGN,32578,1,tmpdata);

        QLOG_WARN()<<u8"单轨出框";
        //the quantity of the first track
        int firstQuantity = data[1];
        QLOG_WARN()<<u8"单轨的数量为:"<<firstQuantity;
        int tmp = m_pExSpliceQuantityA.toInt();
        firstQuantity = firstQuantity*tmp;

        QString binDir=QApplication::applicationDirPath();
        binDir.append("/");
        binDir.append(GDataFactory::get_instance()->get_config_para("FILE_NAME_A"));
        std::vector<std::string> vc;
        binDir=binDir.replace('/','\\');
        vc.push_back(binDir.toStdString());
        vc.push_back(GDataFactory::get_instance()->get_config_para("PRINTER_NAME_A").toStdString());
        vc.push_back(GDataFactory::get_instance()->get_config_para("PRINTER_PORT_A").toStdString());
        QTextCodec *codec = QTextCodec::codecForName("GBK");
        QByteArray typeArray = codec->fromUnicode(m_pExBoardTypeA);
        QByteArray lineArray = codec->fromUnicode(m_pExLineNameA);
        vc.push_back(typeArray.toStdString());
        vc.push_back(lineArray.toStdString());
        vc.push_back(m_pExMaterialNumberA.toStdString());
        vc.push_back(m_pExOrderNumberA.toStdString());
        vc.push_back(QString::number(firstQuantity).toStdString());
        m_pPrintThread->set_print_info(vc,1);

        m_pExBoardTypeA = m_pBoardTypeA;
        GDataFactory::get_instance()->set_config_para_1("EX_BOARD_TYPE_A",m_pExBoardTypeA);
        m_pExLineNameA = m_pLineNameA;
        GDataFactory::get_instance()->set_config_para_1("EX_LINE_NAME_A",m_pExLineNameA);
        m_pExMaterialNumberA = m_pMaterialNumberA;
        GDataFactory::get_instance()->set_config_para_1("EX_MATERIAL_NUMBER_A",m_pExMaterialNumberA);
        m_pExOrderNumberA = m_pOrderNumberA;
        GDataFactory::get_instance()->set_config_para_1("EX_ORDER_NUMBER_A",m_pExOrderNumberA);
        m_pExSpliceQuantityA = m_pSpliceQuantityA;
        GDataFactory::get_instance()->set_config_para_1("EX_SPLICE_QUANTITY_A",m_pExSpliceQuantityA);
    }
    isRunTimer = true;
}

void CentralWgt::slot_rev_from_plc(const QByteArray &data)
{
    QLOG_WARN()<<"rev plc data is:"<<GDataFactory::get_instance()->bytes_to_str(data);

    if(data.length() == 12)
        ;
    else {
        QLOG_WARN()<<"the length of data from PLC is not correct";
        m_pReadThread->set_start_sign(false);
        return;
    }
    QByteArray tmpData = data;

    //the first track out sign
    int firstOutSign;
    QByteArray array1;
    array1.append(tmpData[0]);
    array1.append(tmpData[1]);
    firstOutSign = array1.toHex().toInt(nullptr, 16);
    if(firstOutSign == 1)
    {
        QByteArray clearData;
        clearData.append(static_cast<char>(0x00));
        clearData.append(static_cast<char>(0x00));
        uint16_t data[20];
        data[0] = 0;
        //GDataFactory::get_plc_obj()->writeDataToPLC("32200",1,clearData);
        GDataFactory::get_modbus_client()->sendMessageToPLC(MODBUS_WRITE_SIGN,32200,1,data);


        QLOG_WARN()<<u8"1轨出框";
        QLOG_WARN()<<GDataFactory::get_instance()->bytes_to_str(tmpData);
        //the quantity of the first track
        int firstQuantity;
        QByteArray arrayQuantity1;
        arrayQuantity1.append(tmpData[2]);
        arrayQuantity1.append(tmpData[3]);
        firstQuantity = arrayQuantity1.toHex().toInt();
        QLOG_WARN()<<u8"1轨的数量为:"<<firstQuantity;

        AOI_ZebraPrinterDll dd;
        QString msg=QString::number(firstQuantity);
        QString str="VVVVVVVVVVVVVVVVVV";
        str.replace(0,msg.length(),msg);
        QString val;
        dd.printExcuteFunc(str.replace(0,msg.length(),msg),val);
    }

    //the second track out sign
    int secondOutSign;
    QByteArray array2;
    array2.append(tmpData[6]);
    array2.append(tmpData[7]);
    secondOutSign = array2.toHex().toInt(nullptr,16);
    if(secondOutSign == 1)
    {
        QByteArray clearData;
        clearData.append(static_cast<char>(0x00));
        clearData.append(static_cast<char>(0x00));
//        GDataFactory::get_plc_obj()->writeDataToPLC("32203",1,clearData);
        uint16_t data[5];
        data[0] = 0;
        GDataFactory::get_modbus_client()->sendMessageToPLC(MODBUS_WRITE_SIGN,32203,1,data);
        QLOG_WARN()<<u8"2轨出框";
        QLOG_WARN()<<GDataFactory::get_instance()->bytes_to_str(tmpData);

        //the quantity of the second track
        int secondQuantity;
        QByteArray arrayQuantity2;
        arrayQuantity2.append(tmpData[8]);
        arrayQuantity2.append(tmpData[9]);
        secondQuantity = arrayQuantity2.toHex().toInt();
        QLOG_WARN()<<u8"2轨的数量为:"<<secondQuantity;

        AOI_ZebraPrinterDll dd;
        QString msg=QString::number(secondQuantity);
        QString str="VVVVVVVVVVVVVVVVVV";
        str.replace(0,msg.length(),msg);
        QString val;
        dd.printExcuteFunc(str.replace(0,msg.length(),msg),val);
    }


    m_pReadThread->set_start_sign(false);
}

void CentralWgt::slot_rev_mes_info_to_ui(const QJsonObject &obj)
{
    this->m_pLineEditExInfoA->setText(m_pExOrderIdA);
    this->m_pLineEditExWidthA->setText(m_pExOrderWidthA);
    this->m_pLineEditExInfoB->setText(m_pExOrderIdB);
    this->m_pLineEditExWidthB->setText(m_pExOrderWidthB);

    QString lineName = obj.value(QStringLiteral("lineName")).toString();
    QLOG_WARN()<<"the line_name from Mes is:"<<lineName;
    if(lineName.contains(GDataFactory::get_instance()->get_config_para("LINE_PARA")))
        ;
    else
        return;
    QString produceCmdNumber = obj.value(QStringLiteral("produceCmdNumber")).toString();
    QString productSN = obj.value(QStringLiteral("productSN")).toString();
    QLOG_WARN()<<"the product SN from Mes is:"<<productSN;
    QString productWidth = obj.value(QStringLiteral("productWidth")).toString();
    QString spliceQuantity = obj.value(QStringLiteral("spliceQuantity")).toString();
//    m_pSpliceQuantity = spliceQuantity;
    QString materialNumber = obj.value(QStringLiteral("materialNumber")).toString();
    QString orderNumber = obj.value(QStringLiteral("orderNumber")).toString();
    QString boardType = obj.value(QStringLiteral("boardType")).toString();
    QLOG_WARN()<<"the board-type from Mes is:"<<boardType;
    QString trackNumber = obj.value(QStringLiteral("trackNumber")).toString();

    if(GDataFactory::get_instance()->get_config_para("TRACK_TYPE") == "DOUBLE")
    {
        //1-track
        if(trackNumber == "1")
        {
            m_pLineNameA = lineName;
            m_pMaterialNumberA = materialNumber;
            m_pOrderNumberA = orderNumber;
            m_pBoardTypeA = boardType;
            m_pSpliceQuantityA = spliceQuantity;
            QLOG_WARN()<<u8"the width of track-1 is:"<<productWidth;
            QLOG_WARN()<<u8"the produceCmdNumber of track-1 is:"<<produceCmdNumber;
            this->m_pLineEditCurrentInfoA->setText(produceCmdNumber);
            this->m_pLineEditCurrentWidthA->setText(productWidth);
            GDataFactory::get_instance()->set_config_para_1("EX_ORDER_A",produceCmdNumber);
            GDataFactory::get_instance()->set_config_para_1("EX_WIDTH_A",productWidth);

            if(m_pExOrderIdA == produceCmdNumber)
            {
                if(m_pExOrderWidthA != productWidth)//width is different,alter width
                {
                    double w = productWidth.toDouble();
                    w = w*10.0;
                    uint16_t wd = static_cast<uint16_t>(w);
                    uint16_t data[5];

                    data[0] = wd;
                    for(int i=1;i<4;i++){
                        data[i] = 0;
                    }

                    if(MODBUS_STATUS::RES_OK ==  GDataFactory::get_modbus_client()->sendMessageToPLC(MODBUS_WRITE_SIGN,32572,4,data)){
                        QLOG_INFO()<<"Put_Away:order is same,1-track alter width SUCCESS!";
                    }else{
                        QLOG_WARN()<<"Put_Away:order is same,1-track alter width FAILED!";
                    }
                    QLOG_WARN()<<u8"Put_Away:1-track 制令单一致，宽度不一致，调宽";

                    uint16_t dataSort[1]; dataSort[0] = 1;
                    if(MODBUS_STATUS::RES_OK ==  GDataFactory::get_modbus_client_sort()->sendMessageToPLC(MODBUS_WRITE_SIGN,32590,1,dataSort))
                    {
                        uint16_t dataSortWidth[2];
                        dataSortWidth[0] = wd;
                        dataSortWidth[1] = 0;
                        if(MODBUS_STATUS::RES_OK ==  GDataFactory::get_modbus_client_sort()->sendMessageToPLC(MODBUS_WRITE_SIGN,32480,2,dataSortWidth))
                            QLOG_INFO()<<"Sort:order is same,1-track alter width SUCCESS!";
                        else
                            QLOG_WARN()<<"Sort:order is same,1-track alter width FAILED inner!";
                    }else{
                        QLOG_WARN()<<"Sort:order is same,1-track alter width FAILED!";
                    }
                    QLOG_WARN()<<u8"Sort:1-track 制令单一致，宽度不一致，调宽";
                }
            }
            else//produce-cmd is different, retreat the box
            {
                if(m_pExOrderWidthA == productWidth)//width is same,retreat box
                {
                    QByteArray m_data;
                    m_data.append(static_cast<char>(0x01));
                    m_data.append(static_cast<char>(0x00));

                    uint16_t data[5];
                    data[0] = 0x1;
                    if(MODBUS_STATUS::RES_OK ==  GDataFactory::get_modbus_client()->sendMessageToPLC(MODBUS_WRITE_SIGN,32210,1,data)){
                        QLOG_INFO()<<"order is different,1-track retreat box SUCCESS!";
                    }else{
                        QLOG_WARN()<<"order is different,1-track retreat box FAILED!";
                    }

                    QLOG_WARN()<<u8"1-track 制令单不一致，宽度一致，退框";
                }
                else//width is different,alter the width
                {
                    double w = productWidth.toDouble();
                    w = w*10.0;
                    uint16_t wd = static_cast<uint16_t>(w);
                    uint16_t data[5];

                    data[0] = wd;
                    for(int i=1;i<4;i++){
                        data[i] = 0;
                    }
                    if(MODBUS_STATUS::RES_OK ==  GDataFactory::get_modbus_client()->sendMessageToPLC(MODBUS_WRITE_SIGN,32572,4,data)){
                        QLOG_INFO()<<"Double Put_Away:order is different,1-track retreat box SUCCESS!";
                    }else{
                        QLOG_WARN()<<"Double Put_Away:order is different,1-track retreat box FAILED!";
                    }

                    QLOG_WARN()<<u8"1-track 制令单不一致，宽度不一致，调宽";

                    uint16_t dataSort[1]; dataSort[0] = 1;
                    if(MODBUS_STATUS::RES_OK ==  GDataFactory::get_modbus_client_sort()->sendMessageToPLC(MODBUS_WRITE_SIGN,32590,1,dataSort))
                    {
                        uint16_t dataSortWidth[2];
                        dataSortWidth[0] = wd;
                        dataSortWidth[1] = 0;
                        if(MODBUS_STATUS::RES_OK ==  GDataFactory::get_modbus_client_sort()->sendMessageToPLC(MODBUS_WRITE_SIGN,32480,2,dataSortWidth))
                            QLOG_INFO()<<"Double Sort:order is same,1-track alter width SUCCESS!";
                        else
                            QLOG_WARN()<<"Double Sort:order is same,1-track alter width FAILED inner!";
                    }else{
                        QLOG_WARN()<<"Double Sort:order is same,1-track alter width FAILED!";
                    }
                    QLOG_WARN()<<u8"Double Sort:1-track 制令单一致，宽度不一致，调宽";
                }
            }
            m_pExOrderIdA = produceCmdNumber;
            m_pExOrderWidthA = productWidth;
        }
        else//2-track
        {
            m_pLineNameB = lineName;
            m_pMaterialNumberB = materialNumber;
            m_pOrderNumberB = orderNumber;
            m_pBoardTypeB = boardType;
            m_pSpliceQuantityB = spliceQuantity;
            QLOG_WARN()<<u8"the width of track-2 is:"<<productWidth;
            QLOG_WARN()<<u8"the produceCmdNumber of track-2 is:"<<produceCmdNumber;
            this->m_pLineEditCurrentInfoB->setText(produceCmdNumber);
            this->m_pLineEditCurrentWidthB->setText(productWidth);
            GDataFactory::get_instance()->set_config_para_1("EX_ORDER_B",produceCmdNumber);
            GDataFactory::get_instance()->set_config_para_1("EX_WIDTH_B",productWidth);

            if(m_pExOrderIdB == produceCmdNumber)
            {
                if(m_pExOrderWidthB != productWidth)//width is different,alter width
                {
                    double w = productWidth.toDouble();
                    w = w*10.0;
                    uint16_t wd = static_cast<uint16_t>(w);

                    uint16_t data[5];

                    data[0] = wd;
                    for(int i=1;i<4;i++){
                        data[i] = 0;
                    }
                    if(MODBUS_STATUS::RES_OK ==  GDataFactory::get_modbus_client()->sendMessageToPLC(MODBUS_WRITE_SIGN,32576,4,data)){
                        QLOG_INFO()<<"Double Put_Away:order is different,2-track alter width SUCCESS!";
                    }else{
                        QLOG_WARN()<<"Double Put_Away:order is different,2-track alter width FAILED!";
                    }
                    QLOG_WARN()<<u8"Double Put_Away:2-track 制令单一致，宽度不一致，调宽";

                    uint16_t dataSort[1]; dataSort[0] = 1;
                    if(MODBUS_STATUS::RES_OK ==  GDataFactory::get_modbus_client_sort()->sendMessageToPLC(MODBUS_WRITE_SIGN,32592,1,dataSort))
                    {
                        uint16_t dataSortWidth[2];
                        dataSortWidth[0] = wd;
                        dataSortWidth[1] = 0;
                        if(MODBUS_STATUS::RES_OK ==  GDataFactory::get_modbus_client_sort()->sendMessageToPLC(MODBUS_WRITE_SIGN,32490,2,dataSortWidth))
                            QLOG_INFO()<<"Double Sort:order is same,2-track alter width SUCCESS!";
                        else
                            QLOG_WARN()<<"Double Sort:order is same,2-track alter width FAILED inner!";
                    }else{
                        QLOG_WARN()<<"Double Sort:order is same,2-track alter width FAILED!";
                    }
                    QLOG_WARN()<<u8"Double Sort:2-track 制令单一致，宽度不一致，调宽";
                }
            }
            else
            {
                if(m_pExOrderWidthB == productWidth)//width is same,retreat box
                {
                    uint16_t data[5];
                    data[0] = 0x1;
                    if(MODBUS_STATUS::RES_OK ==  GDataFactory::get_modbus_client()->sendMessageToPLC(MODBUS_WRITE_SIGN,32211,1,data)){
                        QLOG_INFO()<<"Double order is different,2-track retreat box SUCCESS!";
                    }else{
                        QLOG_WARN()<<"Double order is different,2-track retreat box FAILED!";
                    }

                    QLOG_WARN()<<u8"Double 2-track 制令单不一致，宽度一致，退框";
                }
                else//width is different,alter the width
                {
                    double w = productWidth.toDouble();
                    w = w*10.0;
                    uint16_t wd = static_cast<uint16_t>(w);

                    uint16_t data[5];

                    data[0] = wd;
                    for(int i=1;i<4;i++){
                        data[i] = 0;
                    }
                    if(MODBUS_STATUS::RES_OK ==  GDataFactory::get_modbus_client()->sendMessageToPLC(MODBUS_WRITE_SIGN,32576,4,data)){
                        QLOG_INFO()<<"order is different,2-track alter width SUCCESS!";
                    }else{
                        QLOG_WARN()<<"order is different,2-track alter width FAILED!";
                    }
                    QLOG_WARN()<<u8"2-track 制令单不一致，宽度不一致，调宽";

                    uint16_t dataSort[1]; dataSort[0] = 1;
                    if(MODBUS_STATUS::RES_OK ==  GDataFactory::get_modbus_client_sort()->sendMessageToPLC(MODBUS_WRITE_SIGN,32592,1,dataSort))
                    {
                        uint16_t dataSortWidth[2];
                        dataSortWidth[0] = wd;
                        dataSortWidth[1] = 0;
                        if(MODBUS_STATUS::RES_OK ==  GDataFactory::get_modbus_client_sort()->sendMessageToPLC(MODBUS_WRITE_SIGN,32490,2,dataSortWidth))
                            QLOG_INFO()<<"Sort:order is same,2-track alter width SUCCESS!";
                        else
                            QLOG_WARN()<<"Sort:order is same,2-track alter width FAILED inner!";
                    }else{
                        QLOG_WARN()<<"Sort:order is same,2-track alter width FAILED!";
                    }
                    QLOG_WARN()<<u8"Sort:2-track 制令单一致，宽度不一致，调宽";
                }
            }
            m_pExOrderIdB = produceCmdNumber;
            m_pExOrderWidthB = productWidth;
        }
    }
    else
    {
        m_pLineNameA = lineName;
        m_pMaterialNumberA = materialNumber;
        m_pOrderNumberA = orderNumber;
        m_pBoardTypeA = boardType;
        m_pSpliceQuantityA = spliceQuantity;
        QLOG_WARN()<<u8"the width of track-1 is:"<<productWidth;
        QLOG_WARN()<<u8"the produceCmdNumber of track-1 is:"<<produceCmdNumber;
        this->m_pLineEditCurrentInfoA->setText(produceCmdNumber);
        this->m_pLineEditCurrentWidthA->setText(productWidth);
        GDataFactory::get_instance()->set_config_para_1("EX_ORDER_A",produceCmdNumber);
        GDataFactory::get_instance()->set_config_para_1("EX_WIDTH_A",productWidth);

        if(m_pExOrderIdA == produceCmdNumber)
        {
            if(m_pExOrderWidthA != productWidth)//width is different,alter width
            {
                double w = productWidth.toDouble();
                w = w*10.0;
                uint16_t wd = static_cast<uint16_t>(w);
                uint16_t data[1];
                data[0] = wd;

                if(MODBUS_STATUS::RES_OK ==  GDataFactory::get_modbus_client()->sendMessageToPLC(MODBUS_WRITE_SIGN,32576,1,data)){
                    QLOG_INFO()<<"Single Put_Away:order is same,1-track alter width SUCCESS!";
                }else{
                    QLOG_WARN()<<"Single Put_Away:order is same,1-track alter width FAILED!";
                }
                QLOG_WARN()<<u8"Single Put_Away:1-track 制令单一致，宽度不一致，调宽";

                uint16_t dataSort[1]; dataSort[0] = 1;
                if(MODBUS_STATUS::RES_OK ==  GDataFactory::get_modbus_client_sort()->sendMessageToPLC(MODBUS_WRITE_SIGN,32530,1,dataSort))
                {
                    uint16_t dataSortWidth[2];
                    dataSortWidth[0] = wd;
                    dataSortWidth[1] = 0;
                    if(MODBUS_STATUS::RES_OK ==  GDataFactory::get_modbus_client_sort()->sendMessageToPLC(MODBUS_WRITE_SIGN,32480,2,dataSortWidth))
                        QLOG_INFO()<<"Single Sort:order is same,1-track alter width SUCCESS!";
                    else
                        QLOG_WARN()<<"Single Sort:order is same,1-track alter width FAILED inner!";
                }else{
                    QLOG_WARN()<<"Single Sort:order is same,1-track alter width FAILED!";
                }
                QLOG_WARN()<<u8"Single Sort:1-track 制令单一致，宽度不一致，调宽";
            }
        }
        else//produce-cmd is different, retreat the box
        {
            if(m_pExOrderWidthA == productWidth)//width is same,retreat box
            {
                QByteArray m_data;
                m_data.append(static_cast<char>(0x01));
                m_data.append(static_cast<char>(0x00));

                uint16_t data[5];
                data[0] = 0x1;
                if(MODBUS_STATUS::RES_OK ==  GDataFactory::get_modbus_client()->sendMessageToPLC(MODBUS_WRITE_SIGN,32210,1,data)){
                    QLOG_INFO()<<"Single Sort:order is different,1-track retreat box SUCCESS!";
                }else{
                    QLOG_WARN()<<"Single Sort:order is different,1-track retreat box FAILED!";
                }

                QLOG_WARN()<<u8"Single 1-track 制令单不一致，宽度一致，退框";
            }
            else//width is different,alter the width
            {
                double w = productWidth.toDouble();
                w = w*10.0;
                uint16_t wd = static_cast<uint16_t>(w);
                uint16_t data[5];
                data[0] = wd;

                if(MODBUS_STATUS::RES_OK ==  GDataFactory::get_modbus_client()->sendMessageToPLC(MODBUS_WRITE_SIGN,32576,1,data)){
                    QLOG_INFO()<<"Single Put_Away:order is different,1-track retreat box SUCCESS!";
                }else{
                    QLOG_WARN()<<"Single Put_Away:order is different,1-track retreat box FAILED!";
                }

                QLOG_WARN()<<u8"Single Put_Away:1-track 制令单不一致，宽度不一致，调宽";

                uint16_t dataSort[1]; dataSort[0] = 1;
                if(MODBUS_STATUS::RES_OK ==  GDataFactory::get_modbus_client_sort()->sendMessageToPLC(MODBUS_WRITE_SIGN,32530,1,dataSort))
                {
                    uint16_t dataSortWidth[2];
                    dataSortWidth[0] = wd;
                    dataSortWidth[1] = 0;
                    if(MODBUS_STATUS::RES_OK ==  GDataFactory::get_modbus_client_sort()->sendMessageToPLC(MODBUS_WRITE_SIGN,32480,2,dataSortWidth))
                        QLOG_INFO()<<"Single Sort:order is same,1-track alter width SUCCESS!";
                    else
                        QLOG_WARN()<<"Sort:order is same,1-track alter width FAILED inner!";
                }else{
                    QLOG_WARN()<<"Single Sort:order is same,1-track alter width FAILED!";
                }
                QLOG_WARN()<<u8"Single Sort:1-track 制令单一致，宽度不一致，调宽";
            }
        }
        m_pExOrderIdA = produceCmdNumber;
        m_pExOrderWidthA = productWidth;
    }
}

PrintThread::PrintThread()
{

}

void PrintThread::run()
{
    while(1)
    {
        std::lock_guard<std::mutex> lock(mutex);
        if((m_pPrintInfoVector.size() == 0)&&(m_pPrintInfoVector2.size() == 0))
        {
            QEventLoop eventloop;
            QTimer::singleShot(200,&eventloop,&QEventLoop::quit);
            eventloop.exec();
            continue;
        }

        if(m_pPrintInfoVector.size() != 0)
        {
            QLOG_WARN()<<u8"1轨开始打印";
            print_according_to_para(m_pPrintInfoVector);
            m_pPrintInfoVector.clear();
            QLOG_WARN()<<u8"1轨打印完成";
        }

        if(m_pPrintInfoVector2.size() != 0)
        {
            QLOG_WARN()<<u8"2轨开始打印";
            print_according_to_para(m_pPrintInfoVector2);
            m_pPrintInfoVector2.clear();
            QLOG_WARN()<<u8"2轨打印完成";
        }
    }
}
