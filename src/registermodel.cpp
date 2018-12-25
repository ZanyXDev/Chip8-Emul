#include "registermodel.h"
#include "registermodel.h"

RegisterModel::RegisterModel(const QList<quint8> &list, QObject *parent)
    : QAbstractListModel(parent)
    , m_list(list)
{

}

RegisterModel::RegisterModel(quint8 m_size, QObject *parent)
    : QAbstractListModel(parent)
{
    m_list.clear();

    for (int i=0; i< m_size;i++)
    {
        m_list.append( 0 );
    }
}

QVariant RegisterModel::data(const QModelIndex &index, int nRole) const
{
    if ( !index.isValid() )
    {
        return QVariant();
    }
    return (nRole == Qt::DisplayRole || nRole ==Qt::EditRole)
            ? m_list.at( index.row() )
            : QVariant();
}

bool RegisterModel::setData(const QModelIndex &index, const QVariant &value, int nRole)
{
    if (index.isValid() && nRole == Qt::EditRole )
    {
        m_list.replace( index.row(), value.value<quint8>() );
        emit dataChanged( index, index);
        return true;
    }
    return false;
}

int RegisterModel::rowCount(const QModelIndex &parent) const
{
    if ( parent.isValid() )
    {

    }
    return m_list.size();
}

QVariant RegisterModel::headerData(int nSection, Qt::Orientation orientation, int nRole) const
{
    if ( nRole != Qt::DisplayRole)
    {
        return QVariant();
    }
    return ( orientation == Qt::Horizontal )
            ? QString( tr("Number") )
            : QString::number( nSection );
}

Qt::ItemFlags RegisterModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags flags = QAbstractListModel::flags( index );
    return index.isValid()
            ? ( flags | Qt::ItemIsEditable )
            : flags;
}

