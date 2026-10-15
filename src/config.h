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
        {"Wishlist",     {"target",      "Wishlist",     "#6366f1", "#818cf8"}},
        {"Applied",      {"send",        "Applied",      "#0284c7", "#38bdf8"}},
        {"Interviewing", {"briefcase",   "Interviewing", "#d97706", "#fbbf24"}},
        {"Offered",      {"award",       "Offered",      "#059669", "#34d399"}},
        {"Rejected",     {"circle-x",    "Rejected",     "#e11d48", "#fb7185"}}
    };
}

struct TypeStyle {
    QString bgLight;
    QString textLight;
    QString borderLight;
    QString bgDark;
    QString textDark;
    QString borderDark;
};

inline QMap<QString, TypeStyle> getTypeStyles() {
    return {
        {"Full-time",  {"#eff6ff", "#1d4ed8", "#bfdbfe", "#172554", "#93c5fd", "#1e40af"}},
        {"Part-time",  {"#f5f3ff", "#6d28d9", "#ddd6fe", "#2e1065", "#c4b5fd", "#5b21b6"}},
        {"Contract",   {"#fffbeb", "#b45309", "#fde68a", "#451a03", "#fcd34d", "#92400e"}},
        {"Freelance",  {"#f0fdfa", "#0f766e", "#99f6e4", "#042f2e", "#5eead4", "#115e59"}},
        {"Internship", {"#fdf4ff", "#a21caf", "#f5d0fe", "#4a044e", "#f0abfc", "#86198f"}}
    };
}

} // namespace Config
