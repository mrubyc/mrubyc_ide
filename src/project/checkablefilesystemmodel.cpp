#include "checkablefilesystemmodel.h"

#include <QFileSystemModel>
#include <QSet>
#include <QDebug>

CheckableFileSystemModel::CheckableFileSystemModel(QObject *parent)
    :QFileSystemModel(parent)
{
//    connect(this, &CheckableFileSystemModel::dataChanged, this, &CheckableFileSystemModel::changeCheck);
}

CheckableFileSystemModel::~CheckableFileSystemModel()
{

}

Qt::ItemFlags CheckableFileSystemModel::flags(const QModelIndex &index) const
{
    return QFileSystemModel::flags(index) | Qt::ItemIsUserCheckable;
}

QVariant CheckableFileSystemModel::data(const QModelIndex &index, int role) const
{
    if (index.isValid() && index.column() == 0 && role == Qt::CheckStateRole) {
        return m_checkedIndexes.contains(index) ? Qt::Checked : Qt::Unchecked;
    }

    return QFileSystemModel::data(index, role);
}

bool CheckableFileSystemModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (index.isValid() && index.column() == 0 && role == Qt::CheckStateRole) {
        if (value == Qt::Checked) {
            m_checkedIndexes.insert(index);
            if (hasChildren(index))
                recursiveCheck(index, value);
        } else {
            m_checkedIndexes.remove(index);
            if (hasChildren(index))
                recursiveCheck(index, value);
        }
        emit dataChanged(index, index);
        return true;
    }
    return QFileSystemModel::setData(index, value, role);
}

bool CheckableFileSystemModel::recursiveCheck(const QModelIndex &index, const QVariant &value)
{
    bool check = false;
    if (hasChildren(index)) {
        int childrenCount = rowCount(index);
        QModelIndex child;
        for (int i=0; i<childrenCount; i++) {
            child = QFileSystemModel::index(i, 0, index);
            bool status = setData(child, value, Qt::CheckStateRole);
            if (status && (check != status)) {
                check = status;
            }
        }
    }
    return check;
}

QSet<QModelIndex> CheckableFileSystemModel::checkedIndexes()
{
    return m_checkedIndexes;
}
