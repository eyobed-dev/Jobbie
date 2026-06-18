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

        type_label = QLabel(self.job.job_type.upper())
        type_label.setObjectName("TypeBadge")
        type_label.setStyleSheet(
            f"background-color: {bg_color}; color: {text_color}; "
            f"border-radius: 4px; padding: 2px 7px; font-size: 10px; font-weight: 700;"
        )
        top_row.addWidget(type_label)
        top_row.addStretch()

        # Folder button (if folder_path is configured)
        if hasattr(self.job, 'folder_path') and self.job.folder_path and self.job.folder_path.strip():
            folder_btn = QPushButton("📁")
            folder_btn.setObjectName("folderBtn")
            folder_btn.setProperty("class", "card-action-btn")
            folder_btn.setToolTip(f"Open CV & Docs folder:\n{self.job.folder_path}")
            folder_btn.setCursor(Qt.CursorShape.PointingHandCursor)
            folder_btn.clicked.connect(self._open_folder)
            top_row.addWidget(folder_btn)

        # Link button (if job has link)
        if self.job.link:
            link_btn = QPushButton("🔗")
            link_btn.setObjectName("linkBtn")
            link_btn.setProperty("class", "card-action-btn")
            link_btn.setToolTip(f"Open link: {self.job.link}")
            link_btn.setCursor(Qt.CursorShape.PointingHandCursor)
            link_btn.clicked.connect(self._open_link)
            top_row.addWidget(link_btn)

        # Edit button
        edit_btn = QPushButton("✏️")
        edit_btn.setObjectName("editBtn")
        edit_btn.setProperty("class", "card-action-btn")
        edit_btn.setToolTip("Edit Application")
        edit_btn.setCursor(Qt.CursorShape.PointingHandCursor)
        edit_btn.clicked.connect(lambda: self.edit_requested.emit(self.job.id))
        top_row.addWidget(edit_btn)

        # Delete button
        del_btn = QPushButton("🗑️")
        del_btn.setObjectName("deleteBtn")
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
        layout.addWidget(title_label)

        company_layout = QHBoxLayout()
        company_layout.setContentsMargins(0, 0, 0, 0)
        company_layout.setSpacing(4)

        company_label = QLabel(f"🏢  {self.job.company}")
        company_label.setObjectName("CardCompany")
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
            notes_label = QLabel(f"💬 {notes_snippet}")
            notes_label.setStyleSheet("color: #64748b; font-size: 11px; font-style: italic;")
            notes_label.setWordWrap(True)
            layout.addWidget(notes_label)

        # 4. Bottom footer: Salary & Applied Date
        bottom_row = QHBoxLayout()
        bottom_row.setContentsMargins(0, 4, 0, 0)
        bottom_row.setSpacing(8)

        if self.job.salary:
            salary_label = QLabel(self.job.salary)
            salary_label.setObjectName("SalaryBadge")
            bottom_row.addWidget(salary_label)

        bottom_row.addStretch()

        if self.job.date_applied:
            date_label = QLabel(f"📅 {self.job.date_applied}")
            date_label.setObjectName("DateBadge")
            bottom_row.addWidget(date_label)

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
