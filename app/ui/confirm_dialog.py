"""Confirmation dialog for destructive actions."""

from PyQt6.QtWidgets import (
    QDialog, QVBoxLayout, QHBoxLayout, QLabel, QPushButton, QFrame
)
from PyQt6.QtCore import Qt


class ConfirmDialog(QDialog):
    def __init__(self, parent=None, title="Delete Application", message="Are you sure you want to delete this job application? This action cannot be undone."):
        super().__init__(parent)
        self.setWindowTitle(title)
        self.setFixedWidth(380)
        self.setModal(True)
        self.setWindowFlags(self.windowFlags() & ~Qt.WindowType.WindowContextHelpButtonHint)

        layout = QVBoxLayout(self)
        layout.setContentsMargins(24, 24, 24, 20)
        layout.setSpacing(16)

        # Header icon + title
        header_layout = QHBoxLayout()
        header_layout.setSpacing(12)

        icon_label = QLabel("⚠️")
        icon_label.setStyleSheet("font-size: 24px;")
        header_layout.addWidget(icon_label)

        title_label = QLabel(title)
        title_label.setObjectName("DialogHeaderTitle")
        title_label.setStyleSheet("font-size: 16px; font-weight: bold;")
        header_layout.addWidget(title_label)
        header_layout.addStretch()

        layout.addLayout(header_layout)

        # Message body
        msg_label = QLabel(message)
        msg_label.setWordWrap(True)
        msg_label.setStyleSheet("color: #64748b; font-size: 13px; line-height: 1.4;")
        layout.addWidget(msg_label)

        layout.addSpacing(8)

        # Buttons
        btn_layout = QHBoxLayout()
        btn_layout.setSpacing(12)

        self.cancel_btn = QPushButton("Cancel")
        self.cancel_btn.clicked.connect(self.reject)
        self.cancel_btn.setCursor(Qt.CursorShape.PointingHandCursor)

        self.confirm_btn = QPushButton("Delete")
        self.confirm_btn.setObjectName("DangerButton")
        self.confirm_btn.clicked.connect(self.accept)
        self.confirm_btn.setCursor(Qt.CursorShape.PointingHandCursor)

        btn_layout.addWidget(self.cancel_btn)
        btn_layout.addWidget(self.confirm_btn)

        layout.addLayout(btn_layout)
