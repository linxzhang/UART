#ifndef SERIALUI_H
#define SERIALUI_H

#include <QObject>
#include <QtDebug>
#include <QMap>
#include <QVariantMap>
#include <QMutex>
#include <memory>
#include "ndserial.h"

class SerialUI : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString sendbytesText READ getsendText WRITE setsendText NOTIFY sendTextChanged)
    Q_PROPERTY(QString rcvbytesText READ getrcvText WRITE setrcvText NOTIFY rcvTextChanged)

public:
    explicit SerialUI(QObject *parent = nullptr);
    Q_INVOKABLE void updateChkboxValMap(const QString &key, bool value) {
        m_chkValMap[key] = value;
        emit chkValMapChanged();
    }

    Q_INVOKABLE void autoRepeat(bool value);

public:
    bool isrunning();
    void setrunning(bool status);

    bool isrepeatrunning();
    void setrepeatrunning(bool status);

public slots:
    void clearReceiveData();
    void clearSendData();

    void btnSerialSend();
    void toolbaropen();
    void toolbarstop();
    void toolbarabout();

public:
    QString getsendText() const { return m_sendText; }
    void setsendText(const QString &text) {
        if (m_sendText == text) {
            return;
        }
        m_sendText = text;
        emit sendTextChanged();
    }

    QString getrcvText() const { return m_rcvText; }
    void setrcvText(const QString &text) {
        if (m_rcvText == text) {
            return;
        }
        m_rcvText = text;
        emit rcvTextChanged();
    }


signals:
    void chkValMapChanged();
    void sendTextChanged();
    void rcvTextChanged();

private:
    QMap<QString, bool> m_chkValMap;

    bool m_isrepeating;
    QMutex m_repeatmutex;

    bool m_isrunning;
    QMutex m_mutex;


    std::shared_ptr<ndSerial> m_puart;
    QThread *m_thread;

    QString m_sendText;
    QString m_rcvText;

    unsigned long m_rcvnum;
    unsigned long m_sndnum;
};

#endif // SERIALUI_H
