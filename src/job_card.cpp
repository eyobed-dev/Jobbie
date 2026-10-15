#include "icon_helper.h"
#include "job_card.h"
#include "config.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
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
#include <QDate>
#include <QStyle>
#include <QStyleOption>

JobCardWidget::JobCardWidget(const JobApplication &job, bool darkMode, QWidget *parent)
    : QFrame(parent), m_job(job), m_darkMode(darkMode) {
    setObjectName("JobCard");
    setAttribute(Qt::WA_StyledBackground, true);
    setCursor(Qt::PointingHandCursor);
    setMouseTracking(true);
    setToolTip("Click to view & edit details • Drag to move stages");

    initUI();
}

void JobCardWidget::initUI() {
    auto *layout = new QVBoxLayout(this);
    layout->setContentsMargins(12, 12, 12, 12);
    layout->setSpacing(8);

    // 1. Top row: Type badge & Action buttons
    auto *topRow = new QHBoxLayout();
    topRow->setContentsMargins(0, 0, 0, 0);
    topRow->setSpacing(5);

    // Type Badge
    auto typeStyles = Config::getTypeStyles();
    auto style = typeStyles.value(m_job.jobType, typeStyles.value("Full-time"));
    QString bgColor = m_darkMode ? style.bgDark : style.bgLight;
    QString textColor = m_darkMode ? style.textDark : style.textLight;
    QString borderColor = m_darkMode ? style.borderDark : style.borderLight;

    if (m_job.applicationNumber > 0) {
        auto *numBadge = new QLabel(QString("#%1").arg(m_job.applicationNumber, 2, 10, QChar('0')), this);
        numBadge->setObjectName("AppNumBadge");
        numBadge->setStyleSheet(QString(
            "background-color: %1; color: %2; border: 1px solid %3; border-radius: 4px; padding: 2px 6px; font-size: 8pt; font-weight: 700; font-family: monospace;"
        ).arg(m_darkMode ? "#27272a" : "#f4f4f5",
              m_darkMode ? "#d4d4d8" : "#3f3f46",
              m_darkMode ? "#3f3f46" : "#e4e4e7"));
        numBadge->setAttribute(Qt::WA_TransparentForMouseEvents);
        numBadge->setToolTip(QString("Application #%1").arg(m_job.applicationNumber));
        topRow->addWidget(numBadge);
    }

    auto *typeLabel = new QLabel(m_job.jobType.toUpper(), this);
    typeLabel->setObjectName("TypeBadge");
    typeLabel->setStyleSheet(QString(
        "background-color: %1; color: %2; border: 1px solid %3; border-radius: 4px; padding: 2px 7px; font-size: 8.5pt; font-weight: 600; letter-spacing: 0.5px; padding: 2px 7px;"
    ).arg(bgColor, textColor, borderColor));
    typeLabel->setAttribute(Qt::WA_TransparentForMouseEvents);
    topRow->addWidget(typeLabel);
    topRow->addStretch();

    // Folder button (if folderPath is configured)
    if (!m_job.folderPath.trimmed().isEmpty()) {
        auto *folderBtn = new QPushButton(this);
        folderBtn->setIcon(IconHelper::getIcon("folder", m_darkMode ? "#818cf8" : "#6366f1", 14));
        folderBtn->setIconSize(QSize(14, 14));
        folderBtn->setObjectName("folderBtn");
        folderBtn->setProperty("class", "card-action-btn");
        folderBtn->setFixedSize(28, 28);
        folderBtn->setToolTip(QString("Open CV & Docs folder:\n%1").arg(m_job.folderPath));
        folderBtn->setCursor(Qt::PointingHandCursor);
        connect(folderBtn, &QPushButton::clicked, this, &JobCardWidget::openFolder);
        topRow->addWidget(folderBtn);
    }

    // Link button (if link exists)
    if (!m_job.link.trimmed().isEmpty()) {
        auto *linkBtn = new QPushButton(this);
        linkBtn->setIcon(IconHelper::getIcon("external-link", m_darkMode ? "#38bdf8" : "#0284c7", 14));
        linkBtn->setIconSize(QSize(14, 14));
        linkBtn->setObjectName("linkBtn");
        linkBtn->setProperty("class", "card-action-btn");
        linkBtn->setFixedSize(28, 28);
        linkBtn->setToolTip(QString("Open job posting:\n%1").arg(m_job.link));
        linkBtn->setCursor(Qt::PointingHandCursor);
        connect(linkBtn, &QPushButton::clicked, this, &JobCardWidget::openLink);
        topRow->addWidget(linkBtn);
    }

    // Edit button
    auto *editBtn = new QPushButton(this);
    editBtn->setObjectName("editBtn");
    editBtn->setProperty("class", "card-action-btn");
    editBtn->setFixedSize(28, 28);
    editBtn->setIcon(IconHelper::getIcon("pencil", m_darkMode ? "#a1a1aa" : "#71717a", 14));
    editBtn->setIconSize(QSize(14, 14));
    editBtn->setToolTip("Edit Application");
    editBtn->setCursor(Qt::PointingHandCursor);
    connect(editBtn, &QPushButton::clicked, this, [this]() {
        emit editRequested(m_job.id);
    });
    topRow->addWidget(editBtn);

    // Delete button
    auto *delBtn = new QPushButton(this);
    delBtn->setObjectName("deleteBtn");
    delBtn->setProperty("class", "card-action-btn");
    delBtn->setFixedSize(28, 28);
    delBtn->setIcon(IconHelper::getIcon("trash-2", m_darkMode ? "#a1a1aa" : "#71717a", 14));
    delBtn->setIconSize(QSize(14, 14));
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
    titleLabel->setAttribute(Qt::WA_TransparentForMouseEvents);
    layout->addWidget(titleLabel);

    auto *companyLayout = new QHBoxLayout();
    companyLayout->setContentsMargins(0, 0, 0, 0);
    companyLayout->setSpacing(5);

    auto *buildingIcon = new QLabel(this);
    buildingIcon->setFixedSize(14, 14);
    buildingIcon->setScaledContents(true);
    buildingIcon->setPixmap(IconHelper::getPixmap("building-2", m_darkMode ? "#a1a1aa" : "#71717a", 14));
    buildingIcon->setAttribute(Qt::WA_TransparentForMouseEvents);
    companyLayout->addWidget(buildingIcon);

    auto *companyLabel = new QLabel(m_job.company, this);
    companyLabel->setObjectName("CardCompany");
    companyLabel->setAttribute(Qt::WA_TransparentForMouseEvents);
    companyLayout->addWidget(companyLabel);

    if (!m_job.location.trimmed().isEmpty()) {
        auto *locLabel = new QLabel(QString("• %1").arg(m_job.location), this);
        locLabel->setObjectName("CardLocation");
        locLabel->setAttribute(Qt::WA_TransparentForMouseEvents);
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
        auto *notesContainer = new QFrame(this);
        notesContainer->setObjectName("CardNotes");
        notesContainer->setAttribute(Qt::WA_TransparentForMouseEvents);
        auto *notesInner = new QHBoxLayout(notesContainer);
        notesInner->setContentsMargins(6, 4, 8, 4);
        notesInner->setSpacing(6);

        auto *notesIcon = new QLabel(notesContainer);
        notesIcon->setFixedSize(13, 13);
        notesIcon->setScaledContents(true);
        notesIcon->setPixmap(IconHelper::getPixmap("message-square", m_darkMode ? "#fbbf24" : "#d97706", 13));
        notesInner->addWidget(notesIcon, 0, Qt::AlignTop);

        auto *notesLabel = new QLabel(snippet, notesContainer);
        notesLabel->setObjectName("CardNotesText");
        notesLabel->setWordWrap(true);
        notesInner->addWidget(notesLabel, 1);

        layout->addWidget(notesContainer);
    }

    // 4. Bottom footer: Salary & Applied Date
    auto *bottomRow = new QHBoxLayout();
    bottomRow->setContentsMargins(0, 4, 0, 0);
    bottomRow->setSpacing(8);

    if (!m_job.salary.trimmed().isEmpty()) {
        auto *salaryLabel = new QLabel(m_job.salary, this);
        salaryLabel->setObjectName("SalaryBadge");
        salaryLabel->setAttribute(Qt::WA_TransparentForMouseEvents);
        bottomRow->addWidget(salaryLabel);
    }

    bottomRow->addStretch();

    if (!m_job.dateApplied.trimmed().isEmpty()) {
        QString dateText = m_job.dateApplied;
        QDate parsed = QDate::fromString(m_job.dateApplied, "yyyy-MM-dd");
        if (parsed.isValid()) {
            dateText = parsed.toString("MMM d, yyyy");
        }
        auto *dateLayout = new QHBoxLayout();
        dateLayout->setContentsMargins(0, 0, 0, 0);
        dateLayout->setSpacing(4);

        auto *calIcon = new QLabel(this);
        calIcon->setFixedSize(13, 13);
        calIcon->setScaledContents(true);
        calIcon->setPixmap(IconHelper::getPixmap("calendar", m_darkMode ? "#a1a1aa" : "#71717a", 13));
        calIcon->setAttribute(Qt::WA_TransparentForMouseEvents);
        dateLayout->addWidget(calIcon);

        auto *dateLabel = new QLabel(dateText, this);
        dateLabel->setObjectName("DateBadge");
        dateLabel->setAttribute(Qt::WA_TransparentForMouseEvents);
        dateLayout->addWidget(dateLabel);

        bottomRow->addLayout(dateLayout);
    }

    layout->addLayout(bottomRow);
}

void JobCardWidget::paintEvent(QPaintEvent *event) {
    QStyleOption opt;
    opt.initFrom(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
    QFrame::paintEvent(event);
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

    if (path.contains("/Documents/Work/CVs/")) {
        path.replace("/Documents/Work/CVs/", "/Documents/Work/Applications/");
        m_job.folderPath = path;
    }

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

void JobCardWidget::mouseReleaseEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        if ((event->pos() - m_dragStartPos).manhattanLength() < QApplication::startDragDistance()) {
            emit editRequested(m_job.id);
        }
    }
    QFrame::mouseReleaseEvent(event);
}

