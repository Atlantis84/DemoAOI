#ifndef HEADINFOWGT_H
#define HEADINFOWGT_H

#include <QWidget>

class HeadInfoWgt : public QWidget
{
    Q_OBJECT

public:
    static HeadInfoWgt* get_instance()
    {
        if(m_pInstance == nullptr)
            m_pInstance = new HeadInfoWgt();
        return m_pInstance;
    }
private:
    explicit HeadInfoWgt(QWidget *parent = nullptr);

    static HeadInfoWgt* m_pInstance;

protected:
    void paintEvent(QPaintEvent *event);

signals:

public slots:
};

#endif // HEADINFOWGT_H
