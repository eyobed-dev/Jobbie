#pragma once

#include <QDialog>
#include <QLineEdit>
#include <QComboBox>
#include <QDateEdit>
#include <QTextEdit>
#include "models.h"

class JobDialog : public QDialog {
    Q_OBJECT

public:
    explicit JobDialog(const JobApplication *job = nullptr, const QString &defaultStatus = "Wishlist", int nextAppNumber = 1, QWidget *parent = nullptr);

    JobApplication getJob() const;

private slots:
    void onBrowseFolder();
    void onOpenFolder();
    void onCreateFolder();
    void onTestUrl();
    void onSave();

private:
    void initUI();
    void populateData();

    bool m_isEdit;
    JobApplication m_job;
    QString m_defaultStatus;

    QLineEdit *m_companyInput = nullptr;
    QLineEdit *m_titleInput = nullptr;
    QComboBox *m_statusCombo = nullptr;
    QComboBox *m_typeCombo = nullptr;
    QLineEdit *m_salaryInput = nullptr;
    QLineEdit *m_locationInput = nullptr;
    QDateEdit *m_dateEdit = nullptr;
    QLineEdit *m_linkInput = nullptr;
    QLineEdit *m_folderInput = nullptr;
    QTextEdit *m_notesInput = nullptr;
};
