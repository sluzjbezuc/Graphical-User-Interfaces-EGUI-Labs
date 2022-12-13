#include "AuthWindow.hpp"
#include "LibraryUtilities.hpp"

#include <QDialogButtonBox>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QPushButton>
#include <QLineEdit>

AuthWindow::AuthWindow(QWidget* parent) : QDialog(parent)
{
    auto root_layout = new QVBoxLayout(this);
    auto form_layout = new QFormLayout(this);
    auto hbox_layout = new QHBoxLayout(this);
    auto login_button = new QPushButton("Login", this);
    auto cancel_button = new QPushButton("Cancel", this);
    auto register_button = new QPushButton("Register", this);
    m_user = new QLineEdit(this);
    m_pass = new QLineEdit(this);
    root_layout->addLayout(form_layout);
    root_layout->addLayout(hbox_layout);
    hbox_layout->addWidget(login_button);
    hbox_layout->addWidget(cancel_button);
    hbox_layout->addWidget(register_button);
    form_layout->addRow("Username", m_user);
    form_layout->addRow("Password", m_pass);
    m_pass->setEchoMode(QLineEdit::Password);
    this->setLayout(root_layout);
    this->setWindowTitle("Login or Register");
    QObject::connect(
        login_button, &QPushButton::clicked,
        this, &AuthWindow::authorization
    );
    QObject::connect(
        cancel_button, &QPushButton::clicked,
        this, &AuthWindow::reject
    );
    QObject::connect(
        register_button, &QPushButton::clicked,
        this, &AuthWindow::registration
    );

    LoadDB(m_db);
}

void AuthWindow::authorize(void)
{
    while (exec() != QDialog::Accepted)
    {
        QMessageBox::warning(nullptr,
            "Authorization Required!",
            "Please, login or register"
        );
    }
}

void AuthWindow::registration(void)
{
    auto user = m_user->text();
    if (!m_db.exists(user))
    {
        m_db.append(user);
        m_db.password(user) = m_pass->text();
        m_active_user = user;
        SaveDB(m_db);
        accept();
    }
    else
    {
        QMessageBox::warning(this,
            "Registration failed",
            "A user with the given name already exists!"
        );
    }
}

void AuthWindow::authorization(void)
{
    auto user = m_user->text();
    auto pass = m_pass->text();
    if (m_db.exists(user) && m_db.password(user) == pass)
    {
        m_active_user = user;
        accept();
    }
    else
    {
        QMessageBox::warning(this,
            "Authorization failed",
            "The login/password combination you provided is invalid or does not exist!"
        );
    }
}

void AuthWindow::deleteActiveUser(void)
{
    m_db.remove(m_active_user);
    SaveDB(m_db);
    m_active_user = "";
    authorize();
}

bool AuthWindow::isLibrarian(void) const {
    return m_db.isLibrarian(m_active_user);
}
