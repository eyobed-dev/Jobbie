#include "job_card.h"
#include "config.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QStyle>
#include <QMouseEvent>
#include <QApplication>
#include <QDrag>
#include <QMimeData>
#include <QPainter>
#include <QPixmap>
#include <QDesktopServices>
#include <QUrl>
#include <QDir>
#include <QMessageBox>

JobCardWidget::JobCardWidget(const JobApplication &job, bool darkMode, QWidget *parent)
    : QFrame(parent), m_job(job), m_darkMode(darkMode) {
    setObjectName("JobCard");
    setCursor(Qt::PointingHandCursor);
    setMouseTracking(true);
    initUI();
}

void JobCardWidget::initUI() {
    auto *layout = new QVBoxLayout(this);
    layout->setContentsMargins(12, 12, 12, 12);
    layout->setSpacing(8);

    // 1. Top row: Type badge & Action buttons
    auto *topRow = new QHBoxLayout();
    topRow->setContentsMargins(0, 0, 0, 0);
    topRow->setSpacing(6);

    // Type Badge
    auto typeStyles = Config::getTypeStyles();
    auto style = typeStyles.value(m_job.jobType, typeStyles.value("Full-time"));
    QString bgColor = m_darkMode ? style.bgDark : style.bgLight;
    QString textColor = m_darkMode ? style.textDark : style.textLight;

    auto *typeLabel = new QLabel(m_job.jobType.toUpper(), this);
    typeLabel->setObjectName("TypeBadge");
    typeLabel->setStyleSheet(QString(
        "background-color: %1; color: %2; border-radius: 4px; padding: 2px 7px; font-size: 10px; font-weight: 700;"
    ).arg(bgColor, textColor));
    topRow->addWidget(typeLabel);
    topRow->addStretch();

    // Folder button (if folderPath is configured)
    if (!m_job.folderPath.trimmed().isEmpty()) {
        auto *folderBtn = new QPushButton("📁", this);
        folderBtn->setObjectName("folderBtn");
        folderBtn->setProperty("class", "card-action-btn");
        folderBtn->setToolTip(QString("Open CV & Docs folder:\n%1").arg(m_job.folderPath));
        folderBtn->setCursor(Qt::PointingHandCursor);
        connect(folderBtn, &QPushButton::clicked, this, &JobCardWidget::openFolder);
        topRow->addWidget(folderBtn);
    }

    // Link button (if link exists)
    if (!m_job.link.trimmed().isEmpty()) {
        auto *linkBtn = new QPushButton("🔗", this);
        linkBtn->setObjectName("linkBtn");
        linkBtn->setProperty("class", "card-action-btn");
        linkBtn->setToolTip(QString("Open job posting:\n%1").arg(m_job.link));
        linkBtn->setCursor(Qt::PointingHandCursor);
        connect(linkBtn, &QPushButton::clicked, this, &JobCardWidget::openLink);
        topRow->addWidget(linkBtn);
    }

    // Edit button
    auto *editBtn = new QPushButton("✏️", this);
    editBtn->setObjectName("editBtn");
    editBtn->setProperty("class", "card-action-btn");
    editBtn->setToolTip("Edit Application");
    editBtn->setCursor(Qt::PointingHandCursor);
    connect(editBtn, &QPushButton::clicked, this, [this]() {
        emit editRequested(m_job.id);
    });
    topRow->addWidget(editBtn);

    // Delete button
    auto *delBtn = new QPushButton("🗑️", this);
    delBtn->setObjectName("deleteBtn");
    delBtn->setProperty("class", "card-action-btn");
    delBtn->setToolTip("Delete Application");
    delBtn->setCursor(Qt::PointingHandCursor);
    connect(delBtn, &QPushButton::clicked, this, [this]() {
        emit deleteRequested(m_job.id);
    });
    topRow->addWidget(delBtn);

    layout->addLayout(topRow);

    // 2. Title & Company
    auto *titleLabel = new QLabel(m_job.title, this);
    titleLabel->setObjectName("CardTitle");
    titleLabel->setWordWrap(true);
    layout->addWidget(titleLabel);

    auto *companyLayout = new QHBoxLayout();
    companyLayout->setContentsMargins(0, 0, 0, 0);
    companyLayout->setSpacing(4);

    auto *companyLabel = new QLabel(QString("🏢  %1").arg(m_job.company), this);
    companyLabel->setObjectName("CardCompany");
    companyLayout->addWidget(companyLabel);

    if (!m_job.location.trimmed().isEmpty()) {
        auto *locLabel = new QLabel(QString("• %1").arg(m_job.location), this);
        locLabel->setObjectName("CardCompany");
        companyLayout->addWidget(locLabel);
    }

    companyLayout->addStretch();
    layout->addLayout(companyLayout);

    // 3. Notes snippet if present
    if (!m_job.notes.trimmed().isEmpty()) {
        QString snippet = m_job.notes.trimmed().replace("\n", " ");
        if (snippet.length() > 80) {
            snippet = snippet.left(77) + "...";
        }
        auto *notesLabel = new QLabel(QString("💬 %1").arg(snippet), this);
        notesLabel->setStyleSheet("color: #64748b; font-size: 11px; font-style: italic;");
        notesLabel->setWordWrap(true);
        layout->addWidget(notesLabel);
    }

    // 4. Bottom footer: Salary & Applied Date
    auto *bottomRow = new QHBoxLayout();
    bottomRow->setContentsMargins(0, 4, 0, 0);
    bottomRow->setSpacing(8);

    if (!m_job.salary.trimmed().isEmpty()) {
        auto *salaryLabel = new QLabel(m_job.salary, this);
        salaryLabel->setObjectName("SalaryBadge");
        bottomRow->addWidget(salaryLabel);
    }

    bottomRow->addStretch();

    if (!m_job.dateApplied.trimmed().isEmpty()) {
        auto *dateLabel = new QLabel(QString("📅 %1").arg(m_job.dateApplied), this);
        dateLabel->setObjectName("DateBadge");
        bottomRow->addWidget(dateLabel);
    }

    layout->addLayout(bottomRow);
}

void JobCardWidget::openLink() {
    QString url = m_job.link.trimmed();
    if (url.isEmpty()) return;

    if (!url.startsWith("http://") && !url.startsWith("https://")) {
        url = "https://" + url;
    }
    QDesktopServices::openUrl(QUrl(url));
}

void JobCardWidget::openFolder() {
    QString path = m_job.folderPath.trimmed();
    if (path.isEmpty()) return;

    QDir dir(path);
    if (!dir.exists()) {
        auto res = QMessageBox::question(
            this,
            "Folder Not Found",
            QString("The folder for this application does not exist yet:\n%1\n\nWould you like to create it now?").arg(path),
            QMessageBox::Yes | QMessageBox::No
        );
        if (res == QMessageBox::Yes) {
            if (!dir.mkpath(".")) {
                QMessageBox::critical(this, "Error", "Could not create folder:\n" + path);
                return;
            }
        } else {
            return;
        }
    }

    QDesktopServices::openUrl(QUrl::fromLocalFile(path));
}

void JobCardWidget::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        m_dragStartPos = event->pos();
    }
    QFrame::mousePressEvent(event);
}

void JobCardWidget::mouseMoveEvent(QMouseEvent *event) {
    if (!(event->buttons() & Qt::LeftButton)) {
        return;
    }

    if ((event->pos() - m_dragStartPos).manhattanLength() < QApplication::startDragDistance()) {
        return;
    }

    auto *drag = new QDrag(this);
    auto *mimeData = new QMimeData();
    mimeData->setData("application/x-jobbie-job-id", m_job.id.toUtf8());
    mimeData->setText(m_job.id);
    drag->setMimeData(mimeData);

    // Visual thumbnail
    QPixmap pixmap(size());
    pixmap.fill(Qt::transparent);
    QPainter painter(&pixmap);
    painter.setOpacity(0.85);
    render(&painter);
    painter.end();

    drag->setPixmap(pixmap);
    drag->setHotSpot(event->pos());

    setProperty("dragging", true);
    style()->unpolish(this);
    style()->polish(this);

    drag->exec(Qt::MoveAction);

    setProperty("dragging", false);
    style()->unpolish(this);
    style()->polish(this);
}
