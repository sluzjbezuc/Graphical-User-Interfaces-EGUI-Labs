#ifndef AUTHWINDOW_HPP
#define AUTHWINDOW_HPP

#include "UserDB.hpp"
#include <QDialog>
#include <QLineEdit>
#include <QJsonObject>

class AuthWindow : public QDialog
{
public:
    AuthWindow(QWidget* parent = nullptr);

    bool isLibrarian(void) const;
    const QString& activeUser(void) const { return m_active_user; }

    void authorize(void);
    void deleteActiveUser(void);

private slots:
    void registration(void);
    void authorization(void);

private:
    QString m_active_user;
    QLineEdit* m_user;
    QLineEdit* m_pass;
    UserDB m_db;
};

#endif // AUTHWINDOW_HPP
