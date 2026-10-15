#include "kanban_board.h"
#include "config.h"

#include <QHBoxLayout>
#include <QScrollArea>
#include <QStyleOption>
#include <QPainter>

KanbanBoardWidget::KanbanBoardWidget(bool darkMode, QWidget *parent)
    : QWidget(parent), m_darkMode(darkMode) {
    setObjectName("KanbanBoard");
    setAttribute(Qt::WA_StyledBackground, true);
    initUI();
}

void KanbanBoardWidget::initUI() {
    auto *outerLayout = new QHBoxLayout(this);
    outerLayout->setContentsMargins(16, 16, 16, 16);
    outerLayout->setSpacing(0);

    auto *scrollArea = new QScrollArea(this);
    scrollArea->setObjectName("BoardScrollArea");
    scrollArea->setWidgetResizable(true);
    scrollArea->setFrameShape(QFrame::NoFrame);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    scrollArea->viewport()->setAutoFillBackground(false);

    auto *contentWidget = new QWidget();
    contentWidget->setObjectName("BoardContent");
    contentWidget->setAttribute(Qt::WA_TranslucentBackground, true);
    contentWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
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
        contentLayout->addWidget(col, 1);
    }

    scrollArea->setWidget(contentWidget);
    outerLayout->addWidget(scrollArea);
}

void KanbanBoardWidget::paintEvent(QPaintEvent *event) {
    QStyleOption opt;
    opt.initFrom(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
    QWidget::paintEvent(event);
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
