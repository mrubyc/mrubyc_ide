#include "mrcprojecttreemodel.h"
#include "mrcprojecttreeitem.h"
#include "mrcproject.h"
#include "mrcfile.h"

#include <QModelIndex>
#include <QVariant>
#include <QList>
#include <QVector>
#include <QIcon>
#include <QApplication>
#include <QStyle>
#include <QDebug>

MrcProjectTreeModel::MrcProjectTreeModel(MrcProject* mrcProject, QObject *parent)
    :QAbstractItemModel(parent)
    ,m_mrcProject(mrcProject)
{
    m_rootItem = new MrcProjectTreeItem(Q_NULLPTR);
    setupModelData(m_rootItem, mrcProject);
}

MrcProjectTreeModel::~MrcProjectTreeModel()
{
    delete m_rootItem;
}

QVariant MrcProjectTreeModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role != Qt::DisplayRole && role != Qt::EditRole && role != Qt::ToolTipRole
            && role != Qt::DecorationRole)
        return QVariant();

    MrcProjectTreeItem *item = getItem(index);
    if (item) {
        if (role == Qt::DisplayRole || role == Qt::EditRole) {
            QString fileName = item->data()->name();
            return QVariant(fileName);
        } else if (role == Qt::ToolTipRole) {
            QString filePath = item->data()->path();
            return QVariant(filePath);
        } else if (role == Qt::DecorationRole) {
            MrcAbstractFile *abstractFile = item->data();
            if (abstractFile->isFolder()) {
                QIcon icon = QApplication::style()->standardIcon(QStyle::SP_DirHomeIcon);
                return QVariant(icon);
            } else if (abstractFile->isNormalFile()) {
                QIcon icon = QApplication::style()->standardIcon(QStyle::SP_FileIcon);
                return QVariant(icon);
            }
        }
    }
    return QVariant();
}

QModelIndex MrcProjectTreeModel::index(int row, int column, const QModelIndex &parent) const
{
    if (parent.isValid() && parent.column() != 0)
        return QModelIndex();

    MrcProjectTreeItem *parentItem = getItem(parent);
    MrcProjectTreeItem *childItem = parentItem->child(row);
    if (childItem)
        return createIndex(row, column, childItem);
    else
        return QModelIndex();
}

QModelIndex MrcProjectTreeModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    MrcProjectTreeItem *childItem = getItem(index);
    MrcProjectTreeItem *parentItem = childItem->parent();

    if (parentItem == m_rootItem)
        return QModelIndex();

    return createIndex(parentItem->childNumber(), 0, parentItem);
}

int MrcProjectTreeModel::rowCount(const QModelIndex &parent) const
{
    MrcProjectTreeItem *parentItem = getItem(parent);

    return parentItem->childCount();
}

int MrcProjectTreeModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return 1;
}

Qt::ItemFlags MrcProjectTreeModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
}

bool MrcProjectTreeModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (role != Qt::EditRole)
        return false;

    qDebug() << "Name:" << value.typeName();

    MrcProjectTreeItem *item = getItem(index);

    if (value.metaType() == QMetaType::fromType<QString>()) {
        QString newFileName = value.toString();
        emit textChanged(index, newFileName);
        emit dataChanged(index, index);
    } else {
        // Type: QVariant::QWidget* Name: MrcFile*
        MrcAbstractFile *mrcFile = value.value<MrcAbstractFile*>();
        bool result = item->setData(mrcFile);
        if (result)
            emit dataChanged(index, index);
    }
    return true;
}

bool MrcProjectTreeModel::insertRows(int position, int rows, const QModelIndex &parent)
{
    MrcProjectTreeItem *parentItem = getItem(parent);
    bool success;

    beginInsertRows(parent, position, position + rows - 1);
    success = parentItem->insertChildren(position, rows);
    endInsertRows();

    return success;
}

bool MrcProjectTreeModel::removeRows(int position, int rows, const QModelIndex &parent)
{
    MrcProjectTreeItem *parentItem = getItem(parent);
    bool success = true;

    beginRemoveRows(parent, position, position + rows - 1);
    success = parentItem->removeChildren(position, rows);
    endRemoveRows();

    return success;
}

void MrcProjectTreeModel::setupModelData(MrcProjectTreeItem *parent, MrcProject *mrcProject)
{
    // set up project.
    MrcAbstractFile *mrcProjectFile = static_cast<MrcAbstractFile*>(mrcProject);
    parent->insertChildren(0, 1);
    MrcProjectTreeItem* subParent = parent->child(0);
    subParent->setData(mrcProjectFile);

    // set up files in a project
    QList<MrcFile*> fileList = mrcProject->projectMrcFileList();
    subParent->insertChildren(0, fileList.size());
    for (int i=0; i<fileList.size(); i++) {
        MrcFile* mrcFile = fileList.at(i);
        MrcAbstractFile *mrcAbstractFile = static_cast<MrcAbstractFile*>(mrcFile);
        MrcProjectTreeItem *treeItem = subParent->child(i);
        treeItem->setData(mrcAbstractFile);
    }
}

MrcProjectTreeItem *MrcProjectTreeModel::getItem(const QModelIndex &index) const
{
    if (index.isValid()) {
        MrcProjectTreeItem *item = static_cast<MrcProjectTreeItem*>(index.internalPointer());
        if (item)
            return item;
    }
    return m_rootItem;
}
