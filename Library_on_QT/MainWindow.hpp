#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include "LibraryUtilities.hpp"
#include "LibraryModel.hpp"
#include "AuthWindow.hpp"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QTableView>
#include <QLineEdit>
#include <QGroupBox>
#include <QHeaderView>
#include <QMessageBox>
#include <QAbstractItemView>
#include <QCalendarWidget>
#include <QSortFilterProxyModel>
#include <QDateEdit>
#include <QPushButton>
#include <QLineEdit>
#include <QSplitter>
#include <QWidget>
#include <QMainWindow>
#include <QMenuBar>
#include <QAction>

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);

    const BookDB& books(void) const { return m_model->database(); }
    BookDB& books(void) { return m_model->database(); }

private slots:
    void librarian_clicked(void);
    void reserve_clicked(void);
    void cancel_clicked(void);
    void return_clicked(void);

    void book_clicked(const QModelIndex &index);
    void delete_user_clicked(void);
    void change_user_clicked(void);
    void exit_clicked(void);

    void filter_changed(const QString &text);

private:
    bool mayDeleteUser(void) const;
    void authorize(void);
    void update_book_info(void);
    qint32 selectionID(void) const;

    const BookDB& database(void) const { return m_model->database(); }
    BookDB& database(void) { return m_model->database(); }

    QAction* m_action_change_user;
    QAction* m_action_exit;

    AuthWindow* m_auth;
    LibraryModel* m_model;
    QSortFilterProxyModel* m_filter;

    QAction* m_delete_account;
    QTableView* m_books_table;
    QLineEdit* m_book_state;
    QLineEdit* m_book_user;
    QLineEdit* m_book_identifier;
    QLineEdit* m_book_title;
    QLineEdit* m_book_author;
    QLineEdit* m_book_publisher;
    QLineEdit* m_book_year;
    QDateEdit * m_book_lease;
    QDateEdit * m_book_reservation;
    QPushButton* m_button_librarian;
    QPushButton* m_button_reserve;
    QPushButton* m_button_cancel;
    QPushButton* m_button_return;
};

#endif // MAINWINDOW_HPP
