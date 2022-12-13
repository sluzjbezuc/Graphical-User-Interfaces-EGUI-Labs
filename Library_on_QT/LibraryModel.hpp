#ifndef LIBRARYMODEL_HPP
#define LIBRARYMODEL_HPP

#include <QAbstractTableModel>
#include <QObject>

#include "BookDB.hpp"

class LibraryModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    LibraryModel(QObject *parent = nullptr);

    BookDB& database(void) { return m_db; }
    const BookDB& database(void) const { return m_db; }

    int rowCount(const QModelIndex& parent = QModelIndex()) const;
    int columnCount(const QModelIndex& parent = QModelIndex()) const;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;
    QVariant headerData(int section, Qt::Orientation orientation = Qt::Orientation::Horizontal, int role = Qt::DisplayRole) const;

protected:
    void refresh(void);
    void refresh(qint32 id);

private slots:
    void loaded(void);
    void leased(qint32 id, const QDate& date, const QString& user);
    void reserved(qint32 id, const QDate& date, const QString& user);
    void released(qint32 id, const QDate& date, const QString& user);
    void reservation_cancelled(qint32 id, const QDate& date, const QString& user);

private:
    BookDB m_db;
};

#endif // LIBRARYMODEL_HPP
