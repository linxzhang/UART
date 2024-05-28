#ifndef CSENDMESSAGE_H
#define CSENDMESSAGE_H

#include <QObject>

class CSendMessage : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString textFieldValue READ getTextFieldValue WRITE setTextFieldValue NOTIFY textFieldValueChanged)

public:
    explicit CSendMessage(QObject *parent = nullptr);

    QString getTextFieldValue() const { return m_textFieldValue; }
    void setTextFieldValue(const QString &value) {
        if (value != m_textFieldValue) {
            m_textFieldValue = value;
            emit textFieldValueChanged();
        }
    }

signals:
    void sendMessageToQml(const QString &message);
    void textFieldValueChanged();

private:
    QString m_textFieldValue;
};

#endif // CSENDMESSAGE_H
