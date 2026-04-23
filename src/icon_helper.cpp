#include "icon_helper.h"

#include <QSvgRenderer>
#include <QPainter>
#include <QMap>
#include <QFile>
#include <QGuiApplication>

namespace IconHelper {

static QMap<QString, QPixmap> s_pixmapCache;
static QMap<QString, QIcon> s_iconCache;

static QString resolveIconName(const QString &name) {
    if (name == "edit") return "pencil";
    if (name == "trash") return "trash-2";
    if (name == "building") return "building-2";
    if (name == "x-circle") return "circle-x";
    if (name == "alert-triangle") return "triangle-alert";
    return name;
}

QPixmap getPixmap(const QString &name, const QString &strokeColor, int size, qreal dpr) {
    QString resolved = resolveIconName(name);

    if (dpr <= 0.0) {
        dpr = qApp ? qApp->devicePixelRatio() : 1.0;
        if (dpr <= 0.0) dpr = 1.0;
    }

    QString cacheKey = QString("%1_%2_%3_%4").arg(resolved, strokeColor).arg(size).arg(dpr);
    if (s_pixmapCache.contains(cacheKey)) {
        return s_pixmapCache.value(cacheKey);
    }

    QString resPath = QString(":/icons/%1.svg").arg(resolved);
    QFile file(resPath);
    if (!file.open(QIODevice::ReadOnly)) {
        return QPixmap();
    }

    QString svgContent = QString::fromUtf8(file.readAll());
    file.close();

    svgContent.replace("currentColor", strokeColor);

    int pixelSize = static_cast<int>(size * dpr);
    QPixmap pixmap(pixelSize, pixelSize);
    pixmap.fill(Qt::transparent);

    QPainter painter(&pixmap);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setRenderHint(QPainter::SmoothPixmapTransform, true);
    QSvgRenderer renderer(svgContent.toUtf8());
    renderer.render(&painter);
    painter.end();

    pixmap.setDevicePixelRatio(dpr);
    s_pixmapCache.insert(cacheKey, pixmap);
    return pixmap;
}

QIcon getIcon(const QString &name, const QString &strokeColor, int size) {
    QString resolved = resolveIconName(name);
    QString cacheKey = QString("%1_%2_%3").arg(resolved, strokeColor).arg(size);
    if (s_iconCache.contains(cacheKey)) {
        return s_iconCache.value(cacheKey);
    }

    QIcon icon;
    icon.addPixmap(getPixmap(resolved, strokeColor, size, 1.0));
    icon.addPixmap(getPixmap(resolved, strokeColor, size, 2.0));
    s_iconCache.insert(cacheKey, icon);
    return icon;
}

QIcon getThemedIcon(const QString &name, bool darkMode, int size) {
    QString color = darkMode ? "#cbd5e1" : "#475569";
    return getIcon(name, color, size);
}

} // namespace IconHelper
