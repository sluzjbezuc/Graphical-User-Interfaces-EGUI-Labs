#include "LoginWindow.hpp"

#include <QDialogButtonBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QPushButton>
#include <QLineEdit>

LoginWindow::LoginWindow(QWidget* parent) : QDialog(parent)
{
    auto root_layout = new QVBoxLayout(this);
    auto form_layout = new QFormLayout(this);
    auto hbox_layout = new QHBoxLayout(this);
    auto accept_button = new QPushButton("Accept", this);
    auto cancel_button = new QPushButton("Cancel", this);
    m_user = new QLineEdit(this);
    m_pass = new QLineEdit(this);
    root_layout->addLayout(form_layout);
    root_layout->addLayout(hbox_layout);
    hbox_layout->addWidget(accept_button);
    hbox_layout->addWidget(cancel_button);
    form_layout->addRow("Username", m_user);
    form_layout->addRow("Password", m_pass);
    m_pass->setEchoMode(QLineEdit::Password);
    this->setLayout(root_layout);
    this->setWindowTitle("Auth");
    QObject::connect(
        accept_button, &QPushButton::clicked,
        this, &LoginWindow::accept
    );
    QObject::connect(
        cancel_button, &QPushButton::clicked,
        this, &LoginWindow::reject
    );
}

const QJsonObject LoginWindow::toJson(void) const
{
    QJsonObject obj;
    obj.insert("user", m_user->text());
    obj.insert("pwd", m_pass->text());
    return obj;
}

int LoginWindow::exec_create(const QString& hint)
{
    m_user->setPlaceholderText(hint);
    this->setWindowTitle("Register");
    return this->exec();
}

int LoginWindow::exec_signin(const QString& hint)
{
    m_user->setPlaceholderText(hint);
    this->setWindowTitle("Login");
    return this->exec();
}
