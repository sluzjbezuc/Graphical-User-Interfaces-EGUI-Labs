#include "UserDB.hpp"

User::User(const QString& user_t, const QString& pass_t)
    : m_name(user_t), m_pass(pass_t) { }

void User::load(const QJsonObject& json)
{
    m_name = json["user"].toString();
    m_pass = json["pwd"].toString();
}

void User::save(QJsonObject& json) const
{
    json["user"] = m_name;
    json["pwd"] = m_pass;
}


UserDB::UserDB()
{
}

bool UserDB::isLibrarian(const QString& user) const
{
    if (exists(user))
        return m_users[user].isLibrarian();
    return false;
}

void UserDB::clear(void)
{
    m_users.clear();
}

bool UserDB::remove(const QString& user)
{
    return m_users.remove(user);
}

bool UserDB::append(const QString& user)
{
    if (!exists(user))
    {
        m_users.insert(user, User(user, ""));
        return true;
    }
    return false;
}

bool UserDB::exists(const QString& user) const
{
    return m_users.contains(user);
}

QString& UserDB::password(const QString& user)
{
    return m_users[user].pass();
}

QString& UserDB::password(const QString& user) const
{
    return m_users[user].pass();
}


void UserDB::load(const QJsonArray& json)
{
    for (auto it : json)
    {
        User temp;
        temp.load(it.toObject());
        m_users.insert(temp.name(), temp);
    }
}

void UserDB::save(QJsonArray& json) const
{
    for (auto it : m_users)
    {
        QJsonObject obj;
        it.save(obj);
        json.append(obj);
    }
}
