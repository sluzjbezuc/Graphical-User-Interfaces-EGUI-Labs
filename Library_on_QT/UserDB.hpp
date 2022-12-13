#ifndef USERDB_HPP
#define USERDB_HPP

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QMap>

class User {
public:
    User(void) = default;
    User(const QString& user_t, const QString& pass_t = "");

    bool isLibrarian(void) const { return m_name == "librarian"; }
    const QString& name(void) const { return m_name; }
    const QString& pass(void) const { return m_pass; }
    QString& pass(void) { return m_pass; }

    void load(const QJsonObject& json);
    void save(QJsonObject& json) const;
private:
    QString m_name;
    QString m_pass;
};

class UserDB
{
public:
    UserDB(void);

    bool isLibrarian(const QString& user) const;

    void clear(void);
    bool remove(const QString& user);
    bool append(const QString& user);
    bool exists(const QString& user) const;
    QString& password(const QString& user);
    QString& password(const QString& user) const;

    void load(const QJsonArray& json);
    void save(QJsonArray& json) const;

private:
    QMap<QString, User> m_users;
};

#endif // USERDB_HPP
