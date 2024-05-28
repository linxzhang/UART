#ifndef COMBOBOXPORT_H
#define COMBOBOXPORT_H

#include <QObject>

class ComboBoxPort : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QStringList comboList READ getcomboList WRITE setComboList NOTIFY comboListChanged)
    Q_PROPERTY(int count READ count WRITE setCount NOTIFY countChanged)
    Q_PROPERTY(int currentIndex READ currentIndex WRITE setcurrentIndex NOTIFY currentIndexChanged)

public:
    explicit ComboBoxPort(QObject *parent = nullptr);

    ComboBoxPort(const QStringList &list,int count, QObject *parent = 0);

    const QStringList getcomboList();
    void setComboList(const QStringList &comboList);

    int count();
    void setCount(int cnt);

    int currentIndex();
    void setcurrentIndex(int index);

signals:
    void comboListChanged();
    void countChanged();
    void currentIndexChanged();

public slots:
    void append(QString item);
    void onCurrentIndexChanged(int index) {
        if (m_currentIndex != index) {
            m_currentIndex = index;
            emit currentIndexChanged();
            }
        };
private:

    QStringList m_comboList;
    int         m_count;
    int         m_currentIndex;
};

#endif // COMBOBOXPORT_H
