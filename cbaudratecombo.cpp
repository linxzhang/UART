#include "cbaudratecombo.h"
#include "qdebug.h"

CBaudrateCombo::CBaudrateCombo(QObject *parent) : QObject(parent)
{

}

CBaudrateCombo::CBaudrateCombo(const QStringList &list,int count, QObject *parent):
    QObject(parent), m_comboList(list), m_count(count)
{

}

const QStringList CBaudrateCombo::getcomboList()
{
    return m_comboList;
}

void CBaudrateCombo::setComboList(const QStringList &comboList)
{

    if (m_comboList != comboList)
    {
        m_comboList = comboList;
        emit comboListChanged();
    }

}

int CBaudrateCombo::count()
{
    return m_count;
}

void CBaudrateCombo::setCount(int cnt)
{
    if (cnt != m_count)
    {
        m_count = cnt;
        emit countChanged();
    }
}

int CBaudrateCombo::currentIndex()
{
    return m_currentIndex;
}

void CBaudrateCombo::setcurrentIndex(int index)
{
    if (index != m_currentIndex)
    {
        m_currentIndex = index;
        qDebug()<<"current index is"<<index;
        emit currentIndexChanged();
    }
}

void CBaudrateCombo::append(QString item)
{
    qDebug()<<"current text is "<<item;
    m_comboList.append(item);
    emit comboListChanged();
}
