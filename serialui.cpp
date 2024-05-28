#include "serialui.h"
#include <QDebug>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickView>
#include <QComboBox>
#include <QQmlEngine>
#include <QQuickItem>
#include <QThread>

#include <iostream>
#include <cstring>

#include "comboboxport.h"
#include "receivedata.h"
#include "userinputmessage.h"
#include "csendmessage.h"
#include "comboboxmanager.h"
#include "cbaudratecombo.h"

extern ReceiveData      backend;
extern CSendMessage     g_sndmsgObj;
extern ComboBoxPort     combo;
extern userInputMessage txtObjInput;
extern CBaudrateCombo   baudratecombo;

const QString rcvlabel = "Received Bytes: ";
const QString sndlabel = "Send Bytes: ";

SerialUI::SerialUI(QObject *parent) : QObject(parent)
{
    m_thread = nullptr;
    m_isrunning = false;
    m_isrepeating = false;

    m_sndnum = 0;
    m_rcvnum = 0;
}

void SerialUI::autoRepeat(bool value)
{
    if (!isrunning()) {
        return;
    }

    if (isrepeatrunning()) {
        setrepeatrunning(value);
        return;
    } else {
        setrepeatrunning(value);
    }

    QThread *repeathread = QThread::create([this]() {
        while(isrepeatrunning() && isrunning()) {
            QString textFieldText = txtObjInput.gettxtInput();
            QByteArray sndmsg = textFieldText.toUtf8();
            m_puart->Write(sndmsg,  sndmsg.length());
            g_sndmsgObj.sendMessageToQml(textFieldText + " ");
            m_sndnum += sndmsg.length();
            setsendText(sndlabel + QString::number(m_sndnum));
            QThread::msleep(1000);
        }
    });

    connect(repeathread, &QThread::finished, repeathread, &QObject::deleteLater);

    repeathread->start();
}

void SerialUI::btnSerialSend()
{
    //int index = combo.currentIndex();
    QString textFieldText = txtObjInput.gettxtInput();
    g_sndmsgObj.sendMessageToQml(textFieldText + "");

    for (auto it = m_chkValMap.begin(); it != m_chkValMap.end(); ++it) {
        qDebug() << "key: " << it.key() << " value: " << it.value();
    }

    if (!isrunning()) {
        qDebug() << " please open serial before sending any data!\n";
        return;
    }

    bool repeat = m_chkValMap["chkautorepeat"];
    if (repeat) {
        autoRepeat(true);
    } else {
        QByteArray sndmsg = textFieldText.toUtf8();
        m_puart->Write(sndmsg, sndmsg.length());
        m_sndnum += sndmsg.length();
        setsendText(sndlabel + QString::number(m_sndnum));
    }
}

bool SerialUI::isrunning() {

     m_mutex.lock();
     bool running = m_isrunning;
     m_mutex.unlock();

     return running;
}

void SerialUI::setrunning(bool status)
{
    m_mutex.lock();
    m_isrunning = status;
    m_mutex.unlock();
}

bool SerialUI::isrepeatrunning()
{
    m_repeatmutex.lock();
    bool repeating = m_isrepeating;
    m_repeatmutex.unlock();

    return repeating;
}

void SerialUI::setrepeatrunning(bool status)
{
    m_repeatmutex.lock();
    m_isrepeating = status;
    m_repeatmutex.unlock();
}

void SerialUI::toolbaropen()
{
    constexpr int enumbd[3] = { B9600, B19200, B115200};
    int index  = combo.currentIndex();
    QString portname = combo.getcomboList()[index];

    index = baudratecombo.currentIndex();
    QString baudrate = baudratecombo.getcomboList()[index];
    if (index > 2) {
        backend.sendMessageToQml("This baudrate: " + baudrate + " doesn't support yet\n");
        return;
    }

    unsigned int bdrate  = enumbd[index];

    if (isrunning()) {
        backend.sendMessageToQml("serial is already running\n");
        return;
    }

    QString  ttydev = "/dev/" + portname;
    m_puart = std::make_shared<ndSerial>(ttydev.toStdString());
    if (0 != m_puart->Open()) {
        backend.sendMessageToQml("Open UART " + portname + " failed\n");
        return;
    }

    m_puart->ConfigureTermios(bdrate);
    m_puart->SetBlockMode(false);

    setrunning(true);

    QThread *thread = QThread::create([this]() {
        while(isrunning()) {
            unsigned char buf[2048] = { 0 };
            m_puart->Read(buf, sizeof(buf));
            int len = std::strlen((const char*)buf);
            if (len > 0 ) {
                QString msg = QString::fromUtf8(reinterpret_cast<char*>(buf), len) + "\n";
                backend.sendMessageToQml(msg);
                m_rcvnum += len;
                setrcvText(rcvlabel + QString::number(m_rcvnum));
            }
        }

        m_puart->Close();
        qDebug() << "Exit uart receiving thread" ;
    });

    connect(thread, &QThread::finished, thread, &QObject::deleteLater);

    thread->start();
}

void SerialUI::clearReceiveData()
{
    m_rcvnum = 0;
    setrcvText(rcvlabel + QString::number(m_rcvnum));
}

void SerialUI::clearSendData()
{
    m_sndnum = 0;
    setsendText(sndlabel + QString::number(m_sndnum));
}

void SerialUI::toolbarstop()
{
    setrunning(false);

    qDebug() << "click toolbar stop button\n";
}

void SerialUI::toolbarabout()
{
    qDebug() << "click toolbar about button\n";
}
