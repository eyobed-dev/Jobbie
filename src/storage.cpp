#include "storage.h"
#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QJsonDocument>
#include <QJsonArray>
#include <QDebug>

StorageManager::StorageManager(const QString &filePath)
    : m_filePath(filePath) {}

QList<JobApplication> StorageManager::getSeedJobs() const {
    QList<JobApplication> list;

    JobApplication j1;
    j1.id = "1";
    j1.company = "Linear";
    j1.title = "Senior Systems Engineer";
    j1.status = "Interviewing";
    j1.jobType = "Full-time";
    j1.salary = "$160k - $185k";
    j1.dateApplied = QDate::currentDate().toString("yyyy-MM-dd");
    j1.link = "https://linear.app/careers";
    j1.folderPath = "/home/eyobed/Documents/Work/CVs/Linear";
    j1.notes = "Spoke with hiring manager. System architecture round scheduled for next Tuesday.";
    j1.location = "Remote";
    list.append(j1);

    JobApplication j2;
    j2.id = "2";
    j2.company = "Vercel";
    j2.title = "Core Runtime Engineer";
    j2.status = "Applied";
    j2.jobType = "Full-time";
    j2.salary = "$170k - $200k";
    j2.dateApplied = QDate::currentDate().toString("yyyy-MM-dd");
    j2.link = "https://vercel.com/careers";
    j2.notes = "Applied via referral link.";
    j2.location = "Remote";
    list.append(j2);

    JobApplication j3;
    j3.id = "3";
    j3.company = "Stripe";
    j3.title = "Infrastructure Engineer";
    j3.status = "Offered";
    j3.jobType = "Full-time";
    j3.salary = "$190k + Equity";
    j3.dateApplied = QDate::currentDate().toString("yyyy-MM-dd");
    j3.link = "https://stripe.com/jobs";
    j3.notes = "Offer letter received! Reviewing benefits and equity package.";
    j3.location = "Hybrid";
    list.append(j3);

    JobApplication j4;
    j4.id = "4";
    j4.company = "Supabase";
    j4.title = "Backend / Database Dev";
    j4.status = "Wishlist";
    j4.jobType = "Contract";
    j4.salary = "$110/hr";
    j4.link = "https://supabase.com/careers";
    j4.notes = "Need to polish PostgreSQL & Rust portfolio before submitting.";
    j4.location = "Remote";
    list.append(j4);

    JobApplication j5;
    j5.id = "5";
    j5.company = "Datadog";
    j5.title = "APM Platform Engineer";
    j5.status = "Rejected";
    j5.jobType = "Full-time";
    j5.salary = "$150k";
    j5.dateApplied = "2024-01-10";
    j5.link = "https://www.datadoghq.com";
    j5.notes = "Role filled internally, keep in touch for Q3 pipeline.";
    j5.location = "On-site";
    list.append(j5);

    return list;
}

QList<JobApplication> StorageManager::loadJobs() {
    QFile file(m_filePath);
    if (!file.exists()) {
        QList<JobApplication> seed = getSeedJobs();
        saveJobs(seed);
        return seed;
    }

    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Could not open file for reading:" << m_filePath;
        return getSeedJobs();
    }

    QByteArray data = file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(data);
    if (!doc.isArray()) {
        qWarning() << "Invalid JSON format in" << m_filePath;
        return getSeedJobs();
    }

    QList<JobApplication> jobs;
    QJsonArray array = doc.array();
    for (const auto &val : array) {
        if (val.isObject()) {
            jobs.append(JobApplication::fromJson(val.toObject()));
        }
    }
    return jobs;
}

bool StorageManager::saveJobs(const QList<JobApplication> &jobs) {
    QFileInfo fileInfo(m_filePath);
    QDir dir = fileInfo.dir();
    if (!dir.exists()) {
        dir.mkpath(".");
    }

    QJsonArray array;
    for (const auto &job : jobs) {
        array.append(job.toJson());
    }

    QJsonDocument doc(array);
    QFile file(m_filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        qWarning() << "Could not open file for writing:" << m_filePath;
        return false;
    }

    file.write(doc.toJson(QJsonDocument::Indented));
    file.close();
    return true;
}

bool StorageManager::exportToFile(const QString &exportPath, const QList<JobApplication> &jobs) {
    QJsonArray array;
    for (const auto &job : jobs) {
        array.append(job.toJson());
    }

    QJsonDocument doc(array);
    QFile file(exportPath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        return false;
    }

    file.write(doc.toJson(QJsonDocument::Indented));
    file.close();
    return true;
}

bool StorageManager::importFromFile(const QString &importPath, QList<JobApplication> &outJobs) {
    QFile file(importPath);
    if (!file.open(QIODevice::ReadOnly)) {
        return false;
    }

    QByteArray data = file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(data);
    if (!doc.isArray()) {
        return false;
    }

    outJobs.clear();
    QJsonArray array = doc.array();
    for (const auto &val : array) {
        if (val.isObject()) {
            outJobs.append(JobApplication::fromJson(val.toObject()));
        }
    }
    return true;
}
