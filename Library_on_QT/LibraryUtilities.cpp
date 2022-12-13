#include "LibraryUtilities.hpp"

QJsonArray LoadJsonArray(const QString& filename)
{
    auto file = QFile(filename);
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    auto bytes = file.readAll();
    file.close();
    auto document = QJsonDocument::fromJson(bytes);
    return document.array();
}

void SaveJsonArray(const QString& filename, const QJsonArray& array)
{
    auto document = QJsonDocument(array);
    auto file = QFile(filename);
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    file.write(document.toJson());
    file.close();
}

void LoadDB(UserDB& db)
{
    auto json = LoadJsonArray("users.json");
    db.load(json);
}

void LoadDB(BookDB& db)
{
    auto json = LoadJsonArray("books.json");
    db.load(json);
}

void SaveDB(const UserDB& db)
{
    QJsonArray array;
    db.save(array);
    SaveJsonArray("users.json", array);
}

void SaveDB(const BookDB& db)
{
    QJsonArray array;
    db.save(array);
    SaveJsonArray("books.json", array);
}
