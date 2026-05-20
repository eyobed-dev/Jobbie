#pragma once

#include <QString>
#include <QStringList>
#include <QMap>

namespace Config {

inline const QStringList STATUSES = {
    "Wishlist", "Applied", "Interviewing", "Offered", "Rejected"
};

inline const QStringList JOB_TYPES = {
    "Full-time", "Part-time", "Contract", "Freelance", "Internship"
};

struct StatusMeta {
    QString icon;
    QString label;
    QString lightColor;
    QString darkColor;
};

inline QMap<QString, StatusMeta> getStatusMeta() {
    return {
        {"Wishlist",     {"🎯", "Wishlist",     "#6366f1", "#818cf8"}},
        {"Applied",      {"⏱️", "Applied",      "#0284c7", "#38bdf8"}},
        {"Interviewing", {"💼", "Interviewing", "#d97706", "#fbbf24"}},
        {"Offered",      {"🎉", "Offered",      "#059669", "#34d399"}},
        {"Rejected",     {"❌", "Rejected",     "#e11d48", "#fb7185"}}
    };
}

struct TypeStyle {
    QString bgLight;
    QString textLight;
    QString bgDark;
    QString textDark;
};

inline QMap<QString, TypeStyle> getTypeStyles() {
    return {
        {"Full-time",  {"#e0e7ff", "#4338ca", "#312e81", "#c7d2fe"}},
        {"Part-time",  {"#f3e8ff", "#7e22ce", "#581c87", "#e9d5ff"}},
        {"Contract",   {"#ffedd5", "#c2410c", "#7c2d12", "#fed7aa"}},
        {"Freelance",  {"#fce7f3", "#be185d", "#831843", "#fbcfe8"}},
        {"Internship", {"#ccfbf1", "#0f766e", "#134e4a", "#99f6e4"}}
    };
}

} // namespace Config
