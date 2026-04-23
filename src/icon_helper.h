#pragma once

#include <QIcon>
#include <QPixmap>
#include <QString>

namespace IconHelper {

// Returns a vector-crisp QIcon for a named icon tinted with strokeColor (size in px)
QIcon getIcon(const QString &name, const QString &strokeColor = "#000000", int size = 16);

// Returns a high-DPI crisp QPixmap for a named icon tinted with strokeColor
QPixmap getPixmap(const QString &name, const QString &strokeColor = "#000000", int size = 16, qreal dpr = 1.0);

// Helper for theme-appropriate icon
QIcon getThemedIcon(const QString &name, bool darkMode, int size = 16);

} // namespace IconHelper
