from app.icon_helper import get_icon, get_pixmap
"""Main Application Window for Jobbie."""

from typing import List, Optional
from PyQt6.QtWidgets import (
    QMainWindow, QWidget, QVBoxLayout, QHBoxLayout, QLabel,
    QLineEdit, QPushButton, QButtonGroup, QFileDialog, QMessageBox,
    QFrame
)
from PyQt6.QtCore import Qt, QTimer
from PyQt6.QtGui import QKeySequence, QShortcut

from app.models import JobApplication
from app.config import JOB_TYPES, STATUSES
from app.storage import StorageManager
from app.styles import get_stylesheet
from app.ui.kanban_board import KanbanBoardWidget
from app.ui.job_dialog import JobDialog
from app.ui.confirm_dialog import ConfirmDialog


class MainWindow(QMainWindow):
    def __init__(self, storage: StorageManager, dark_mode: bool = False):
        super().__init__()
        self.storage = storage
        self.dark_mode = dark_mode
        self.jobs: List[JobApplication] = []
        self.active_type_filter = "All"
        self.search_query = ""

        self.setWindowTitle("Jobbie — Native Job Pipeline & Kanban")
        self.resize(1340, 840)
        self.setMinimumSize(960, 600)
        self.setWindowState(Qt.WindowState.WindowMaximized)

        self.init_ui()
        self.apply_theme()
        self.load_data()
        self.setup_shortcuts()

    def init_ui(self):
        central_widget = QWidget()
        main_layout = QVBoxLayout(central_widget)
        main_layout.setContentsMargins(0, 0, 0, 0)
        main_layout.setSpacing(0)

        # 1. Top Command Bar
        header_bar = QFrame()
        header_bar.setObjectName("HeaderBar")
        header_layout = QVBoxLayout(header_bar)
        header_layout.setContentsMargins(16, 12, 16, 12)
        header_layout.setSpacing(10)

        # Top row: Brand + Compact Metrics + Quick Actions
        top_row = QHBoxLayout()
        top_row.setSpacing(16)

        # Brand
        brand_label = QLabel("Jobbie")
        brand_label.setObjectName("AppTitle")
        top_row.addWidget(brand_label)

        # Compact Metric Badges (Does not waste screen space!)
        self.badge_total = QLabel("0 Total")
        self.badge_total.setObjectName("BadgeTotal")
        self.badge_total.setProperty("class", "metric-badge")
        self.badge_active = QLabel("0 Active")
        self.badge_active.setObjectName("BadgeActive")
        self.badge_active.setProperty("class", "metric-badge")
        self.badge_offers = QLabel("0 Offers")
        self.badge_offers.setObjectName("BadgeOffers")
        self.badge_offers.setProperty("class", "metric-badge")
        
        top_row.addWidget(self.badge_total)
        top_row.addWidget(self.badge_active)
        top_row.addWidget(self.badge_offers)
        top_row.addStretch()

        # Action: Export / Backup
        export_btn = QPushButton("Backup")
        export_btn.setToolTip("Export jobs to JSON file (Ctrl+E)")
        export_btn.setCursor(Qt.CursorShape.PointingHandCursor)
        export_btn.clicked.connect(self.export_jobs)
        top_row.addWidget(export_btn)

        # Action: Import
        import_btn = QPushButton("Import")
        import_btn.setToolTip("Import jobs from JSON file (Ctrl+I)")
        import_btn.setCursor(Qt.CursorShape.PointingHandCursor)
        import_btn.clicked.connect(self.import_jobs)
        top_row.addWidget(import_btn)

        # Action: Dark / Light Theme Toggle
        self.theme_btn = QPushButton("Dark" if not self.dark_mode else "Light")
        self.theme_btn.setIcon(get_icon("moon" if not self.dark_mode else "sun", "#09090b" if not self.dark_mode else "#fafafa", 13))
        self.theme_btn.setToolTip("Toggle Dark/Light Mode (Ctrl+T)")
        self.theme_btn.setCursor(Qt.CursorShape.PointingHandCursor)
        self.theme_btn.clicked.connect(self.toggle_theme)
        top_row.addWidget(self.theme_btn)

        # Action: Add Job Button (Primary)
        add_job_btn = QPushButton("Add Job")
        add_job_btn.setIcon(get_icon("plus", "#09090b" if self.dark_mode else "#fafafa", 13))
        add_job_btn.setObjectName("PrimaryButton")
        add_job_btn.setToolTip("Add new application (Ctrl+N)")
        add_job_btn.setCursor(Qt.CursorShape.PointingHandCursor)
        add_job_btn.clicked.connect(lambda: self.open_job_dialog())
        top_row.addWidget(add_job_btn)

        header_layout.addLayout(top_row)

        # Bottom row of header: Live Search + Filter Chips
        controls_row = QHBoxLayout()
        controls_row.setSpacing(12)

        # Search Bar
        self.search_input = QLineEdit()
        self.search_input.setPlaceholderText("Search company, role, or notes... (Ctrl+F)")
        self.search_input.addAction(get_icon("search", "#71717a" if self.dark_mode else "#a1a1aa", 13), QLineEdit.ActionPosition.LeadingPosition)
        self.search_input.setClearButtonEnabled(True)
        self.search_input.textChanged.connect(self.on_search_changed)
        self.search_input.setFixedWidth(340)
        controls_row.addWidget(self.search_input)

        controls_row.addSpacing(6)

        # Filter Chips: All, Full-time, Part-time, Contract, Freelance, Internship
        filter_label = QLabel("Filter:")
        filter_label.setStyleSheet("color: #64748b; font-size: 12px; font-weight: 600;")
        controls_row.addWidget(filter_label)

        self.filter_btn_group = QButtonGroup(self)
        self.filter_btn_group.setExclusive(True)

        all_chip = QPushButton("All")
        all_chip.setCheckable(True)
        all_chip.setChecked(True)
        all_chip.setProperty("class", "filter-chip")
        all_chip.setCursor(Qt.CursorShape.PointingHandCursor)
        all_chip.clicked.connect(lambda: self.on_filter_changed("All"))
        self.filter_btn_group.addButton(all_chip)
        controls_row.addWidget(all_chip)

        for jtype in JOB_TYPES:
            chip = QPushButton(jtype)
            chip.setCheckable(True)
            chip.setProperty("class", "filter-chip")
            chip.setCursor(Qt.CursorShape.PointingHandCursor)
            chip.clicked.connect(lambda checked, t=jtype: self.on_filter_changed(t))
            self.filter_btn_group.addButton(chip)
            controls_row.addWidget(chip)

        controls_row.addStretch()
        header_layout.addLayout(controls_row)

        main_layout.addWidget(header_bar)

        # 2. Main Full-Height Kanban Board
        self.board = KanbanBoardWidget(dark_mode=self.dark_mode)
        self.board.job_moved.connect(self.on_job_moved)
        self.board.add_job_requested.connect(self.open_job_dialog)
        self.board.edit_job_requested.connect(self.edit_job_by_id)
        self.board.delete_job_requested.connect(self.confirm_delete_job)
        main_layout.addWidget(self.board, 1)

        self.setCentralWidget(central_widget)

    def setup_shortcuts(self):
        # Ctrl+N -> New Job
        new_shortcut = QShortcut(QKeySequence("Ctrl+N"), self)
        new_shortcut.activated.connect(lambda: self.open_job_dialog())

        # Ctrl+F -> Search Focus
        search_shortcut = QShortcut(QKeySequence("Ctrl+F"), self)
        search_shortcut.activated.connect(self.search_input.setFocus)

        # Ctrl+E -> Export
        export_shortcut = QShortcut(QKeySequence("Ctrl+E"), self)
        export_shortcut.activated.connect(self.export_jobs)

        # Ctrl+I -> Import
        import_shortcut = QShortcut(QKeySequence("Ctrl+I"), self)
        import_shortcut.activated.connect(self.import_jobs)

        # Ctrl+T -> Theme
        theme_shortcut = QShortcut(QKeySequence("Ctrl+T"), self)
        theme_shortcut.activated.connect(self.toggle_theme)

    def apply_theme(self):
        stylesheet = get_stylesheet(self.dark_mode)
        self.setStyleSheet(stylesheet)
        self.board.set_dark_mode(self.dark_mode)
        self.theme_btn.setText("Light" if self.dark_mode else "Dark")
        self.theme_btn.setIcon(get_icon("sun" if self.dark_mode else "moon", "#fafafa" if self.dark_mode else "#09090b", 13))

    def toggle_theme(self):
        self.dark_mode = not self.dark_mode
        self.apply_theme()
        self.refresh_board()

    def load_data(self):
        self.jobs = self.storage.load_jobs()
        self.refresh_board()

    def save_data(self):
        self.storage.save_jobs(self.jobs)
        self.update_stats()

    def get_next_application_number(self) -> int:
        return max((getattr(j, "application_number", 0) for j in self.jobs), default=0) + 1

    def update_stats(self):
        total = len(self.jobs)
        active = sum(1 for j in self.jobs if j.status in ["Applied", "Interviewing"])
        offers = sum(1 for j in self.jobs if j.status == "Offered")
        max_num = max((getattr(j, "application_number", 0) for j in self.jobs), default=0)

        self.badge_total.setText(f"{total} Logged (#{max_num:02d})")
        self.badge_total.setToolTip(f"Total logged applications: {total}\nLatest entry counter: #{max_num:02d}\nNext application will be: #{max_num + 1:02d}")
        self.badge_active.setText(f"{active} Active")
        self.badge_offers.setText(f"{offers} Offered" if offers != 1 else "1 Offer")

    def get_filtered_jobs(self) -> List[JobApplication]:
        query = self.search_query.lower().strip()
        filtered = []

        for job in self.jobs:
            # Type filter
            if self.active_type_filter != "All" and job.job_type != self.active_type_filter:
                continue

            # Query filter (matches company, title, notes, location)
            if query:
                haystack = f"{job.company} {job.title} {job.notes} {job.location}".lower()
                if query not in haystack:
                    continue

            filtered.append(job)

        return filtered

    def refresh_board(self):
        filtered_jobs = self.get_filtered_jobs()
        self.board.set_jobs(filtered_jobs)
        self.update_stats()

    def on_search_changed(self, text: str):
        self.search_query = text
        self.refresh_board()

    def on_filter_changed(self, job_type: str):
        self.active_type_filter = job_type
        self.refresh_board()

    def on_job_moved(self, job_id: str, new_status: str):
        for job in self.jobs:
            if job.id == job_id:
                if job.status != new_status:
                    job.status = new_status
                    self.save_data()
                    self.refresh_board()
                break

    def open_job_dialog(self, default_status: str = "Wishlist"):
        next_num = self.get_next_application_number()
        dialog = JobDialog(default_status=default_status, next_app_number=next_num, parent=self)
        if dialog.exec():
            new_job = dialog.get_job()
            if getattr(new_job, "application_number", 0) <= 0:
                new_job.application_number = next_num
            self.jobs.append(new_job)
            self.save_data()
            self.refresh_board()

    def edit_job_by_id(self, job_id: str):
        target_job = next((j for j in self.jobs if j.id == job_id), None)
        if not target_job:
            return

        dialog = JobDialog(job=target_job, next_app_number=getattr(target_job, "application_number", 1), parent=self)
        if dialog.exec():
            self.save_data()
            self.refresh_board()

    def confirm_delete_job(self, job_id: str):
        target_job = next((j for j in self.jobs if j.id == job_id), None)
        if not target_job:
            return

        dialog = ConfirmDialog(
            parent=self,
            title="Delete Application",
            message=f"Are you sure you want to delete '{target_job.title}' at '{target_job.company}'? This action cannot be undone."
        )
        if dialog.exec():
            self.jobs = [j for j in self.jobs if j.id != job_id]
            self.save_data()
            self.refresh_board()

    def export_jobs(self):
        from app.native_dialog import get_save_file_name
        file_path = get_save_file_name(
            self, "Backup Jobs to JSON", "jobbie_backup.json", "*.json"
        )
        if file_path:
            success = self.storage.export_to_file(file_path, self.jobs)
            if success:
                QMessageBox.information(self, "Backup Complete", f"Successfully backed up {len(self.jobs)} applications to:\n{file_path}")
            else:
                QMessageBox.critical(self, "Backup Failed", "Failed to write backup file.")

    def import_jobs(self):
        from app.native_dialog import get_open_file_name
        file_path = get_open_file_name(
            self, "Import Jobs from JSON", "", "*.json"
        )
        if file_path:
            imported = self.storage.import_from_file(file_path)
            if imported is not None:
                # Merge or replace? Ask user
                res = QMessageBox.question(
                    self,
                    "Import Mode",
                    f"Found {len(imported)} applications in backup.\n\nClick 'Yes' to merge with existing jobs, or 'No' to replace all existing jobs.",
                    QMessageBox.StandardButton.Yes | QMessageBox.StandardButton.No | QMessageBox.StandardButton.Cancel
                )
                if res == QMessageBox.StandardButton.Yes:
                    # Merge by ID
                    existing_ids = {j.id for j in self.jobs}
                    for j in imported:
                        if j.id not in existing_ids:
                            self.jobs.append(j)
                    self.save_data()
                    self.refresh_board()
                    QMessageBox.information(self, "Import Complete", f"Merged applications. Total jobs now: {len(self.jobs)}")
                elif res == QMessageBox.StandardButton.No:
                    self.jobs = imported
                    self.save_data()
                    self.refresh_board()
                    QMessageBox.information(self, "Import Complete", f"Replaced with {len(self.jobs)} imported applications.")
            else:
                QMessageBox.critical(self, "Import Failed", "Failed to parse JSON file.")
