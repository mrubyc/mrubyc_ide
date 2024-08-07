#include "mrcprojecttreeitem.h"
#include "mrcfile.h"

MrcProjectTreeItem::MrcProjectTreeItem(MrcAbstractFile *mrcFile, MrcProjectTreeItem *parent)
    :QObject(parent)
{
    m_parentItem = parent;
    m_mrcFile = mrcFile;
}

MrcProjectTreeItem::~MrcProjectTreeItem()
{
    qDeleteAll(m_childItems);
}

MrcProjectTreeItem *MrcProjectTreeItem::parent()
{
    return m_parentItem;
}

int MrcProjectTreeItem::childNumber() const
{
    if (m_parentItem)
        return m_parentItem->m_childItems.indexOf(const_cast<MrcProjectTreeItem*>(this));

    return 0;
}

MrcAbstractFile *MrcProjectTreeItem::data() const
{
    return m_mrcFile;
}

bool MrcProjectTreeItem::setData(MrcAbstractFile *value)
{
    m_mrcFile = value;
    return true;
}

MrcProjectTreeItem *MrcProjectTreeItem::child(int number)
{
    return m_childItems.value(number);
}

int MrcProjectTreeItem::childCount() const
{
    return m_childItems.count();
}

bool MrcProjectTreeItem::insertChildren(int position, int count)
{
    if (position < 0 || position > m_childItems.size())
        return false;

    for (int row = 0; row < count; ++row) {
        MrcProjectTreeItem *item = new MrcProjectTreeItem(Q_NULLPTR, this);
        m_childItems.insert(position, item);
    }

    return true;
}

bool MrcProjectTreeItem::removeChildren(int position, int count)
{
    if (position < 0 || position + count > m_childItems.size())
        return false;

    for (int row = 0; row < count; ++row)
        delete m_childItems.takeAt(position);

    return true;

}

