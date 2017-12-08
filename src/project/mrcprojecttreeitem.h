#ifndef MRCPROJECTTREEITEM_H
#define MRCPROJECTTREEITEM_H

#include <QObject>
#include <QVector>
#include <QList>

class MrcAbstractFile;

/*!
 * \brief The MrcProjectTreeItem class.
 *        This is an element data model for MRC Project Tree View.
 */
class MrcProjectTreeItem : public QObject
{
    Q_OBJECT
public:
    explicit  MrcProjectTreeItem(MrcAbstractFile *mrcFile, MrcProjectTreeItem *parent = Q_NULLPTR);
    ~MrcProjectTreeItem();

    /*!
     * \brief Get the own parent.
     * \return parent's element.
     */
    MrcProjectTreeItem *parent();

    /*!
     * \brief Get brother's count.
     * \return brother's count.
     */
    int childNumber() const;

    /*!
     * \brief Get the data.
     *          It is own data.
     * \return MrcFile object.
     */
    MrcAbstractFile *data() const;

    /*!
     * \brief Set the data.
     * \param value MrcFile object.
     * \return always true.
     */
    bool setData(MrcAbstractFile *value);

    /*!
     * \brief Get the child's element of number.
     * \param number
     * \return child's element
     */
    MrcProjectTreeItem *child(int number);

    /*!
     * \brief Get the child's count in the element.
     * \return child's count
     */
    int childCount() const;

    /*!
     * \brief insertChildren
     * \param position
     * \param dataList
     * \return
     */
    bool insertChildren(int position, int count);

    /*!
     * \brief Remove the children.
     *        It deletes the child's element from position to positon + count.
     * \param position
     * \param count
     * \return
     */
    bool removeChildren(int position, int count);

private:
    //! own data
    MrcAbstractFile *m_mrcFile;
    //! parent element
    MrcProjectTreeItem *m_parentItem;

    //! children element
    QList<MrcProjectTreeItem*> m_childItems;
};

#endif // MRCPROJECTTREEITEM_H
