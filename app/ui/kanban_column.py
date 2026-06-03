"""Kanban Column widget supporting native drag-and-drop."""

from typing import List
from PyQt6.QtWidgets import (
    QFrame, QVBoxLayout, QHBoxLayout, QLabel, QPushButton, QScrollArea, QWidget
)
from PyQt6.QtCore import Qt, pyqtSignal
from PyQt6.QtGui import QDragEnterEvent, QDragLeaveEvent, QDropEvent

from app.models import JobApplication
from app.config import STATUS_META
from app.ui.job_card import JobCardWidget


class KanbanColumnWidget(QFrame):
    job_dropped = pyqtSignal(str, str)     # job_id, new_status
    add_requested = pyqtSignal(str)       # status
    edit_requested = pyqtSignal(str)      # job_id
    delete_requested = pyqtSignal(str)    # job_id

    def __init__(self, status: str, dark_mode: bool = False, parent=None):
        super().__init__(parent)
        self.status = status
        self.dark_mode = dark_mode
        self.cards: List[JobCardWidget] = []

        self.setObjectName("KanbanColumn")
        self.setAcceptDrops(True)
        self.setMinimumWidth(280)
        self.setMaximumWidth(360)

        self.init_ui()

    def init_ui(self):
        main_layout = QVBoxLayout(self)
        main_layout.setContentsMargins(6, 6, 6, 6)
        main_layout.setSpacing(8)

        # 1. Column Header
        header_frame = QFrame()
        header_frame.setObjectName("ColumnHeader")
        header_layout = QHBoxLayout(header_frame)
        header_layout.setContentsMargins(8, 6, 8, 8)
        header_layout.setSpacing(8)

        meta = STATUS_META.get(self.status, {})
        status_color = meta.get("dark_color" if self.dark_mode else "light_color", "#6366f1")

        # Status icon
        icon_label = QLabel(meta.get("icon", "📋"))
        icon_label.setStyleSheet("font-size: 15px;")
        header_layout.addWidget(icon_label)

        # Title
        title_label = QLabel(self.status)
        title_label.setObjectName("ColumnTitle")
        header_layout.addWidget(title_label)

        # Count badge
        self.count_badge = QLabel("0")
        self.count_badge.setObjectName("ColumnCount")
        header_layout.addWidget(self.count_badge)

        header_layout.addStretch()

        # "+ Add" button in header
        add_btn = QPushButton("+")
        add_btn.setToolTip(f"Add application to {self.status}")
        add_btn.setCursor(Qt.CursorShape.PointingHandCursor)
        add_btn.setFixedSize(24, 24)
        add_btn.setStyleSheet(
            "font-size: 14px; font-weight: bold; border-radius: 12px; padding: 0px;"
        )
        add_btn.clicked.connect(lambda: self.add_requested.emit(self.status))
        header_layout.addWidget(add_btn)

        main_layout.addWidget(header_frame)

        # 2. Scrollable Cards Container
        self.scroll_area = QScrollArea()
        self.scroll_area.setWidgetResizable(True)
        self.scroll_area.setFrameShape(QFrame.Shape.NoFrame)
        self.scroll_area.setHorizontalScrollBarPolicy(Qt.ScrollBarPolicy.ScrollBarAlwaysOff)

        self.cards_container = QWidget()
        self.cards_layout = QVBoxLayout(self.cards_container)
        self.cards_layout.setContentsMargins(4, 4, 4, 4)
        self.cards_layout.setSpacing(10)
        self.cards_layout.addStretch()

        self.scroll_area.setWidget(self.cards_container)
        main_layout.addWidget(self.scroll_area)

    def set_jobs(self, jobs: List[JobApplication]):
        # Clear existing card widgets
        while self.cards_layout.count() > 1:
            item = self.cards_layout.takeAt(0)
            if item.widget():
                item.widget().deleteLater()
        self.cards.clear()

        # Update count
        self.count_badge.setText(str(len(jobs)))

        if not jobs:
            # Placeholder widget
            placeholder = QLabel("Drop applications here\nor click + to add")
            placeholder.setAlignment(Qt.AlignmentFlag.AlignCenter)
            placeholder.setStyleSheet(
                "color: #94a3b8; font-size: 12px; font-style: italic; "
                "border: 1.5px dashed #cbd5e1; border-radius: 10px; padding: 30px 10px; margin: 6px 0;"
            )
            self.cards_layout.insertWidget(0, placeholder)
            return

        # Insert cards
        for i, job in enumerate(jobs):
            card = JobCardWidget(job, dark_mode=self.dark_mode)
            card.edit_requested.connect(self.edit_requested.emit)
            card.delete_requested.connect(self.delete_requested.emit)
            self.cards.append(card)
            self.cards_layout.insertWidget(i, card)

    def dragEnterEvent(self, event: QDragEnterEvent):
        if event.mimeData().hasFormat("application/x-jobbie-job-id"):
            event.acceptProposedAction()
            self.setProperty("dragOver", "true")
            self.style().unpolish(self)
            self.style().polish(self)
        else:
            event.ignore()

    def dragMoveEvent(self, event):
        if event.mimeData().hasFormat("application/x-jobbie-job-id"):
            event.acceptProposedAction()
        else:
            event.ignore()

    def dragLeaveEvent(self, event: QDragLeaveEvent):
        self.setProperty("dragOver", "false")
        self.style().unpolish(self)
        self.style().polish(self)
        event.accept()

    def dropEvent(self, event: QDropEvent):
        self.setProperty("dragOver", "false")
        self.style().unpolish(self)
        self.style().polish(self)

        if event.mimeData().hasFormat("application/x-jobbie-job-id"):
            raw_id = event.mimeData().data("application/x-jobbie-job-id").data()
            job_id = raw_id.decode("utf-8")
            event.acceptProposedAction()
            self.job_dropped.emit(job_id, self.status)
        else:
            event.ignore()
