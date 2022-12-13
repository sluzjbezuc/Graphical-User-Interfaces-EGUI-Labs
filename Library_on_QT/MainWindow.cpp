#include "MainWindow.hpp"

const QString default_title = "Library System";

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), m_auth(nullptr), m_model(nullptr)
{
    m_auth = new AuthWindow(this);
    m_model = new LibraryModel(this);
    LoadDB(database());

    m_action_change_user = this->menuBar()->addAction("Change User");
    m_delete_account = this->menuBar()->addAction("Delete My Account");
    m_action_exit = this->menuBar()->addAction("Exit");

    auto m_root = new QWidget(this);
    auto m_root_layout = new QVBoxLayout(this);
    m_root->setLayout(m_root_layout);
    this->setCentralWidget(m_root);
    this->setWindowTitle(default_title);
    this->resize(1024, 768);

    auto m_filter_layout = new QFormLayout(this);
    auto m_filter_regexp = new QLineEdit(this);
    m_filter_layout->addRow("Filter", m_filter_regexp);
    m_root_layout->addLayout(m_filter_layout);

    auto m_books_library = new QGroupBox("Library", this);
    auto m_books_library_layout = new QHBoxLayout(this);
    m_books_table = new QTableView(this);
    m_books_library->setLayout(m_books_library_layout);
    m_books_library_layout->addWidget(m_books_table);

    auto m_books_library_info = new QGroupBox("Info", this);
    auto m_books_library_info_layout = new QFormLayout(this);
    m_books_library_info->setLayout(m_books_library_info_layout);
    m_books_library_info->setMaximumWidth(384);
    m_book_lease = new QDateEdit(this);
    m_book_reservation = new QDateEdit(this);
    m_book_lease->setCalendarPopup(true);
    m_book_lease->setMinimumDate(QDate::currentDate());
    m_book_reservation->setCalendarPopup(true);
    m_book_reservation->setMinimumDate(QDate::currentDate());
    auto m_book_buttons = new QHBoxLayout(this);
    m_button_librarian = new QPushButton("Confirm", this);
    m_button_reserve = new QPushButton("Reserve", this);
    m_button_return = new QPushButton("Return", this);
    m_button_cancel = new QPushButton("Cancel Reservation", this);
    m_book_state = new QLineEdit(this);
    m_book_user = new QLineEdit(this);
    m_book_identifier = new QLineEdit(this);
    m_book_title = new QLineEdit(this);
    m_book_author = new QLineEdit(this);
    m_book_year = new QLineEdit(this);
    m_book_user->setReadOnly(true);
    m_book_identifier->setReadOnly(true);
    m_book_title->setReadOnly(true);
    m_book_author->setReadOnly(true);
    m_book_year->setReadOnly(true);
    m_book_buttons->addWidget(m_button_librarian);
    m_book_buttons->addWidget(m_button_reserve);
    m_book_buttons->addWidget(m_button_cancel);
    m_book_buttons->addWidget(m_button_return);
    m_books_library_info_layout->addRow("State: ", m_book_state);
    m_books_library_info_layout->addRow("User: ", m_book_user);
    m_books_library_info_layout->addRow("Identifier: ", m_book_identifier);
    m_books_library_info_layout->addRow("Title: ", m_book_title);
    m_books_library_info_layout->addRow("Author: ", m_book_author);
    m_books_library_info_layout->addRow("Year: ", m_book_year);
    m_books_library_info_layout->addRow("Lease: ", m_book_lease);
    m_books_library_info_layout->addRow("Reservation: ", m_book_reservation);
    m_books_library_info_layout->addRow("Actions:", m_book_buttons);

    auto m_books_layout = new QHBoxLayout(this);
    m_books_layout->addWidget(m_books_library);
    m_books_layout->addWidget(m_books_library_info);
    m_root_layout->addLayout(m_books_layout);

    m_filter = new QSortFilterProxyModel(this);
    m_filter->setSourceModel(m_model);
    m_filter->setFilterKeyColumn(1);
    m_filter->setFilterCaseSensitivity(Qt::CaseInsensitive);

    m_books_table->setModel(m_filter);
    m_books_table->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_books_table->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

    MainWindow::connect(
        m_books_table, &QAbstractItemView::clicked,
        this, &MainWindow::book_clicked
    );
    MainWindow::connect(
        m_action_change_user, &QAction::triggered,
        this, &MainWindow::change_user_clicked
    );
    MainWindow::connect(
        m_delete_account, &QAction::triggered,
        this, &MainWindow::delete_user_clicked
    );
    MainWindow::connect(
        m_action_exit, &QAction::triggered,
        this, &MainWindow::exit_clicked
    );

    MainWindow::connect(
        m_button_reserve, &QPushButton::clicked,
        this, &MainWindow::reserve_clicked
    );
    MainWindow::connect(
        m_button_return, &QPushButton::clicked,
        this, &MainWindow::return_clicked
    );
    MainWindow::connect(
        m_button_cancel, &QPushButton::clicked,
        this, &MainWindow::cancel_clicked
    );
    MainWindow::connect(
        m_button_librarian, &QPushButton::clicked,
        this, &MainWindow::librarian_clicked
    );

    MainWindow::connect(
        m_filter_regexp, &QLineEdit::textChanged,
        this, &MainWindow::filter_changed
    );

    authorize();
}

bool MainWindow::mayDeleteUser(void) const
{
    return !m_auth->isLibrarian()
        && !database().isHoldingAnyBooks(m_auth->activeUser());
}

void MainWindow::librarian_clicked(void)
{
    auto id = selectionID();
    m_model->database().lease(id, QDate::currentDate(), m_book_user->text());
    update_book_info();
    SaveDB(m_model->database());
}

void MainWindow::reserve_clicked(void)
{
    auto id = selectionID();
    m_model->database().reserve(id, QDate::currentDate(), m_auth->activeUser());
    update_book_info();
    SaveDB(m_model->database());
    m_delete_account->setEnabled(false);
}

void MainWindow::cancel_clicked(void)
{
    auto id = selectionID();
    m_model->database().cancel_reservation(id, QDate::currentDate(), m_auth->activeUser());
    update_book_info();
    SaveDB(m_model->database());
    m_delete_account->setEnabled(mayDeleteUser());
}

void MainWindow::return_clicked(void)
{
    auto id = selectionID();
    m_model->database().release(id, QDate::currentDate(), m_auth->activeUser());
    update_book_info();
    SaveDB(m_model->database());
    m_delete_account->setEnabled(mayDeleteUser());
}

void MainWindow::book_clicked(const QModelIndex&)
{
    update_book_info();
}

void MainWindow::update_book_info(void) {
    auto id = selectionID();
    auto book = database().book(id);

    if (m_auth->isLibrarian())
    {
        m_book_user->setText(book.user());
        m_button_return->setEnabled(book.isLeased());
        m_button_librarian->setEnabled(book.isReserved() && !book.isLeased());
    }
    else
    {
        if (!book.isReserved() && !book.isLeased()) {
            m_button_reserve->setText("Reserve");
            m_button_reserve->setEnabled(true);
            m_button_cancel->setEnabled(false);
            m_book_user->setText("[NONE]");
        }
        if (book.isReserved()) {
            m_button_reserve->setEnabled(false);
            if (book.user() == m_auth->activeUser()) {
                m_book_user->setText(book.user());
                m_button_cancel->setEnabled(true);
            }
            else  {
                m_button_cancel->setEnabled(false);
                m_book_user->setText("[HIDDEN]");
            }
        }
    }

    if (book.isLeased() && !book.isReserved())
        m_book_state->setText("Leased");
    if (!book.isLeased() && book.isReserved())
        m_book_state->setText("Reserved");
    if (book.isLeased() && book.isReserved())
        m_book_state->setText("Leased and further reserved");
    if (!book.isLeased() && !book.isReserved())
        m_book_state->setText("Available");

    m_book_identifier->setText(QString::number(id));
    m_book_title->setText(book.title());
    m_book_author->setText(book.author());
    m_book_year->setText(QString::number(book.year()));
}

void MainWindow::filter_changed(const QString& text)
{
    m_filter->setFilterRegularExpression(text);
}

void MainWindow::change_user_clicked(void)
{
    this->authorize();
}

void MainWindow::delete_user_clicked(void)
{
    m_auth->deleteActiveUser();
}

void MainWindow::exit_clicked(void)
{
    this->close();
}

void MainWindow::authorize(void) {
    m_auth->authorize();
    auto user = m_auth->activeUser();
    auto isLibrarian = m_auth->isLibrarian();
    m_button_librarian->setVisible(isLibrarian);
    m_button_librarian->setEnabled(false);
    m_button_return->setVisible(isLibrarian);
    m_button_return->setEnabled(false);
    m_button_reserve->setVisible(!isLibrarian);
    m_button_reserve->setEnabled(false);
    m_button_cancel->setVisible(true);
    m_button_cancel->setEnabled(false);
    m_book_user->setEnabled(isLibrarian);
    m_delete_account->setEnabled(mayDeleteUser());
    this->setWindowTitle(default_title + " - " + m_auth->activeUser());
}

qint32 MainWindow::selectionID(void) const
{
    auto row = m_books_table->selectionModel()
            ->selection().indexes()[0].row();
    auto id_index = m_model->index(row, 0);
    return id_index.data().toInt();
}
