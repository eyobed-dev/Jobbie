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
    return {};
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
