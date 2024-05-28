#ifndef USERINPUTMESSAGE_H
#define USERINPUTMESSAGE_H

#include <QObject>

class userInputMessage : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString txtInput READ gettxtInput WRITE settxtInput NOTIFY textInputChanged)

public:
    explicit userInputMessage(QObject *parent = nullptr);
    QString gettxtInput() const { return m_txtInput;};
    void settxtInput(const QString &text) {
        if (m_txtInput != text) {
            m_txtInput = text;
            emit textInputChanged();
        }

    };

signals:
    void textInputChanged();

private:
    QString m_txtInput;

};

#endif // USERINPUTMESSAGE_H
