#include "LibraryModel.hpp"

LibraryModel::LibraryModel(QObject *parent)
    : QAbstractTableModel(parent)
{
    LibraryModel::connect(
        &m_db, &BookDB::leased,
        this, &LibraryModel::leased
    );
    LibraryModel::connect(
        &m_db, &BookDB::reserved,
        this, &LibraryModel::reserved
    );
    LibraryModel::connect(
        &m_db, &BookDB::released,
        this, &LibraryModel::leased
    );
    LibraryModel::connect(
        &m_db, &BookDB::reservation_cancelled,
        this, &LibraryModel::reservation_cancelled
    );
    LibraryModel::connect(
        &m_db, &BookDB::loaded,
        this, &LibraryModel::loaded
    );
}

int LibraryModel::rowCount(const QModelIndex&) const
{
    return m_db.count();
}

int LibraryModel::columnCount(const QModelIndex&) const
{
    return 6;
}

QVariant LibraryModel::data(const QModelIndex& index, int role) const
{
    if (role == Qt::TextAlignmentRole)
        return Qt::AlignCenter;
    if (role != Qt::DisplayRole)
        return QVariant();
    auto id = m_db.id(index.row());
    auto book = m_db.book(id);
    switch(index.column())
    {
    case 0:
        return id;
    case 1:
        return book.title();
    case 2:
        return book.author();
    case 3:
        return book.publisher();
    case 4:
        return book.year();
    case 5:
        return !book.isLeased() && !book.isReserved();
    }
    return QVariant();
}

QVariant LibraryModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::TextAlignmentRole)
    {
        return Qt::AlignCenter;
    }
    if (orientation == Qt::Orientation::Horizontal && role == Qt::DisplayRole)
    {
        switch (section)
        {
        case 0:
            return "ID";
        case 1:
            return "Title";
        case 2:
            return "Author";
        case 3:
            return "Publisher";
        case 4:
            return "Year";
        case 5:
            return "Is Available (?)";
        }
    }
    return QAbstractTableModel::headerData(section, orientation, role);
}

void LibraryModel::refresh(void) {
    emit dataChanged(index(0, 0), index(rowCount() - 1, columnCount() - 1));
}

void LibraryModel::refresh(qint32 id) {

    auto row = m_db.index(id);
    emit dataChanged(index(row, 0), index(row, columnCount() - 1));
}

void LibraryModel::loaded(void) { refresh(); }
void LibraryModel::leased(qint32 id, const QDate&, const QString&) { refresh(id); }
void LibraryModel::reserved(qint32 id, const QDate&, const QString&) { refresh(id); }
void LibraryModel::released(qint32 id, const QDate&, const QString&) { refresh(id); }
void LibraryModel::reservation_cancelled(qint32 id, const QDate&, const QString&) { refresh(id); }
