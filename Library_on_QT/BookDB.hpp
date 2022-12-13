#ifndef BOOKDB_HPP
#define BOOKDB_HPP

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QObject>
#include <QList>

class Book {
public:
    Book(void);

    qint32 id(void) const { return m_id; }
    const QString& user(void) const { return m_user; }
    const QString& title(void) const { return m_title; }
    const QString& author(void) const { return m_author; }
    const QString& publisher(void) const { return m_publisher; }

    const qint64& year(void) const { return m_year; }
    const QDate& leased(void) const { return m_leased; }
    const QDate& reserved(void) const { return m_reserved; }
    bool isValid(void) const { return m_id != -1; }
    bool isLeased(void) const { return m_is_leased; }
    bool isReserved(void) const { return m_is_reserved; }

    bool lease(const QDate& date, const QString& user);
    bool reserve(const QDate& date, const QString& user);
    bool release(const QDate& date, const QString& user);
    bool cancel_reservation(const QDate& date, const QString& user);

    void load(const QJsonObject& json);
    void save(QJsonObject& json) const;
private:
    qint32 m_id;
    QString m_user;
    QString m_title;
    QString m_author;
    QString m_publisher;
    qint64 m_year;
    QDate m_leased;
    QDate m_reserved;
    bool m_is_leased;
    bool m_is_reserved;
};

class BookDB : public QObject
{
    Q_OBJECT

public:
    BookDB(void);

    qint32 id(qint32 index) const;
    qint32 index(qint32 id) const;

    const Book book(qint32 id) const { return m_books[index(id)]; }
    const QList<Book>& books(void) const { return m_books; }
    QList<Book>::size_type count(void) const { return m_books.count(); }

    bool exists(qint32 id) const;
    bool isLeased(qint32 id) const;
    bool isReserved(qint32 id) const;
    const QString& heldBy(qint32 id) const;

    bool isHoldingAnyBooks(const QString& user) const;

    bool lease(qint32 id, const QDate& date, const QString& user);
    bool reserve(qint32 id, const QDate& date, const QString& user);
    bool release(qint32 id, const QDate& date, const QString& user);
    bool cancel_reservation(qint32 id, const QDate& date, const QString& user);

    void load(const QJsonArray& json);
    void save(QJsonArray& json) const;

signals:
    void loaded(void);
    void leased(qint32 id, const QDate& date, const QString& user);
    void reserved(qint32 id, const QDate& date, const QString& user);
    void released(qint32 id, const QDate& date, const QString& user);
    void reservation_cancelled(qint32 id, const QDate& date, const QString& user);

private:
    QList<Book> m_books;
};

#endif // BOOKDB_HPP
