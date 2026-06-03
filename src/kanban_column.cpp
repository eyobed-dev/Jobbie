#include "kanban_column.h"
#include "config.h"

#include <QHBoxLayout>
#include <QPushButton>
#include <QStyle>
#include <QDragEnterEvent>
#include <QDragMoveEvent>
#include <QDragLeaveEvent>
#include <QDropEvent>
#include <QMimeData>

KanbanColumnWidget::KanbanColumnWidget(const QString &status, bool darkMode, QWidget *parent)
    : QFrame(parent), m_status(status), m_darkMode(darkMode) {
    setObjectName("KanbanColumn");
    setAcceptDrops(true);
    setMinimumWidth(280);
    setMaximumWidth(360);
    initUI();
}

void KanbanColumnWidget::initUI() {
    auto *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(6, 6, 6, 6);
    mainLayout->setSpacing(8);

    // 1. Column Header
    auto *headerFrame = new QFrame(this);
    headerFrame->setObjectName("ColumnHeader");
    auto *headerLayout = new QHBoxLayout(headerFrame);
    headerLayout->setContentsMargins(8, 6, 8, 8);
    headerLayout->setSpacing(8);

    auto metaMap = Config::getStatusMeta();
    auto meta = metaMap.value(m_status);

    auto *iconLabel = new QLabel(meta.icon, headerFrame);
    iconLabel->setStyleSheet("font-size: 15px;");
    headerLayout->addWidget(iconLabel);

    auto *titleLabel = new QLabel(m_status, headerFrame);
    titleLabel->setObjectName("ColumnTitle");
    headerLayout->addWidget(titleLabel);

    m_countBadge = new QLabel("0", headerFrame);
    m_countBadge->setObjectName("ColumnCount");
    headerLayout->addWidget(m_countBadge);

    headerLayout->addStretch();

    // Quick add button in column header
    auto *addBtn = new QPushButton("+", headerFrame);
    addBtn->setToolTip(QString("Add application to %1").arg(m_status));
    addBtn->setCursor(Qt::PointingHandCursor);
    addBtn->setFixedSize(24, 24);
    addBtn->setStyleSheet("font-size: 14px; font-weight: bold; border-radius: 12px; padding: 0px;");
    connect(addBtn, &QPushButton::clicked, this, [this]() {
        emit addRequested(m_status);
    });
    headerLayout->addWidget(addBtn);

    mainLayout->addWidget(headerFrame);

    // 2. Scrollable Area
    m_scrollArea = new QScrollArea(this);
    m_scrollArea->setWidgetResizable(true);
    m_scrollArea->setFrameShape(QFrame::NoFrame);
    m_scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    m_cardsContainer = new QWidget();
    m_cardsLayout = new QVBoxLayout(m_cardsContainer);
    m_cardsLayout->setContentsMargins(4, 4, 4, 4);
    m_cardsLayout->setSpacing(10);
    m_cardsLayout->addStretch();

    m_scrollArea->setWidget(m_cardsContainer);
    mainLayout->addWidget(m_scrollArea);
}

void KanbanColumnWidget::setJobs(const QList<JobApplication> &jobs) {
    // Clear existing widgets
    while (m_cardsLayout->count() > 1) {
        auto *item = m_cardsLayout->takeAt(0);
        if (item->widget()) {
            item->widget()->deleteLater();
        }
        delete item;
    }
    m_cards.clear();

    m_countBadge->setText(QString::number(jobs.size()));

    if (jobs.isEmpty()) {
        auto *placeholder = new QLabel("Drop applications here\nor click + to add", m_cardsContainer);
        placeholder->setAlignment(Qt::AlignCenter);
        placeholder->setStyleSheet(
            "color: #94a3b8; font-size: 12px; font-style: italic; "
            "border: 1.5px dashed #cbd5e1; border-radius: 10px; padding: 30px 10px; margin: 6px 0;"
        );
        m_cardsLayout->insertWidget(0, placeholder);
        return;
    }

    for (int i = 0; i < jobs.size(); ++i) {
        auto *card = new JobCardWidget(jobs[i], m_darkMode, m_cardsContainer);
        connect(card, &JobCardWidget::editRequested, this, &KanbanColumnWidget::editRequested);
        connect(card, &JobCardWidget::deleteRequested, this, &KanbanColumnWidget::deleteRequested);
        m_cards.append(card);
        m_cardsLayout->insertWidget(i, card);
    }
}

void KanbanColumnWidget::setDarkMode(bool darkMode) {
    m_darkMode = darkMode;
}

void KanbanColumnWidget::dragEnterEvent(QDragEnterEvent *event) {
    if (event->mimeData()->hasFormat("application/x-jobbie-job-id")) {
        event->acceptProposedAction();
        setProperty("dragOver", true);
        style()->unpolish(this);
        style()->polish(this);
    } else {
        event->ignore();
    }
}

void KanbanColumnWidget::dragMoveEvent(QDragMoveEvent *event) {
    if (event->mimeData()->hasFormat("application/x-jobbie-job-id")) {
        event->acceptProposedAction();
    } else {
        event->ignore();
    }
}

void KanbanColumnWidget::dragLeaveEvent(QDragLeaveEvent *event) {
    setProperty("dragOver", false);
    style()->unpolish(this);
    style()->polish(this);
    event->accept();
}

void KanbanColumnWidget::dropEvent(QDropEvent *event) {
    setProperty("dragOver", false);
    style()->unpolish(this);
    style()->polish(this);

    if (event->mimeData()->hasFormat("application/x-jobbie-job-id")) {
        QString jobId = QString::fromUtf8(event->mimeData()->data("application/x-jobbie-job-id"));
        event->acceptProposedAction();
        emit jobDropped(jobId, m_status);
    } else {
        event->ignore();
    }
}
