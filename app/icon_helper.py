from pathlib import Path
from PyQt6.QtGui import QIcon, QPixmap
from PyQt6.QtCore import Qt

ICONS_DIR = Path(__file__).resolve().parent.parent / "resources" / "icons"

_icon_cache = {}
_pixmap_cache = {}

def resolve_name(name: str) -> str:
    if name == "edit": return "pencil"
    if name == "trash": return "trash-2"
    if name == "building": return "building-2"
    if name == "x-circle": return "circle-x"
    if name == "alert-triangle": return "triangle-alert"
    return name

def get_pixmap(name: str, color: str = "#000000", size: int = 16) -> QPixmap:
    name = resolve_name(name)
    key = (name, color, size)
    if key in _pixmap_cache:
        return _pixmap_cache[key]

    path = ICONS_DIR / f"{name}.svg"
    if not path.exists():
        return QPixmap()

    svg_data = path.read_text(encoding="utf-8").replace("currentColor", color)
    pix = QPixmap()
    pix.loadFromData(svg_data.encode("utf-8"), "SVG")
    if not pix.isNull():
        pix = pix.scaled(size, size, Qt.AspectRatioMode.KeepAspectRatio, Qt.TransformationMode.SmoothTransformation)
    _pixmap_cache[key] = pix
    return pix

def get_icon(name: str, color: str = "#000000", size: int = 16) -> QIcon:
    name = resolve_name(name)
    key = (name, color, size)
    if key in _icon_cache:
        return _icon_cache[key]
    pix = get_pixmap(name, color, size)
    icon = QIcon(pix)
    _icon_cache[key] = icon
    return icon
