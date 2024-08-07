#include "savechangesmodel.h"
#include "mrcfile.h"

#include <QDebug>

SaveChangesModel::SaveChangesModel(const QList<MrcFile*> &mrcFileList, QObject *parent)
    :QAbstractItemModel(parent)
    ,m_mrcFileList(mrcFileList)
{

}


SaveChangesModel::~SaveChangesModel()
{
}

QVariant SaveChangesModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role != Qt::DisplayRole && role != Qt::ToolTipRole)
        return QVariant();

    QString displayStr;
    MrcFile* mrcFile = static_cast<MrcFile*>(index.internalPointer());
    if (role == Qt::DisplayRole) {
        displayStr = mrcFile->name();
    } else if (role == Qt::ToolTipRole) {
        displayStr = mrcFile->path();
    }
    return QVariant(displayStr);
}

QModelIndex SaveChangesModel::index(int row, int column, const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    int count = m_mrcFileList.size();
    if (row < count) {
        return createIndex(row, column, m_mrcFileList.at(row));
    }
    return QModelIndex();
}

QModelIndex SaveChangesModel::parent(const QModelIndex &index) const
{
    Q_UNUSED(index);
    return QModelIndex();
}

int SaveChangesModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_mrcFileList.size();
}

int SaveChangesModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 1;
}

//Qt::ItemFlags SaveChangesModel::flags(const QModelIndex &index) const
//{
//    return QAbstractItemModel::flags(index);// | Qt::ToolTipRole | Qt::DisplayRole;
//}
