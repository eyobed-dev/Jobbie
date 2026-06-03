"""Kanban Board container holding columns across pipeline stages."""

from typing import List, Dict
from PyQt6.QtWidgets import (
    QWidget, QHBoxLayout, QScrollArea, QFrame
)
from PyQt6.QtCore import Qt, pyqtSignal

from app.models import JobApplication
from app.config import STATUSES
from app.ui.kanban_column import KanbanColumnWidget


class KanbanBoardWidget(QWidget):
    job_moved = pyqtSignal(str, str)            # job_id, new_status
    add_job_requested = pyqtSignal(str)        # status
    edit_job_requested = pyqtSignal(str)       # job_id
    delete_job_requested = pyqtSignal(str)     # job_id

    def __init__(self, dark_mode: bool = False, parent=None):
        super().__init__(parent)
        self.dark_mode = dark_mode
        self.columns: Dict[str, KanbanColumnWidget] = {}

        self.init_ui()

    def init_ui(self):
        # Outer layout
        outer_layout = QHBoxLayout(self)
        outer_layout.setContentsMargins(12, 12, 12, 12)
        outer_layout.setSpacing(0)

        # Scroll area for horizontal overflow on smaller screens
        self.scroll_area = QScrollArea()
        self.scroll_area.setWidgetResizable(True)
        self.scroll_area.setFrameShape(QFrame.Shape.NoFrame)
        self.scroll_area.setVerticalScrollBarPolicy(Qt.ScrollBarPolicy.ScrollBarAlwaysOff)
        self.scroll_area.setHorizontalScrollBarPolicy(Qt.ScrollBarPolicy.ScrollBarAsNeeded)

        # Content widget
        content_widget = QWidget()
        content_layout = QHBoxLayout(content_widget)
        content_layout.setContentsMargins(0, 0, 0, 0)
        content_layout.setSpacing(14)

        for status in STATUSES:
            col = KanbanColumnWidget(status, dark_mode=self.dark_mode)
            col.job_dropped.connect(self.job_moved.emit)
            col.add_requested.connect(self.add_job_requested.emit)
            col.edit_requested.connect(self.edit_job_requested.emit)
            col.delete_requested.connect(self.delete_job_requested.emit)
            self.columns[status] = col
            content_layout.addWidget(col)

        self.scroll_area.setWidget(content_widget)
        outer_layout.addWidget(self.scroll_area)

    def set_jobs(self, jobs: List[JobApplication]):
        """Distribute jobs into their respective status columns."""
        for status, col in self.columns.items():
            status_jobs = [j for j in jobs if j.status == status]
            col.set_jobs(status_jobs)

    def set_dark_mode(self, dark_mode: bool):
        self.dark_mode = dark_mode
        for col in self.columns.values():
            col.dark_mode = dark_mode
