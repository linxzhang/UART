#include "comboboxport.h"
#include "qdebug.h"

ComboBoxPort::ComboBoxPort(QObject *parent) : QObject(parent)
{

}

ComboBoxPort::ComboBoxPort(const QStringList &list, int count, QObject *parent) :
    QObject(parent), m_comboList(list), m_count(count)
{

}
const QStringList ComboBoxPort::getcomboList()
{
    return m_comboList;
}

void ComboBoxPort::setComboList(const QStringList &comboList)
{

    if (m_comboList != comboList)
    {
        m_comboList = comboList;
        emit comboListChanged();
    }

}

int ComboBoxPort::count()
{
    return m_count;
}

void ComboBoxPort::setCount(int cnt)
{
    if (cnt != m_count)
    {
        m_count = cnt;
        emit countChanged();
    }
}

int ComboBoxPort::currentIndex()
{
    return m_currentIndex;
}

void ComboBoxPort::setcurrentIndex(int index)
{
    if (index != m_currentIndex)
    {
        m_currentIndex = index;
        qDebug()<<"current index is"<<index;
        emit currentIndexChanged();
    }
}

void ComboBoxPort::append(QString item)
{
    qDebug()<<"current text is "<<item;
    m_comboList.append(item);
    emit comboListChanged();
}

