#include "icon_helper.h"
#include "job_dialog.h"
#include "config.h"
#include "native_dialog.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QFileDialog>
#include <QMessageBox>
#include <QDesktopServices>
#include <QUrl>
#include <QDir>
#include <QDate>

JobDialog::JobDialog(const JobApplication *job, const QString &defaultStatus, int nextAppNumber, QWidget *parent)
    : QDialog(parent), m_defaultStatus(defaultStatus) {
    if (job) {
        m_isEdit = true;
        m_job = *job;
        if (m_job.applicationNumber <= 0) {
            m_job.applicationNumber = nextAppNumber;
        }
        setWindowTitle(QString("#%1 %2 at %3 — Jobbie").arg(QString("%1").arg(m_job.applicationNumber, 2, 10, QChar('0')), m_job.company, m_job.title));
    } else {
        m_isEdit = false;
        m_job.applicationNumber = nextAppNumber;
        setWindowTitle(QString("New Job Application (#%1) — Jobbie").arg(QString("%1").arg(nextAppNumber, 2, 10, QChar('0'))));
    }

    setMinimumWidth(640);
    setModal(true);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

    initUI();
    populateData();
}

void JobDialog::initUI() {
    auto *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(24, 20, 24, 20);
    mainLayout->setSpacing(16);

    QString numStr = QString("%1").arg(m_job.applicationNumber, 2, 10, QChar('0'));
    QString titleText = m_isEdit
        ? QString("Application #%1: %2 at %3").arg(numStr, m_job.company, m_job.title)
        : QString("New Job Application (#%1)").arg(numStr);
    auto *headerTitle = new QLabel(titleText, this);
    headerTitle->setObjectName("DialogHeaderTitle");
    headerTitle->setStyleSheet("font-size: 17px; font-weight: bold;");
    mainLayout->addWidget(headerTitle);

    auto *grid = new QGridLayout();
    grid->setHorizontalSpacing(16);
    grid->setVerticalSpacing(14);

    // Row 0: Company & Title
    auto *lblCompany = new QLabel("Company Name *", this);
    lblCompany->setObjectName("FieldLabel");
    m_companyInput = new QLineEdit(this);
    m_companyInput->setPlaceholderText("e.g. Stripe, Linear, GitHub");

    auto *lblTitle = new QLabel("Job Title *", this);
    lblTitle->setObjectName("FieldLabel");
    m_titleInput = new QLineEdit(this);
    m_titleInput->setPlaceholderText("e.g. Senior Software Engineer");

    grid->addWidget(lblCompany, 0, 0);
    grid->addWidget(m_companyInput, 1, 0);
    grid->addWidget(lblTitle, 0, 1);
    grid->addWidget(m_titleInput, 1, 1);

    // Row 1: Status & Type
    auto *lblStatus = new QLabel("Pipeline Status", this);
    lblStatus->setObjectName("FieldLabel");
    m_statusCombo = new QComboBox(this);
    m_statusCombo->addItems(Config::STATUSES);

    auto *lblType = new QLabel("Job Type", this);
    lblType->setObjectName("FieldLabel");
    m_typeCombo = new QComboBox(this);
    m_typeCombo->addItems(Config::JOB_TYPES);

    grid->addWidget(lblStatus, 2, 0);
    grid->addWidget(m_statusCombo, 3, 0);
    grid->addWidget(lblType, 2, 1);
    grid->addWidget(m_typeCombo, 3, 1);

    // Row 2: Salary & Location
    auto *lblSalary = new QLabel("Salary / Compensation", this);
    lblSalary->setObjectName("FieldLabel");
    m_salaryInput = new QLineEdit(this);
    m_salaryInput->setPlaceholderText("e.g. $140k - $165k or $90/hr");

    auto *lblLocation = new QLabel("Location / Work Mode", this);
    lblLocation->setObjectName("FieldLabel");
    m_locationInput = new QLineEdit(this);
    m_locationInput->setPlaceholderText("e.g. Remote, Hybrid, San Francisco");

    grid->addWidget(lblSalary, 4, 0);
    grid->addWidget(m_salaryInput, 5, 0);
    grid->addWidget(lblLocation, 4, 1);
    grid->addWidget(m_locationInput, 5, 1);

    // Row 3: Date Applied & Job Link
    auto *lblDate = new QLabel("Date Applied", this);
    lblDate->setObjectName("FieldLabel");
    m_dateEdit = new QDateEdit(this);
    m_dateEdit->setCalendarPopup(true);
    m_dateEdit->setDisplayFormat("yyyy-MM-dd");
    m_dateEdit->setDate(QDate::currentDate());

    auto *lblLink = new QLabel("Job Posting URL", this);
    lblLink->setObjectName("FieldLabel");
    auto *urlLayout = new QHBoxLayout();
    urlLayout->setSpacing(6);
    m_linkInput = new QLineEdit(this);
    m_linkInput->setPlaceholderText("https://...");
    auto *testUrlBtn = new QPushButton("Open", this);
    testUrlBtn->setIcon(IconHelper::getIcon("external-link", "#475569", 13));
    testUrlBtn->setToolTip("Open URL in browser to test");
    connect(testUrlBtn, &QPushButton::clicked, this, &JobDialog::onTestUrl);
    urlLayout->addWidget(m_linkInput);
    urlLayout->addWidget(testUrlBtn);

    grid->addWidget(lblDate, 6, 0);
    grid->addWidget(m_dateEdit, 7, 0);
    grid->addWidget(lblLink, 6, 1);
    grid->addLayout(urlLayout, 7, 1);

    // Row 4: Dedicated CV & Application Documents Folder
    auto *folderHeaderLayout = new QHBoxLayout();
    folderHeaderLayout->setSpacing(6);
    auto *folderIconLabel = new QLabel(this);
    folderIconLabel->setFixedSize(14, 14);
    folderIconLabel->setScaledContents(true);
    folderIconLabel->setPixmap(IconHelper::getPixmap("folder", "#71717a", 14));
    folderHeaderLayout->addWidget(folderIconLabel);

    auto *lblFolder = new QLabel("Attached CV & Application Documents Folder", this);
    lblFolder->setObjectName("FieldLabel");
    folderHeaderLayout->addWidget(lblFolder);
    folderHeaderLayout->addStretch();

    auto *folderLayout = new QHBoxLayout();
    folderLayout->setSpacing(6);

    m_folderInput = new QLineEdit(this);
    m_folderInput->setPlaceholderText(QString("/home/eyobed/Documents/Work/Applications/<location>/%1 <company>...").arg(m_job.applicationNumber, 2, 10, QChar('0')));

    auto *browseFolderBtn = new QPushButton("Browse...", this);
    browseFolderBtn->setIcon(IconHelper::getIcon("folder", "#475569", 13));
    browseFolderBtn->setToolTip("Select folder on your computer");
    browseFolderBtn->setCursor(Qt::PointingHandCursor);
    connect(browseFolderBtn, &QPushButton::clicked, this, &JobDialog::onBrowseFolder);

    auto *openFolderBtn = new QPushButton("Open", this);
    openFolderBtn->setIcon(IconHelper::getIcon("folder-open", "#475569", 13));
    openFolderBtn->setToolTip("Open folder in file manager");
    openFolderBtn->setCursor(Qt::PointingHandCursor);
    connect(openFolderBtn, &QPushButton::clicked, this, &JobDialog::onOpenFolder);

    auto *createFolderBtn = new QPushButton("Create", this);
    createFolderBtn->setIcon(IconHelper::getIcon("folder-plus", "#475569", 13));
    createFolderBtn->setToolTip("Create folder if it does not exist");
    createFolderBtn->setCursor(Qt::PointingHandCursor);
    connect(createFolderBtn, &QPushButton::clicked, this, &JobDialog::onCreateFolder);

    folderLayout->addWidget(m_folderInput);
    folderLayout->addWidget(browseFolderBtn);
    folderLayout->addWidget(createFolderBtn);
    folderLayout->addWidget(openFolderBtn);

    grid->addLayout(folderHeaderLayout, 8, 0, 1, 2);
    grid->addLayout(folderLayout, 9, 0, 1, 2);

    mainLayout->addLayout(grid);

    // Notes
    auto *lblNotes = new QLabel("Notes, Recruiter Contacts & Interview Timeline", this);
    lblNotes->setObjectName("FieldLabel");
    mainLayout->addWidget(lblNotes);

    m_notesInput = new QTextEdit(this);
    m_notesInput->setPlaceholderText("Add interview feedback, recruiter contacts, technical details, follow-ups...");
    m_notesInput->setFixedHeight(120);
    mainLayout->addWidget(m_notesInput);

    // Buttons
    auto *btnLayout = new QHBoxLayout();
    btnLayout->setSpacing(10);
    btnLayout->addStretch();

    auto *cancelBtn = new QPushButton("Cancel", this);
    cancelBtn->setCursor(Qt::PointingHandCursor);
    connect(cancelBtn, &QPushButton::clicked, this, &QDialog::reject);

    auto *saveBtn = new QPushButton(m_isEdit ? "Save Changes" : "Add to Pipeline", this);
    saveBtn->setObjectName("PrimaryButton");
    saveBtn->setIcon(IconHelper::getIcon(m_isEdit ? "check" : "plus", "#ffffff", 13));
    saveBtn->setCursor(Qt::PointingHandCursor);
    connect(saveBtn, &QPushButton::clicked, this, &JobDialog::onSave);

    btnLayout->addWidget(cancelBtn);
    btnLayout->addWidget(saveBtn);

    mainLayout->addLayout(btnLayout);
}

void JobDialog::populateData() {
    if (m_isEdit) {
        m_companyInput->setText(m_job.company);
        m_titleInput->setText(m_job.title);
        m_statusCombo->setCurrentText(m_job.status);
        m_typeCombo->setCurrentText(m_job.jobType);
        m_salaryInput->setText(m_job.salary);
        m_locationInput->setText(m_job.location);
        m_linkInput->setText(m_job.link);
        m_folderInput->setText(m_job.folderPath);
        m_notesInput->setPlainText(m_job.notes);

        if (!m_job.dateApplied.isEmpty()) {
            QDate d = QDate::fromString(m_job.dateApplied, "yyyy-MM-dd");
            if (d.isValid()) {
                m_dateEdit->setDate(d);
            }
        }
    } else {
        m_statusCombo->setCurrentText(m_defaultStatus);
    }
}

void JobDialog::onBrowseFolder() {
    QString current = m_folderInput->text().trimmed();
    QString startDir = current;
    if (startDir.isEmpty()) {
        QString location = m_locationInput ? m_locationInput->text().trimmed() : QString();
        QString baseDir = "/home/eyobed/Documents/Work/Applications";
        if (!location.isEmpty() && location.compare("Remote", Qt::CaseInsensitive) != 0) {
            QString locDir = QString("%1/%2").arg(baseDir, location);
            if (QDir(locDir).exists()) {
                startDir = locDir;
            } else if (QDir(baseDir).exists()) {
                startDir = baseDir;
            } else {
                startDir = QDir::homePath();
            }
        } else if (QDir(baseDir).exists()) {
            startDir = baseDir;
        } else {
            startDir = QDir::homePath();
        }
    } else if (startDir.contains("/Documents/Work/CVs/")) {
        startDir.replace("/Documents/Work/CVs/", "/Documents/Work/Applications/");
    }

    QString dir = NativeDialog::getExistingDirectory(this, "Select CV & Documents Folder", startDir);
    if (!dir.isEmpty()) {
        m_folderInput->setText(dir);
    }
}

void JobDialog::onOpenFolder() {
    QString path = m_folderInput->text().trimmed();
    if (path.isEmpty()) {
        QMessageBox::information(this, "No Folder", "Please specify or browse for a folder location first.");
        return;
    }

    if (path.contains("/Documents/Work/CVs/")) {
        path.replace("/Documents/Work/CVs/", "/Documents/Work/Applications/");
        m_folderInput->setText(path);
    }

    QDir dir(path);
    if (!dir.exists()) {
        auto res = QMessageBox::question(
            this,
            "Folder Does Not Exist",
            QString("The folder '%1' does not exist yet. Would you like to create it now?").arg(path),
            QMessageBox::Yes | QMessageBox::No
        );
        if (res == QMessageBox::Yes) {
            if (dir.mkpath(".")) {
                QDesktopServices::openUrl(QUrl::fromLocalFile(path));
            } else {
                QMessageBox::critical(this, "Error", "Failed to create folder:\n" + path);
            }
        }
        return;
    }

    QDesktopServices::openUrl(QUrl::fromLocalFile(path));
}

void JobDialog::onCreateFolder() {
    QString path = m_folderInput->text().trimmed();
    if (path.isEmpty()) {
        QString company = m_companyInput->text().trimmed();
        if (company.isEmpty()) {
            company = "NewApplication";
        }
        QString location = m_locationInput ? m_locationInput->text().trimmed() : QString();
        QString baseDir = "/home/eyobed/Documents/Work/Applications";
        int num = m_job.applicationNumber > 0 ? m_job.applicationNumber : 1;
        QString numPrefix = QString("%1").arg(num, 2, 10, QChar('0'));
        QString folderName = QString("%1 %2").arg(numPrefix, company).trimmed();
        if (!location.isEmpty() && location.compare("Remote", Qt::CaseInsensitive) != 0) {
            path = QString("%1/%2/%3").arg(baseDir, location, folderName).trimmed();
        } else {
            path = QString("%1/%2").arg(baseDir, folderName).trimmed();
        }
        m_folderInput->setText(path);
    } else if (path.contains("/Documents/Work/CVs/")) {
        path.replace("/Documents/Work/CVs/", "/Documents/Work/Applications/");
        m_folderInput->setText(path.trimmed());
    }

    QDir dir(path);
    if (dir.exists()) {
        QMessageBox::information(this, "Folder Exists", QString("Folder already exists:\n%1").arg(path));
        return;
    }

    if (dir.mkpath(".")) {
        QMessageBox::information(this, "Folder Created", QString("Successfully created folder:\n%1").arg(path));
    } else {
        QMessageBox::critical(this, "Error", "Failed to create folder:\n" + path);
    }
}

void JobDialog::onTestUrl() {
    QString url = m_linkInput->text().trimmed();
    if (url.isEmpty()) return;

    if (!url.startsWith("http://") && !url.startsWith("https://")) {
        url = "https://" + url;
    }
    QDesktopServices::openUrl(QUrl(url));
}

void JobDialog::onSave() {
    QString company = m_companyInput->text().trimmed();
    QString title = m_titleInput->text().trimmed();

    if (company.isEmpty()) {
        QMessageBox::warning(this, "Validation Error", "Please provide a Company Name.");
        m_companyInput->setFocus();
        return;
    }

    if (title.isEmpty()) {
        QMessageBox::warning(this, "Validation Error", "Please provide a Job Title.");
        m_titleInput->setFocus();
        return;
    }

    m_job.company = company;
    m_job.title = title;
    m_job.status = m_statusCombo->currentText();
    m_job.jobType = m_typeCombo->currentText();
    m_job.salary = m_salaryInput->text().trimmed();
    m_job.location = m_locationInput->text().trimmed();
    m_job.dateApplied = m_dateEdit->date().toString("yyyy-MM-dd");
    m_job.link = m_linkInput->text().trimmed();
    m_job.folderPath = m_folderInput->text().trimmed();
    m_job.notes = m_notesInput->toPlainText();
    m_job.updatedAt = QDateTime::currentDateTime().toString(Qt::ISODate);

    accept();
}

JobApplication JobDialog::getJob() const {
    return m_job;
}
