#ifndef DEMOAOITHREAD_H
#define DEMOAOITHREAD_H
#include <QThread>
#include <QMutex>
class DemoAOIThread : public QThread
{
    Q_OBJECT
public:
    DemoAOIThread();

    void set_start_sign(bool sign){m_pStartSign = sign;}

protected:
    void run();

    bool m_pStartSign;
    QMutex mutex;

signals:
    void signal_plc_info_to_ui(const QByteArray& data);
public slots:
};

#endif // DEMOAOITHREAD_H
