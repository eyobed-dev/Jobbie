#include "kanban_board.h"
#include "config.h"

#include <QHBoxLayout>
#include <QScrollArea>

KanbanBoardWidget::KanbanBoardWidget(bool darkMode, QWidget *parent)
    : QWidget(parent), m_darkMode(darkMode) {
    initUI();
}

void KanbanBoardWidget::initUI() {
    auto *outerLayout = new QHBoxLayout(this);
    outerLayout->setContentsMargins(12, 12, 12, 12);
    outerLayout->setSpacing(0);

    auto *scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setFrameShape(QFrame::NoFrame);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    auto *contentWidget = new QWidget();
    auto *contentLayout = new QHBoxLayout(contentWidget);
    contentLayout->setContentsMargins(0, 0, 0, 0);
    contentLayout->setSpacing(14);

    for (const auto &status : Config::STATUSES) {
        auto *col = new KanbanColumnWidget(status, m_darkMode, contentWidget);
        connect(col, &KanbanColumnWidget::jobDropped, this, &KanbanBoardWidget::jobMoved);
        connect(col, &KanbanColumnWidget::addRequested, this, &KanbanBoardWidget::addJobRequested);
        connect(col, &KanbanColumnWidget::editRequested, this, &KanbanBoardWidget::editJobRequested);
        connect(col, &KanbanColumnWidget::deleteRequested, this, &KanbanBoardWidget::deleteJobRequested);

        m_columns[status] = col;
        contentLayout->addWidget(col);
    }

    scrollArea->setWidget(contentWidget);
    outerLayout->addWidget(scrollArea);
}

void KanbanBoardWidget::setJobs(const QList<JobApplication> &jobs) {
    for (auto it = m_columns.begin(); it != m_columns.end(); ++it) {
        QString status = it.key();
        QList<JobApplication> colJobs;
        for (const auto &j : jobs) {
            if (j.status == status) {
                colJobs.append(j);
            }
        }
        it.value()->setJobs(colJobs);
    }
}

void KanbanBoardWidget::setDarkMode(bool darkMode) {
    m_darkMode = darkMode;
    for (auto *col : m_columns) {
        col->setDarkMode(darkMode);
    }
}
