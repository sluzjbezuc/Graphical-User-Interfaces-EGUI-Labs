#ifndef LOGINWINDOW_HPP
#define LOGINWINDOW_HPP

#include <QDialog>
#include <QLineEdit>
#include <QJsonObject>

class LoginWindow : public QDialog
{
public:
    LoginWindow(QWidget* parent = nullptr);
    const QJsonObject toJson(void) const;

    int exec_create(const QString& hint);
    int exec_signin(const QString& hint);

private:
    QLineEdit* m_user;
    QLineEdit* m_pass;
};

#endif // LOGINWINDOW_HPP
