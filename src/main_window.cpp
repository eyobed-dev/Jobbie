#include "main_window.h"
#include "config.h"
#include "styles.h"
#include "job_dialog.h"
#include "confirm_dialog.h"
#include "native_dialog.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFrame>
#include <QShortcut>
#include <QKeySequence>
#include <QFileDialog>
#include <QMessageBox>
#include <QSet>

MainWindow::MainWindow(const StorageManager &storage, bool darkMode, QWidget *parent)
    : QMainWindow(parent), m_storage(storage), m_darkMode(darkMode) {
    setWindowTitle("Jobbie — Native C++ Job Pipeline & Kanban");
    resize(1340, 840);
    setMinimumSize(960, 600);

    initUI();
    applyTheme();
    loadData();
    setupShortcuts();
}

void MainWindow::initUI() {
    setWindowIcon(QIcon(":/icons/icon.png"));

    auto *centralWidget = new QWidget(this);
    auto *mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    // 1. Header Bar
    auto *headerBar = new QFrame(centralWidget);
    headerBar->setObjectName("HeaderBar");
    auto *headerLayout = new QVBoxLayout(headerBar);
    headerLayout->setContentsMargins(16, 12, 16, 12);
    headerLayout->setSpacing(10);

    // Top Row: Brand + Stats + Actions
    auto *topRow = new QHBoxLayout();
    topRow->setSpacing(16);

    auto *logoLayout = new QHBoxLayout();
    logoLayout->setSpacing(8);
    auto *logoIconLabel = new QLabel(headerBar);
    QPixmap iconPix(":/icons/icon_64.png");
    logoIconLabel->setPixmap(iconPix);
    logoIconLabel->setFixedSize(28, 28);
    logoIconLabel->setScaledContents(true);
    logoLayout->addWidget(logoIconLabel);

    auto *brandLabel = new QLabel("Jobbie", headerBar);
    brandLabel->setObjectName("AppTitle");
    logoLayout->addWidget(brandLabel);
    topRow->addLayout(logoLayout);

    m_badgeTotal = new QLabel("0 Total", headerBar);
    m_badgeTotal->setProperty("class", "metric-badge");

    m_badgeActive = new QLabel("0 Active", headerBar);
    m_badgeActive->setProperty("class", "metric-badge");

    m_badgeOffers = new QLabel("0 Offers", headerBar);
    m_badgeOffers->setProperty("class", "metric-badge");
    m_badgeOffers->setStyleSheet("color: #059669; font-weight: bold;");

    topRow->addWidget(m_badgeTotal);
    topRow->addWidget(m_badgeActive);
    topRow->addWidget(m_badgeOffers);
    topRow->addStretch();

    // Export button
    auto *exportBtn = new QPushButton("Backup", headerBar);
    exportBtn->setToolTip("Export jobs to JSON (Ctrl+E)");
    exportBtn->setCursor(Qt::PointingHandCursor);
    connect(exportBtn, &QPushButton::clicked, this, &MainWindow::exportJobs);
    topRow->addWidget(exportBtn);

    // Import button
    auto *importBtn = new QPushButton("Import", headerBar);
    importBtn->setToolTip("Import jobs from JSON (Ctrl+I)");
    importBtn->setCursor(Qt::PointingHandCursor);
    connect(importBtn, &QPushButton::clicked, this, &MainWindow::importJobs);
    topRow->addWidget(importBtn);

    // Theme toggle
    m_themeBtn = new QPushButton(m_darkMode ? "☀️ Light" : "🌙 Dark", headerBar);
    m_themeBtn->setToolTip("Toggle Theme (Ctrl+T)");
    m_themeBtn->setCursor(Qt::PointingHandCursor);
    connect(m_themeBtn, &QPushButton::clicked, this, &MainWindow::toggleTheme);
    topRow->addWidget(m_themeBtn);

    // Add Job button
    auto *addJobBtn = new QPushButton("+ Add Job", headerBar);
    addJobBtn->setObjectName("PrimaryButton");
    addJobBtn->setToolTip("Add new application (Ctrl+N)");
    addJobBtn->setCursor(Qt::PointingHandCursor);
    connect(addJobBtn, &QPushButton::clicked, this, [this]() {
        openJobDialog();
    });
    topRow->addWidget(addJobBtn);

    headerLayout->addLayout(topRow);

    // Bottom Row: Live Search + Filter Chips
    auto *controlsRow = new QHBoxLayout();
    controlsRow->setSpacing(12);

    m_searchInput = new QLineEdit(headerBar);
    m_searchInput->setPlaceholderText("🔍  Search company, role, or notes... (Ctrl+F)");
    m_searchInput->setClearButtonEnabled(true);
    m_searchInput->setFixedWidth(340);
    connect(m_searchInput, &QLineEdit::textChanged, this, &MainWindow::onSearchChanged);
    controlsRow->addWidget(m_searchInput);

    controlsRow->addSpacing(6);

    auto *filterLabel = new QLabel("Filter:", headerBar);
    filterLabel->setStyleSheet("color: #64748b; font-size: 12px; font-weight: 600;");
    controlsRow->addWidget(filterLabel);

    m_filterGroup = new QButtonGroup(this);
    m_filterGroup->setExclusive(true);

    auto *allChip = new QPushButton("All", headerBar);
    allChip->setCheckable(true);
    allChip->setChecked(true);
    allChip->setProperty("class", "filter-chip");
    allChip->setCursor(Qt::PointingHandCursor);
    connect(allChip, &QPushButton::clicked, this, [this]() { onFilterChanged("All"); });
    m_filterGroup->addButton(allChip);
    controlsRow->addWidget(allChip);

    for (const auto &type : Config::JOB_TYPES) {
        auto *chip = new QPushButton(type, headerBar);
        chip->setCheckable(true);
        chip->setProperty("class", "filter-chip");
        chip->setCursor(Qt::PointingHandCursor);
        connect(chip, &QPushButton::clicked, this, [this, type]() { onFilterChanged(type); });
        m_filterGroup->addButton(chip);
        controlsRow->addWidget(chip);
    }

    controlsRow->addStretch();
    headerLayout->addLayout(controlsRow);

    mainLayout->addWidget(headerBar);

    // 2. Kanban Board
    m_board = new KanbanBoardWidget(m_darkMode, centralWidget);
    connect(m_board, &KanbanBoardWidget::jobMoved, this, &MainWindow::onJobMoved);
    connect(m_board, &KanbanBoardWidget::addJobRequested, this, &MainWindow::openJobDialog);
    connect(m_board, &KanbanBoardWidget::editJobRequested, this, &MainWindow::editJobById);
    connect(m_board, &KanbanBoardWidget::deleteJobRequested, this, &MainWindow::confirmDeleteJob);
    mainLayout->addWidget(m_board, 1);

    setCentralWidget(centralWidget);
}

void MainWindow::setupShortcuts() {
    auto *newShortcut = new QShortcut(QKeySequence("Ctrl+N"), this);
    connect(newShortcut, &QShortcut::activated, this, [this]() { openJobDialog(); });

    auto *searchShortcut = new QShortcut(QKeySequence("Ctrl+F"), this);
    connect(searchShortcut, &QShortcut::activated, m_searchInput, qOverload<>(&QWidget::setFocus));

    auto *exportShortcut = new QShortcut(QKeySequence("Ctrl+E"), this);
    connect(exportShortcut, &QShortcut::activated, this, &MainWindow::exportJobs);

    auto *importShortcut = new QShortcut(QKeySequence("Ctrl+I"), this);
    connect(importShortcut, &QShortcut::activated, this, &MainWindow::importJobs);

    auto *themeShortcut = new QShortcut(QKeySequence("Ctrl+T"), this);
    connect(themeShortcut, &QShortcut::activated, this, &MainWindow::toggleTheme);
}

void MainWindow::applyTheme() {
    setStyleSheet(Styles::getStylesheet(m_darkMode));
    m_board->setDarkMode(m_darkMode);
    m_themeBtn->setText(m_darkMode ? "☀️ Light" : "🌙 Dark");
}

void MainWindow::toggleTheme() {
    m_darkMode = !m_darkMode;
    applyTheme();
    refreshBoard();
}

void MainWindow::loadData() {
    m_jobs = m_storage.loadJobs();
    refreshBoard();
}

void MainWindow::saveData() {
    m_storage.saveJobs(m_jobs);
    updateStats();
}

void MainWindow::updateStats() {
    int total = m_jobs.size();
    int active = 0;
    int offers = 0;
    for (const auto &j : m_jobs) {
        if (j.status == "Applied" || j.status == "Interviewing") {
            active++;
        }
        if (j.status == "Offered") {
            offers++;
        }
    }

    m_badgeTotal->setText(QString("%1 Total").arg(total));
    m_badgeActive->setText(QString("%1 Active").arg(active));
    m_badgeOffers->setText(offers == 1 ? "1 Offer" : QString("%1 Offers").arg(offers));
}

QList<JobApplication> MainWindow::getFilteredJobs() const {
    QString query = m_searchQuery.toLower().trimmed();
    QList<JobApplication> filtered;

    for (const auto &j : m_jobs) {
        if (m_activeTypeFilter != "All" && j.jobType != m_activeTypeFilter) {
            continue;
        }

        if (!query.isEmpty()) {
            QString haystack = QString("%1 %2 %3 %4").arg(j.company, j.title, j.notes, j.location).toLower();
            if (!haystack.contains(query)) {
                continue;
            }
        }

        filtered.append(j);
    }
    return filtered;
}

void MainWindow::refreshBoard() {
    m_board->setJobs(getFilteredJobs());
    updateStats();
}

void MainWindow::onSearchChanged(const QString &text) {
    m_searchQuery = text;
    refreshBoard();
}

void MainWindow::onFilterChanged(const QString &jobType) {
    m_activeTypeFilter = jobType;
    refreshBoard();
}

void MainWindow::onJobMoved(const QString &jobId, const QString &newStatus) {
    for (auto &j : m_jobs) {
        if (j.id == jobId) {
            if (j.status != newStatus) {
                j.status = newStatus;
                saveData();
                refreshBoard();
            }
            break;
        }
    }
}

void MainWindow::openJobDialog(const QString &defaultStatus) {
    JobDialog dialog(nullptr, defaultStatus, this);
    if (dialog.exec() == QDialog::Accepted) {
        m_jobs.append(dialog.getJob());
        saveData();
        refreshBoard();
    }
}

void MainWindow::editJobById(const QString &jobId) {
    for (int i = 0; i < m_jobs.size(); ++i) {
        if (m_jobs[i].id == jobId) {
            JobDialog dialog(&m_jobs[i], m_jobs[i].status, this);
            if (dialog.exec() == QDialog::Accepted) {
                m_jobs[i] = dialog.getJob();
                saveData();
                refreshBoard();
            }
            break;
        }
    }
}

void MainWindow::confirmDeleteJob(const QString &jobId) {
    for (int i = 0; i < m_jobs.size(); ++i) {
        if (m_jobs[i].id == jobId) {
            QString msg = QString("Are you sure you want to delete '%1' at '%2'? This action cannot be undone.")
                .arg(m_jobs[i].title, m_jobs[i].company);
            ConfirmDialog dialog("Delete Application", msg, this);
            if (dialog.exec() == QDialog::Accepted) {
                m_jobs.removeAt(i);
                saveData();
                refreshBoard();
            }
            break;
        }
    }
}

void MainWindow::exportJobs() {
    QString path = NativeDialog::getSaveFileName(
        this, "Backup Jobs to JSON", "jobbie_backup.json", "*.json"
    );
    if (!path.isEmpty()) {
        if (m_storage.exportToFile(path, m_jobs)) {
            QMessageBox::information(this, "Backup Complete", QString("Successfully backed up %1 applications to:\n%2").arg(m_jobs.size()).arg(path));
        } else {
            QMessageBox::critical(this, "Backup Error", "Failed to write backup file.");
        }
    }
}

void MainWindow::importJobs() {
    QString path = NativeDialog::getOpenFileName(
        this, "Import Jobs from JSON", "", "*.json"
    );
    if (!path.isEmpty()) {
        QList<JobApplication> imported;
        if (m_storage.importFromFile(path, imported)) {
            auto res = QMessageBox::question(
                this,
                "Import Mode",
                QString("Found %1 applications in backup.\n\nClick 'Yes' to merge with existing jobs, or 'No' to replace all existing jobs.")
                    .arg(imported.size()),
                QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel
            );
            if (res == QMessageBox::Yes) {
                QSet<QString> existingIds;
                for (const auto &j : m_jobs) existingIds.insert(j.id);
                for (const auto &j : imported) {
                    if (!existingIds.contains(j.id)) {
                        m_jobs.append(j);
                    }
                }
                saveData();
                refreshBoard();
                QMessageBox::information(this, "Import Complete", QString("Merged applications. Total jobs now: %1").arg(m_jobs.size()));
            } else if (res == QMessageBox::No) {
                m_jobs = imported;
                saveData();
                refreshBoard();
                QMessageBox::information(this, "Import Complete", QString("Replaced with %1 applications.").arg(m_jobs.size()));
            }
        } else {
            QMessageBox::critical(this, "Import Error", "Failed to parse JSON file.");
        }
    }
}
