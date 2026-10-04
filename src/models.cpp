#include "models.h"

QJsonObject JobApplication::toJson() const {
    QJsonObject obj;
    obj["id"] = id;
    obj["application_number"] = applicationNumber;
    obj["company"] = company;
    obj["title"] = title;
    obj["status"] = status;
    obj["job_type"] = jobType;
    obj["salary"] = salary;
    obj["date_applied"] = dateApplied;
    obj["link"] = link;
    QString normalizedPath = folderPath;
    if (normalizedPath.contains("/Documents/Work/CVs/")) {
        normalizedPath.replace("/Documents/Work/CVs/", "/Documents/Work/Applications/");
    }
    obj["folder_path"] = normalizedPath;
    obj["notes"] = notes;
    obj["location"] = location;
    obj["created_at"] = createdAt;
    obj["updated_at"] = updatedAt;
    return obj;
}

JobApplication JobApplication::fromJson(const QJsonObject &obj) {
    JobApplication job;
    job.id = obj.value("id").toString(QUuid::createUuid().toString(QUuid::WithoutBraces));
    job.company = obj.value("company").toString().trimmed();
    job.title = obj.value("title").toString().trimmed();
    job.status = obj.value("status").toString("Wishlist");

    if (obj.contains("job_type")) {
        job.jobType = obj.value("job_type").toString("Full-time");
    } else if (obj.contains("type")) {
        job.jobType = obj.value("type").toString("Full-time");
    }

    job.salary = obj.value("salary").toString();
    if (obj.contains("application_number")) {
        job.applicationNumber = obj.value("application_number").toInt();
    } else if (obj.contains("applicationNumber")) {
        job.applicationNumber = obj.value("applicationNumber").toInt();
    }

    if (obj.contains("date_applied")) {
        job.dateApplied = obj.value("date_applied").toString();
    } else if (obj.contains("dateApplied")) {
        job.dateApplied = obj.value("dateApplied").toString();
    }

    job.link = obj.value("link").toString();

    // Folder path for CV & application docs
    if (obj.contains("folder_path")) {
        job.folderPath = obj.value("folder_path").toString();
    } else if (obj.contains("folderPath")) {
        job.folderPath = obj.value("folderPath").toString();
    }
    if (job.folderPath.contains("/Documents/Work/CVs/")) {
        job.folderPath.replace("/Documents/Work/CVs/", "/Documents/Work/Applications/");
    }

    job.notes = obj.value("notes").toString();
    job.location = obj.value("location").toString("Remote");
    job.createdAt = obj.value("created_at").toString(QDateTime::currentDateTime().toString(Qt::ISODate));
    job.updatedAt = obj.value("updated_at").toString(QDateTime::currentDateTime().toString(Qt::ISODate));

    if (job.applicationNumber <= 0 && !job.folderPath.isEmpty()) {
        QString folderName = job.folderPath.section('/', -1).trimmed();
        int idx = 0;
        while (idx < folderName.length() && folderName[idx].isDigit()) {
            idx++;
        }
        if (idx > 0) {
            job.applicationNumber = folderName.left(idx).toInt();
        }
    }

    return job;
}
