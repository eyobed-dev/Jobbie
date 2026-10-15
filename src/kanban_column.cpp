#include "icon_helper.h"
#include "kanban_column.h"
#include "config.h"

#include <QHBoxLayout>
#include <QPushButton>
#include <QStyle>
#include <QStyleOption>
#include <QPainter>
#include <QDragEnterEvent>
#include <QDragMoveEvent>
#include <QDragLeaveEvent>
#include <QDropEvent>
#include <QMimeData>

KanbanColumnWidget::KanbanColumnWidget(const QString &status, bool darkMode, QWidget *parent)
    : QFrame(parent), m_status(status), m_darkMode(darkMode) {
    setObjectName("KanbanColumn");
    setAttribute(Qt::WA_StyledBackground, true);
    setAcceptDrops(true);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    setMinimumWidth(270);
    setMaximumWidth(QWIDGETSIZE_MAX);
    initUI();
}

void KanbanColumnWidget::initUI() {
    auto *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 8);
    mainLayout->setSpacing(2);

    // Colored stage accent strip across the top of the column
    auto metaMap = Config::getStatusMeta();
    auto meta = metaMap.value(m_status);
    QString colColor = m_darkMode ? meta.darkColor : meta.lightColor;

    m_topAccent = new QFrame(this);
    m_topAccent->setFixedHeight(3);
    m_topAccent->setStyleSheet(QString(
        "background-color: %1; border-top-left-radius: 8px; border-top-right-radius: 8px; border: none;"
    ).arg(colColor));
    mainLayout->addWidget(m_topAccent);

    // 1. Column Header
    auto *headerFrame = new QFrame(this);
    headerFrame->setObjectName("ColumnHeader");
    auto *headerLayout = new QHBoxLayout(headerFrame);
    headerLayout->setContentsMargins(12, 6, 12, 6);
    headerLayout->setSpacing(8);

    m_iconLabel = new QLabel(headerFrame);
    m_iconLabel->setFixedSize(18, 18);
    m_iconLabel->setScaledContents(true);
    m_iconLabel->setPixmap(IconHelper::getPixmap(meta.icon, colColor, 18));
    headerLayout->addWidget(m_iconLabel);

    auto *titleLabel = new QLabel(m_status, headerFrame);
    titleLabel->setObjectName("ColumnTitle");
    headerLayout->addWidget(titleLabel);

    m_countBadge = new QLabel("0", headerFrame);
    m_countBadge->setObjectName("ColumnCount");
    headerLayout->addWidget(m_countBadge);

    headerLayout->addStretch();

    // Quick add button in column header
    m_addBtn = new QPushButton(headerFrame);
    m_addBtn->setObjectName("ColumnAddBtn");
    m_addBtn->setToolTip(QString("Add application to %1").arg(m_status));
    m_addBtn->setCursor(Qt::PointingHandCursor);
    m_addBtn->setFixedSize(26, 26);
    m_addBtn->setIcon(IconHelper::getIcon("plus", m_darkMode ? "#a1a1aa" : "#71717a", 13));
    m_addBtn->setIconSize(QSize(13, 13));
    connect(m_addBtn, &QPushButton::clicked, this, [this]() {
        emit addRequested(m_status);
    });
    headerLayout->addWidget(m_addBtn);

    mainLayout->addWidget(headerFrame);

    // 2. Scrollable Area
    m_scrollArea = new QScrollArea(this);
    m_scrollArea->setObjectName("ColumnScrollArea");
    m_scrollArea->setWidgetResizable(true);
    m_scrollArea->setFrameShape(QFrame::NoFrame);
    m_scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_scrollArea->viewport()->setAutoFillBackground(false);

    m_cardsContainer = new QWidget();
    m_cardsContainer->setObjectName("ColumnCardsContainer");
    m_cardsContainer->setAttribute(Qt::WA_TranslucentBackground, true);
    m_cardsLayout = new QVBoxLayout(m_cardsContainer);
    m_cardsLayout->setContentsMargins(8, 4, 8, 8);
    m_cardsLayout->setSpacing(10);
    m_cardsLayout->addStretch();

    m_scrollArea->setWidget(m_cardsContainer);
    mainLayout->addWidget(m_scrollArea);
}

void KanbanColumnWidget::paintEvent(QPaintEvent *event) {
    QStyleOption opt;
    opt.initFrom(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
    QFrame::paintEvent(event);
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
        placeholder->setStyleSheet(QString(
            "color: %1; font-size: 12px; font-weight: 500; "
            "border: 1px dashed %2; border-radius: 6px; padding: 28px 10px; margin: 4px 0;"
        ).arg(m_darkMode ? "#71717a" : "#a1a1aa", m_darkMode ? "#27272a" : "#e4e4e7"));
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
    auto metaMap = Config::getStatusMeta();
    auto meta = metaMap.value(m_status);
    QString colColor = m_darkMode ? meta.darkColor : meta.lightColor;
    if (m_topAccent) {
        m_topAccent->setStyleSheet(QString(
            "background-color: %1; border-top-left-radius: 8px; border-top-right-radius: 8px; border: none;"
        ).arg(colColor));
    }
    if (m_iconLabel) {
        m_iconLabel->setPixmap(IconHelper::getPixmap(meta.icon, colColor, 18));
    }
    if (m_addBtn) {
        m_addBtn->setIcon(IconHelper::getIcon("plus", m_darkMode ? "#a1a1aa" : "#71717a", 13));
    }
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
