#include "BookDB.hpp"

Book::Book(void) : m_id(-1), m_is_leased(false), m_is_reserved(false) { }

bool Book::lease(const QDate& date, const QString& user)
{
    if (m_is_reserved) {
        if (m_user == user) {
            m_leased = date;
            m_is_leased = true;
            m_is_reserved = false;
            return true;
        }
    }
    return false;
}

bool Book::reserve(const QDate& date, const QString& user)
{
    if (!m_is_reserved && !m_is_leased) {
        m_user = user;
        m_reserved = date;
        m_is_reserved = true;
        return true;
    }
    return false;
}


bool Book::release(const QDate&, const QString&)
{
    if (m_is_leased) {
        m_is_leased = false;
        m_user = QString();
        m_leased = QDate();
        m_reserved = QDate();
        return true;
    }
    return false;
}

bool Book::cancel_reservation(const QDate&, const QString& user)
{
    if (m_is_reserved) {
        m_is_reserved = false;
        m_user = QString();
        m_leased = QDate();
        m_reserved = QDate();
        return true;
    }
    return false;
}


void Book::load(const QJsonObject& json)
{
    m_id = json["id"].toInteger();
    m_user = json["user"].toString();
    m_title = json["title"].toString();
    m_author = json["author"].toString();
    m_publisher = json["publisher"].toString();
    m_year = json["date"].toInteger();


    auto str_leased = json["leased"].toString();
    if (str_leased.isEmpty() || m_user.isEmpty()) {
        m_is_leased = false;
        m_leased = QDate();
    } else {
        m_is_leased = true;
        m_leased = QDate::fromString(str_leased);
    }

    auto str_reserved = json["reserved"].toString();
    if (str_reserved.isEmpty() || m_user.isEmpty()) {
        m_leased = QDate();
        m_is_reserved = false;
    } else {
        m_is_reserved = true;
        m_reserved = QDate::fromString(str_reserved);
    }
}

void Book::save(QJsonObject& json) const
{
    json["id"] = m_id;
    json["user"] = m_user;
    json["title"] = m_title;
    json["author"] = m_author;
    json["publisher"] = m_publisher;
    json["date"] = m_year;

    if (m_is_leased)
        json["leased"] = m_leased.toString();
    else
        json["leased"] = "";

    if (m_is_reserved)
        json["reserved"] = m_reserved.toString();
    else
        json["reserved"] = "";
}

BookDB::BookDB()
{
}

qint32 BookDB::id(qint32 index) const
{
    return m_books[index].id();
}

qint32 BookDB::index(qint32 id) const
{
    for (auto it = m_books.begin(); it != m_books.end(); ++it) {
        if (it->id() == id) {
            return it - m_books.begin();
        }
    }
    return -1;
}

bool BookDB::exists(qint32 id) const
{
    return index(id) != -1;
}

bool BookDB::isLeased(qint32 id) const
{
    return m_books[index(id)].isLeased();
}

bool BookDB::isReserved(qint32 id) const
{
    return m_books[index(id)].isReserved();
}

const QString& BookDB::heldBy(qint32 id) const
{
    return m_books[index(id)].user();
}

bool BookDB::isHoldingAnyBooks(const QString& user) const
{
    for (auto it = m_books.begin(); it != m_books.end(); ++it) {
        if (it->user() == user) {
            return true;
        }
    }
    return false;
}

bool BookDB::lease(qint32 id, const QDate& date, const QString& user)
{
    if (m_books[index(id)].lease(date, user)) {
        emit leased(id, date, user);
        return true;
    }
    return false;
}

bool BookDB::reserve(qint32 id, const QDate& date, const QString& user)
{
    if (m_books[index(id)].reserve(date, user)) {
        emit reserved(id, date, user);
        return true;
    }
    return false;
}

bool BookDB::release(qint32 id, const QDate& date, const QString& user)
{
    if (m_books[index(id)].release(date, user)) {
        emit released(id, date, user);
        return true;
    }
    return false;
}

bool BookDB::cancel_reservation(qint32 id, const QDate& date, const QString& user)
{
    if (m_books[index(id)].cancel_reservation(date, user)) {
        emit reservation_cancelled(id, date, user);
        return true;
    }
    return false;
}

void BookDB::load(const QJsonArray& json)
{
    for (auto it : json)
    {
        Book temp;
        temp.load(it.toObject());
        m_books.append(temp);
    }
    emit loaded();
}

void BookDB::save(QJsonArray& json) const
{
    for (auto it = m_books.begin(); it != m_books.end(); ++it)
    {
        QJsonObject obj;
        it->save(obj);
        json.append(obj);
    }
}
