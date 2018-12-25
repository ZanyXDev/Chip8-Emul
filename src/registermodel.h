#ifndef REGISTERMODEL_H
#define REGISTERMODEL_H

#include <QObject>
#include <QAbstractListModel>
#include <QList>
#include <QVariant>

class RegisterModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit RegisterModel(const QList<quint8> &list,QObject *parent = nullptr);

    QVariant data (const QModelIndex& index, int nRole) const;

    bool setData(const QModelIndex& index,
                 const QVariant &value,
                 int nRole);

    int rowCount(const QModelIndex & parent = QModelIndex() ) const;

    QVariant headerData(int nSection,
                        Qt::Orientation orientation,
                        int nRole = Qt::DisplayRole) const;

    Qt::ItemFlags flags (const QModelIndex &index) const;

signals:

public slots:

private:
    QList<quint8> m_list;
};

#endif // REGISTERMODEL_H
