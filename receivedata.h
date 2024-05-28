#ifndef RECEIVEDATA_H
#define RECEIVEDATA_H

#include <QObject>

class ReceiveData : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString textFieldValue READ getTextFieldValue WRITE setTextFieldValue NOTIFY textFieldValueChanged)

public:
    explicit ReceiveData(QObject *parent = nullptr);

public:
    QString getTextFieldValue() const { return m_textFieldValue; }
    void setTextFieldValue(const QString &value) {
        if (value != m_textFieldValue) {
            m_textFieldValue = value;
            emit textFieldValueChanged();
        }
    }

signals:
    void textFieldValueChanged();
    void sendMessageToQml(const QString &message);

private:
    QString m_textFieldValue;

};

#endif // RECEIVEDATA_H
