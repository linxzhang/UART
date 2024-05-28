#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickView>
#include <QComboBox>
#include <QTimer>
#include <QQmlComponent>

#include "serialui.h"
#include "csendmessage.h"
#include "receivedata.h"
#include "comboboxport.h"
#include "cbaudratecombo.h"
#include "comboboxmanager.h"
#include "userinputmessage.h"

CSendMessage     g_sndmsgObj;
ReceiveData      backend;
ComboBoxPort     combo;
CBaudrateCombo   baudratecombo;
ComboBoxManager  combomngr;
userInputMessage txtObjInput;

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    SerialUI serialUiInstance;

    serialUiInstance.setrcvText("Received Bytes: 0");
    serialUiInstance.setsendText("Send Bytes: 0");

    engine.rootContext()->setContextProperty("SerialUI", &serialUiInstance);
    engine.rootContext()->setContextProperty("backend", &backend);
    engine.rootContext()->setContextProperty("txt_sendmessage", &g_sndmsgObj);

    QStringList tmp;
    tmp << "ttysWK0" << "ttysWK1" << "ttysWK2" << "ttysWK3";
    combo.setComboList(tmp);

    /**
     * init uart baudrate combobox value
     */
    QStringList bdtmp;
    bdtmp << "9600" << "19200" << "115200";
    baudratecombo.setComboList(bdtmp);
    baudratecombo.setcurrentIndex(2);

    QQmlContext *ownContext = engine.rootContext();
    ownContext->setContextProperty("serialname", QVariant::fromValue(combo.getcomboList()));
    ownContext->setContextProperty("serialbaudrate", QVariant::fromValue(baudratecombo.getcomboList()));
    ownContext->setContextProperty("combomanager", QVariant::fromValue(combomngr.getComboBoxIndices()));
    ownContext->setContextProperty("linecheckstatus", true);
    ownContext->setContextProperty("inputmessage", QVariant::fromValue(txtObjInput.gettxtInput()));

    QQmlContext *classContext = engine.rootContext();
    classContext->setContextProperty("comboModel", &combo);
    classContext->setContextProperty("baudratecombo", &baudratecombo);
    classContext->setContextProperty("combomngr", &combomngr);
    classContext->setContextProperty("txtUserInput", &txtObjInput);


    /**
     * @brief close the main window
     */
    QObject::connect(&app, &QGuiApplication::lastWindowClosed, [&]() {
        qDebug() << "The window was closed!";
        serialUiInstance.setrunning(false);
    });

    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
