#ifndef SAVECHANGESMODEL_H
#define SAVECHANGESMODEL_H

#include <QList>
#include <QAbstractItemModel>

class MrcFile;

/*!
 * \brief The SaveChangesModel class.
 *        It is a model to dispaly unsaved files.
 */
class SaveChangesModel : public QAbstractItemModel
{
public:
    explicit SaveChangesModel(const QList<MrcFile*> &mrcFileList, QObject *parent = Q_NULLPTR);
    ~SaveChangesModel();

    QVariant data(const QModelIndex &index, int role) const override;
    QModelIndex index(int row, int column,
                      const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &index) const override;

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

//    Qt::ItemFlags flags(const QModelIndex &index) const override;
private:
    //! unsave MRC file List.
    QList<MrcFile*> m_mrcFileList;
};

#endif // SAVECHANGESMODEL_H
