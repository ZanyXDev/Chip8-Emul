#include "registermodel.h"


RegisterModel::RegisterModel(const QList<quint8> &list, QObject *parent)
    : QAbstractListModel(parent)
    , m_list(list)
{

}

RegisterModel::RegisterModel( QObject *parent)
    : QAbstractListModel(parent)
{
    m_list.clear();

    for (int i=0; i< MAX_REG; i++)
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
    if ( nRole == Qt::DisplayRole )
    {
        return QString("0x%1").arg( m_list.at( index.row() ),0,16);
    }
    if ( nRole == Qt::EditRole )
    {
        return m_list.at( index.row() );
    }
    return QVariant();
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

    if ( orientation == Qt::Horizontal )
    {
        return QString( tr("REG") );
    }
    else
    {
        QString h_data;
        switch (nSection) {
        case 0:
            h_data = QString("V0");
            break;
        case 1:
            h_data = QString("V1");
            break;
        case 2:
            h_data = QString("V2");
            break;
        case 3:
            h_data = QString("V3");
            break;
        case 4:
            h_data = QString("V4");
            break;
        case 5:
            h_data = QString("V5");
            break;
        case 6:
            h_data = QString("V6");
            break;
        case 7:
            h_data = QString("V7");
            break;
        case 8:
            h_data = QString("V8");
            break;
        case 9:
            h_data = QString("V9");
            break;
        case 10:
            h_data = QString("VA");
            break;
        case 11:
            h_data = QString("VB");
            break;
        case 12:
            h_data = QString("VC");
            break;
        case 13:
            h_data = QString("VD");
            break;
        case 14:
            h_data = QString("VE");
            break;
        case 15:
            h_data = QString("VF");
            break;
        default:
            break;
        }
        return h_data;
    }

    return QVariant();
}

Qt::ItemFlags RegisterModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags flags = QAbstractListModel::flags( index );
    return index.isValid()
            ? ( flags | Qt::ItemIsSelectable )
            : flags;
}

