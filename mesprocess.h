#ifndef MESPROCESS_H
#define MESPROCESS_H

#include <QObject>
#include <QNetworkReply>
#include <QNetworkAccessManager>
class MesProcess : public QObject
{
    Q_OBJECT    
private:
    explicit MesProcess(QObject *parent = nullptr);
public:
    void exec_http_post();
    void exec_http_post_zd();
    void exec_http_get(QString productsn);
    void exec_http_common_get(const QByteArray& data,int sign);

    static MesProcess* m_pInstance;
    static MesProcess* get_instance()
    {
        if(m_pInstance == nullptr)
            m_pInstance = new MesProcess();
        return m_pInstance;
    }
private:
    QNetworkAccessManager* accessManager;
    QNetworkAccessManager* accessManagerCommon;

signals:
    void signal_to_ui(QByteArray data);
    void signal_to_common(const QByteArray& data);
public slots:
    void slot_urlReply(QNetworkReply* reply);
    void slot_urlReply_common(QNetworkReply* reply);
};

#endif // MESPROCESS_H
