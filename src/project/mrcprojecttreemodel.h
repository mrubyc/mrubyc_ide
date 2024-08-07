#ifndef MRCPROJECTTREEMODEL_H
#define MRCPROJECTTREEMODEL_H

#include <QObject>
#include <QAbstractItemModel>

QT_BEGIN_NAMESPACE
class QVariant;
class QModelIndex;
QT_END_NAMESPACE

class MrcProject;
class MrcProjectTreeItem;

/*!
 * \brief The MrcProjectTreeModel class.
 * It is a data model for a tree view of mrc project.
 */
class MrcProjectTreeModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit MrcProjectTreeModel(MrcProject* mrcProject, QObject *parent = Q_NULLPTR);
    ~MrcProjectTreeModel();

    QVariant data(const QModelIndex &index, int role) const override;

    QModelIndex index(int row, int column,
                      const QModelIndex &parent = QModelIndex()) const override;

    QModelIndex parent(const QModelIndex &index) const override;

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    Qt::ItemFlags flags(const QModelIndex &index) const override;


    bool setData(const QModelIndex &index, const QVariant &value,
                 int role = Qt::EditRole) override;

    bool insertRows(int position, int rows,
                    const QModelIndex &parent = QModelIndex()) override;

    bool removeRows(int position, int rows,
                    const QModelIndex &parent = QModelIndex()) override;

Q_SIGNALS:
    /*!
     * \brief This signal is send when the user changes a text in the view.
     * \param index It is an index of the view.
     * \param changedText change after a text.
     */
    void textChanged(QModelIndex index, const QString &changedText);
private:
    void setupModelData(MrcProjectTreeItem *parent, MrcProject *mrcProject);
    MrcProjectTreeItem *getItem(const QModelIndex &index) const;

    MrcProjectTreeItem *m_rootItem;
    MrcProject* m_mrcProject;
};

#endif // MRCPROJECTTREEMODEL_H
