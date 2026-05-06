#pragma once

#include <QString>
#include <QList>
#include "models.h"

class StorageManager {
public:
    explicit StorageManager(const QString &filePath = "jobs.json");

    QList<JobApplication> loadJobs();
    bool saveJobs(const QList<JobApplication> &jobs);
    bool exportToFile(const QString &exportPath, const QList<JobApplication> &jobs);
    bool importFromFile(const QString &importPath, QList<JobApplication> &outJobs);

private:
    QString m_filePath;
    QList<JobApplication> getSeedJobs() const;
};
