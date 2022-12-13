#ifndef LIBRARYUTILITIES_H
#define LIBRARYUTILITIES_H

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QTextStream>
#include <QFile>

#include "UserDB.hpp"
#include "BookDB.hpp"

QJsonArray LoadJsonArray(const QString& filename);
void SaveJsonArray(const QString& filename, const QJsonArray& array);

void LoadDB(UserDB& db);
void LoadDB(BookDB& db);
void SaveDB(const UserDB& db);
void SaveDB(const BookDB& db);

#endif // LIBRARYUTILITIES_H
