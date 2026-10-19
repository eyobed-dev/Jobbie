#pragma once

#include <QMainWindow>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QButtonGroup>
#include <QAction>
#include <QList>

#include "models.h"
#include "storage.h"
#include "kanban_board.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(const StorageManager &storage, bool darkMode = false, QWidget *parent = nullptr);

private slots:
    void onSearchChanged(const QString &text);
    void onFilterChanged(const QString &jobType);
    void onJobMoved(const QString &jobId, const QString &newStatus);
    void openJobDialog(const QString &defaultStatus = "Wishlist");
    void editJobById(const QString &jobId);
    void confirmDeleteJob(const QString &jobId);
    void exportJobs();
    void importJobs();
    void toggleTheme();

private:
    void initUI();
    void setupShortcuts();
    void applyTheme();
    void loadData();
    void saveData();
    void updateStats();
    void refreshBoard();
    QList<JobApplication> getFilteredJobs() const;
    int getNextApplicationNumber() const;

    StorageManager m_storage;
    bool m_darkMode;
    QList<JobApplication> m_jobs;
    QString m_activeTypeFilter = "All";
    QString m_searchQuery;

    QLabel *m_badgeTotal = nullptr;
    QLabel *m_badgeActive = nullptr;
    QLabel *m_badgeOffers = nullptr;
    QPushButton *m_themeBtn = nullptr;
    QPushButton *m_addJobBtn = nullptr;
    QPushButton *m_exportBtn = nullptr;
    QPushButton *m_importBtn = nullptr;
    QAction *m_searchAction = nullptr;
    QLineEdit *m_searchInput = nullptr;
    QButtonGroup *m_filterGroup = nullptr;
    KanbanBoardWidget *m_board = nullptr;
};
