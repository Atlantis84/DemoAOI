#ifndef CENTRALWGT_H
#define CENTRALWGT_H

#include <QObject>
#include <QWidget>
#include <QLineEdit>
#include <QTextEdit>
#include "demoaoithread.h"
#include <queue>
#include <mutex>
#include <condition_variable>

class PrintThread : public QThread{
    Q_OBJECT
public:
    PrintThread();
    void set_print_info(std::vector<std::string> vec, int printerID)
    {
        m_pPinterID = printerID;
        if(printerID == 1)
            m_pPrintInfoVector = vec;
        else
            m_pPrintInfoVector2 = vec;
    }
protected:
    void run();
private:
    std::queue<std::vector<std::string>> m_pQueue;
    std::vector<std::string> m_pPrintInfoVector;
    std::vector<std::string> m_pPrintInfoVector2;
    std::condition_variable condition;
    int m_pPinterID;
    std::mutex mutex;

};
class CentralWgt : public QWidget
{
    Q_OBJECT
public:
    static CentralWgt* get_instance()
    {
        if(m_pInstance == nullptr)
            m_pInstance = new CentralWgt();
        return m_pInstance;
    }

    void modifyToolsWidthDouble(uint16_t* data);
    void modifyToolsWidthSingle(uint16_t* data);

protected:
    void showEvent(QShowEvent *event);
    void timerEvent(QTimerEvent *event);
private:
    explicit CentralWgt(QWidget *parent = nullptr);
    static CentralWgt* m_pInstance;

    QLineEdit* m_pLineEditExInfoA;
    QLineEdit* m_pLineEditCurrentInfoA;
    QLineEdit* m_pLineEditExWidthA;
    QLineEdit* m_pLineEditCurrentWidthA;

    QLineEdit* m_pLineEditExInfoB;
    QLineEdit* m_pLineEditCurrentInfoB;
    QLineEdit* m_pLineEditExWidthB;
    QLineEdit* m_pLineEditCurrentWidthB;

    QLineEdit* m_pLineEditLineName;

    QTextEdit* m_pTextEditLogInfo;

    int m_pTimerID;
    int m_pTimerMesID;

    QString m_pExOrderIdA;
    QString m_pExOrderWidthA;
    QString m_pExOrderIdB;
    QString m_pExOrderWidthB;

//    QString m_pBoardType;
//    QString m_pLineName;
//    QString m_pMaterialNumber;
//    QString m_pOrderNumber;
//    QString m_pSpliceQuantity;

    QString m_pExBoardTypeA;
    QString m_pExLineNameA;
    QString m_pExMaterialNumberA;
    QString m_pExOrderNumberA;
    QString m_pExSpliceQuantityA;

    QString m_pBoardTypeA;
    QString m_pLineNameA;
    QString m_pMaterialNumberA;
    QString m_pOrderNumberA;
    QString m_pSpliceQuantityA;

    QString m_pExBoardTypeB;
    QString m_pExLineNameB;
    QString m_pExMaterialNumberB;
    QString m_pExOrderNumberB;
    QString m_pExSpliceQuantityB;

    QString m_pBoardTypeB;
    QString m_pLineNameB;
    QString m_pMaterialNumberB;
    QString m_pOrderNumberB;
    QString m_pSpliceQuantityB;

    DemoAOIThread* m_pReadThread;

    bool isRunTimer = false;

    PrintThread* m_pPrintThread;

signals:

public slots:
    void slot_rev_logs(const QString info);

    void slot_btn_read_click();
    void slot_btn_write_click();
    void slot_btn_set_para_click();

    void slot_rev_from_plc(const QByteArray& data);
    void slot_rev_mes_info_to_ui(const QJsonObject& obj);
};

#endif // CENTRALWGT_H
