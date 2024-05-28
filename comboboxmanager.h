#ifndef COMBOBOXMANAGER_H
#define COMBOBOXMANAGER_H

#include <QObject>
#include <QMap>
#include <QDebug>

typedef QMap<QString, int> StringToIntMap;

class ComboBoxManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(StringToIntMap comboBoxIndices READ getComboBoxIndices NOTIFY comboBoxIndicesChanged)

public:
    explicit ComboBoxManager(QObject *parent = nullptr);

    QMap<QString, int> getComboBoxIndices() const { return m_comboBoxIndices; }
    Q_INVOKABLE void setComboBoxIndex(const QString &id, int index) {
        if (m_comboBoxIndices[id] != index) {
            qDebug() << "combobox id = " + id << ", index = " << index;
            m_comboBoxIndices[id] = index;
            emit comboBoxIndicesChanged();
        }
    }
signals:
    void comboBoxIndicesChanged();

private:
    QMap<QString, int> m_comboBoxIndices;
};

#endif // COMBOBOXMANAGER_H
