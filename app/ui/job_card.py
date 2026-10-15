from app.icon_helper import get_icon, get_pixmap
"""Draggable Job Card Widget."""

from PyQt6.QtWidgets import (
    QFrame, QVBoxLayout, QHBoxLayout, QLabel, QPushButton, QWidget, QApplication
)
from PyQt6.QtCore import Qt, QMimeData, QPoint, pyqtSignal, QUrl
from PyQt6.QtGui import QDrag, QPixmap, QPainter, QCursor, QDesktopServices

from app.models import JobApplication
from app.config import TYPE_COLORS, STATUS_META


class JobCardWidget(QFrame):
    edit_requested = pyqtSignal(str)
    delete_requested = pyqtSignal(str)

    def __init__(self, job: JobApplication, dark_mode: bool = False, parent=None):
        super().__init__(parent)
        self.job = job
        self.dark_mode = dark_mode
        self.drag_start_pos = QPoint()

        self.setObjectName("JobCard")
        self.setCursor(Qt.CursorShape.PointingHandCursor)
        self.setMouseTracking(True)
        self.setToolTip("Click to view & edit details • Drag to move stages")
        self.init_ui()

    def init_ui(self):
        layout = QVBoxLayout(self)
        layout.setContentsMargins(12, 12, 12, 12)
        layout.setSpacing(8)

        # 1. Top row: Type badge & Action buttons
        top_row = QHBoxLayout()
        top_row.setContentsMargins(0, 0, 0, 0)
        top_row.setSpacing(6)

        # Type pill
        type_style = TYPE_COLORS.get(self.job.job_type, TYPE_COLORS["Full-time"])
        bg_color = type_style["bg_dark"] if self.dark_mode else type_style["bg_light"]
        text_color = type_style["text_dark"] if self.dark_mode else type_style["text_light"]
        border_color = type_style["border_dark"] if self.dark_mode else type_style["border_light"]

        if getattr(self.job, "application_number", 0) > 0:
            num_badge = QLabel(f"#{self.job.application_number:02d}")
            num_badge.setObjectName("AppNumBadge")
            nb_bg = "#27272a" if self.dark_mode else "#f4f4f5"
            nb_text = "#d4d4d8" if self.dark_mode else "#3f3f46"
            nb_border = "#3f3f46" if self.dark_mode else "#e4e4e7"
            num_badge.setStyleSheet(
                f"background-color: {nb_bg}; color: {nb_text}; border: 1px solid {nb_border}; "
                f"border-radius: 4px; padding: 2px 6px; font-size: 10px; font-weight: 700; font-family: monospace;"
            )
            num_badge.setAttribute(Qt.WidgetAttribute.WA_TransparentForMouseEvents, True)
            num_badge.setToolTip(f"Application entry #{self.job.application_number}")
            top_row.addWidget(num_badge)

        type_label = QLabel(self.job.job_type.upper())
        type_label.setObjectName("TypeBadge")
        type_label.setStyleSheet(
            f"background-color: {bg_color}; color: {text_color}; border: 1px solid {border_color}; "
            f"border-radius: 4px; padding: 2px 7px; font-size: 10px; font-weight: 600;"
        )
        type_label.setAttribute(Qt.WidgetAttribute.WA_TransparentForMouseEvents, True)
        top_row.addWidget(type_label)
        top_row.addStretch()

        # Folder button (if folder_path is configured)
        if hasattr(self.job, 'folder_path') and self.job.folder_path and self.job.folder_path.strip():
            folder_btn = QPushButton()
            folder_btn.setObjectName("folderBtn")
            folder_btn.setIcon(get_icon("folder", "#818cf8" if self.dark_mode else "#6366f1", 13))
            folder_btn.setProperty("class", "card-action-btn")
            folder_btn.setToolTip(f"Open CV & Docs folder:\n{self.job.folder_path}")
            folder_btn.setCursor(Qt.CursorShape.PointingHandCursor)
            folder_btn.clicked.connect(self._open_folder)
            top_row.addWidget(folder_btn)

        # Link button (if job has link)
        if self.job.link:
            link_btn = QPushButton()
            link_btn.setObjectName("linkBtn")
            link_btn.setIcon(get_icon("external-link", "#38bdf8" if self.dark_mode else "#0284c7", 13))
            link_btn.setProperty("class", "card-action-btn")
            link_btn.setToolTip(f"Open link: {self.job.link}")
            link_btn.setCursor(Qt.CursorShape.PointingHandCursor)
            link_btn.clicked.connect(self._open_link)
            top_row.addWidget(link_btn)

        # Edit button
        edit_btn = QPushButton()
        edit_btn.setObjectName("editBtn")
        edit_btn.setIcon(get_icon("pencil", "#a1a1aa" if self.dark_mode else "#71717a", 13))
        edit_btn.setProperty("class", "card-action-btn")
        edit_btn.setToolTip("Edit Application")
        edit_btn.setCursor(Qt.CursorShape.PointingHandCursor)
        edit_btn.clicked.connect(lambda: self.edit_requested.emit(self.job.id))
        top_row.addWidget(edit_btn)

        # Delete button
        del_btn = QPushButton()
        del_btn.setObjectName("deleteBtn")
        del_btn.setIcon(get_icon("trash-2", "#a1a1aa" if self.dark_mode else "#71717a", 13))
        del_btn.setProperty("class", "card-action-btn")
        del_btn.setToolTip("Delete Application")
        del_btn.setCursor(Qt.CursorShape.PointingHandCursor)
        del_btn.clicked.connect(lambda: self.delete_requested.emit(self.job.id))
        top_row.addWidget(del_btn)

        layout.addLayout(top_row)

        # 2. Title & Company
        title_label = QLabel(self.job.title)
        title_label.setObjectName("CardTitle")
        title_label.setWordWrap(True)
        title_label.setAttribute(Qt.WidgetAttribute.WA_TransparentForMouseEvents, True)
        layout.addWidget(title_label)

        company_layout = QHBoxLayout()
        company_layout.setContentsMargins(0, 0, 0, 0)
        company_layout.setSpacing(4)

        building_icon = QLabel()
        building_icon.setFixedSize(13, 13)
        building_icon.setScaledContents(True)
        building_icon.setPixmap(get_pixmap("building-2", "#a1a1aa" if self.dark_mode else "#71717a", 13))
        building_icon.setAttribute(Qt.WidgetAttribute.WA_TransparentForMouseEvents, True)
        company_layout.addWidget(building_icon)

        company_label = QLabel(self.job.company)
        company_label.setObjectName("CardCompany")
        company_label.setAttribute(Qt.WidgetAttribute.WA_TransparentForMouseEvents, True)
        company_layout.addWidget(company_label)

        if self.job.location:
            loc_label = QLabel(f"• {self.job.location}")
            loc_label.setObjectName("CardCompany")
            company_layout.addWidget(loc_label)

        company_layout.addStretch()
        layout.addLayout(company_layout)

        # 3. Notes snippet if present
        if self.job.notes and self.job.notes.strip():
            notes_snippet = self.job.notes.strip().replace("\n", " ")
            if len(notes_snippet) > 80:
                notes_snippet = notes_snippet[:77] + "..."
            notes_container = QFrame()
            notes_container.setObjectName("CardNotes")
            notes_inner = QHBoxLayout(notes_container)
            notes_inner.setContentsMargins(6, 4, 8, 4)
            notes_inner.setSpacing(6)

            notes_icon = QLabel(notes_container)
            notes_icon.setFixedSize(12, 12)
            notes_icon.setScaledContents(True)
            notes_icon.setPixmap(get_pixmap("message-square", "#fbbf24" if self.dark_mode else "#d97706", 12))
            notes_inner.addWidget(notes_icon, 0, Qt.AlignmentFlag.AlignTop)

            notes_label = QLabel(notes_snippet, notes_container)
            notes_label.setObjectName("CardNotesText")
            notes_label.setWordWrap(True)
            notes_inner.addWidget(notes_label, 1)

            notes_container.setAttribute(Qt.WidgetAttribute.WA_TransparentForMouseEvents, True)
            layout.addWidget(notes_container)

        # 4. Bottom footer: Salary & Applied Date
        bottom_row = QHBoxLayout()
        bottom_row.setContentsMargins(0, 4, 0, 0)
        bottom_row.setSpacing(8)

        if self.job.salary:
            salary_label = QLabel(self.job.salary)
            salary_label.setObjectName("SalaryBadge")
            salary_label.setAttribute(Qt.WidgetAttribute.WA_TransparentForMouseEvents, True)
            bottom_row.addWidget(salary_label)

        bottom_row.addStretch()

        if self.job.date_applied:
            date_layout = QHBoxLayout()
            date_layout.setContentsMargins(0, 0, 0, 0)
            date_layout.setSpacing(4)

            cal_icon = QLabel()
            cal_icon.setFixedSize(12, 12)
            cal_icon.setScaledContents(True)
            cal_icon.setPixmap(get_pixmap("calendar", "#a1a1aa" if self.dark_mode else "#71717a", 12))
            date_layout.addWidget(cal_icon)

            date_label = QLabel(self.job.date_applied)
            date_label.setObjectName("DateBadge")
            date_layout.addWidget(date_label)

            cal_icon.setAttribute(Qt.WidgetAttribute.WA_TransparentForMouseEvents, True)
            date_label.setAttribute(Qt.WidgetAttribute.WA_TransparentForMouseEvents, True)
            bottom_row.addLayout(date_layout)

        layout.addLayout(bottom_row)

    def _open_link(self):
        if self.job.link:
            url = self.job.link.strip()
            if not url.startswith("http://") and not url.startswith("https://"):
                url = "https://" + url
            QDesktopServices.openUrl(QUrl(url))

    def _open_folder(self):
        if hasattr(self.job, 'folder_path') and self.job.folder_path:
            import os
            from PyQt6.QtWidgets import QMessageBox
            path = self.job.folder_path.strip()
            if "/Documents/Work/CVs/" in path:
                path = path.replace("/Documents/Work/CVs/", "/Documents/Work/Applications/")
                self.job.folder_path = path
            if not os.path.exists(path):
                res = QMessageBox.question(
                    self,
                    "Folder Not Found",
                    f"The folder for this application does not exist yet:\n{path}\n\nWould you like to create it now?",
                    QMessageBox.StandardButton.Yes | QMessageBox.StandardButton.No
                )
                if res == QMessageBox.StandardButton.Yes:
                    os.makedirs(path, exist_ok=True)
                else:
                    return
            QDesktopServices.openUrl(QUrl.fromLocalFile(path))

    def mousePressEvent(self, event):
        if event.button() == Qt.MouseButton.LeftButton:
            self.drag_start_pos = event.pos()
        super().mousePressEvent(event)

    def mouseMoveEvent(self, event):
        if not (event.buttons() & Qt.MouseButton.LeftButton):
            return

        if (event.pos() - self.drag_start_pos).manhattanLength() < QApplication.startDragDistance():
            return

        drag = QDrag(self)
        mime_data = QMimeData()
        mime_data.setData("application/x-jobbie-job-id", self.job.id.encode("utf-8"))
        mime_data.setText(self.job.id)
        drag.setMimeData(mime_data)

        # Render visual pixmap for the drag operation
        pixmap = QPixmap(self.size())
        pixmap.fill(Qt.GlobalColor.transparent)
        painter = QPainter(pixmap)
        painter.setOpacity(0.85)
        self.render(painter)
        painter.end()

        drag.setPixmap(pixmap)
        drag.setHotSpot(event.pos())

        self.setProperty("dragging", "true")
        self.style().unpolish(self)
        self.style().polish(self)

        drag.exec(Qt.DropAction.MoveAction)

        self.setProperty("dragging", "false")
        self.style().unpolish(self)
        self.style().polish(self)

    def mouseReleaseEvent(self, event):
        if event.button() == Qt.MouseButton.LeftButton:
            if (event.pos() - self.drag_start_pos).manhattanLength() < QApplication.startDragDistance():
                self.edit_requested.emit(self.job.id)
        super().mouseReleaseEvent(event)
