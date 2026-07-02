"""Dialog for creating and editing job applications."""

from typing import Optional
from datetime import datetime
from PyQt6.QtWidgets import (
    QDialog, QVBoxLayout, QHBoxLayout, QGridLayout, QLabel,
    QLineEdit, QComboBox, QDateEdit, QTextEdit, QPushButton,
    QMessageBox, QFileDialog
)
from PyQt6.QtCore import Qt, QDate, QUrl
from PyQt6.QtGui import QDesktopServices

from app.models import JobApplication
from app.config import STATUSES, JOB_TYPES


class JobDialog(QDialog):
    def __init__(self, job: Optional[JobApplication] = None, default_status: str = "Wishlist", parent=None):
        super().__init__(parent)
        self.job = job
        self.default_status = default_status

        title_text = "Edit Job Application" if job else "Add New Job Application"
        self.setWindowTitle(title_text)
        self.setMinimumWidth(560)
        self.setModal(True)
        self.setWindowFlags(self.windowFlags() & ~Qt.WindowType.WindowContextHelpButtonHint)

        self.init_ui()
        self.populate_data()

    def init_ui(self):
        main_layout = QVBoxLayout(self)
        main_layout.setContentsMargins(24, 20, 24, 20)
        main_layout.setSpacing(16)

        # Header
        header_title = QLabel("Edit Job Application" if self.job else "New Job Application")
        header_title.setObjectName("DialogHeaderTitle")
        header_title.setStyleSheet("font-size: 17px; font-weight: bold;")
        main_layout.addWidget(header_title)

        # Form Grid
        grid = QGridLayout()
        grid.setHorizontalSpacing(16)
        grid.setVerticalSpacing(14)

        # Row 0: Company & Title
        lbl_company = QLabel("Company Name *")
        lbl_company.setObjectName("FieldLabel")
        self.company_input = QLineEdit()
        self.company_input.setPlaceholderText("e.g. Stripe, Linear, GitHub")

        lbl_title = QLabel("Job Title *")
        lbl_title.setObjectName("FieldLabel")
        self.title_input = QLineEdit()
        self.title_input.setPlaceholderText("e.g. Fullstack Engineer")

        grid.addWidget(lbl_company, 0, 0)
        grid.addWidget(self.company_input, 1, 0)
        grid.addWidget(lbl_title, 0, 1)
        grid.addWidget(self.title_input, 1, 1)

        # Row 1: Status & Job Type
        lbl_status = QLabel("Pipeline Status")
        lbl_status.setObjectName("FieldLabel")
        self.status_combo = QComboBox()
        self.status_combo.addItems(STATUSES)

        lbl_type = QLabel("Job Type")
        lbl_type.setObjectName("FieldLabel")
        self.type_combo = QComboBox()
        self.type_combo.addItems(JOB_TYPES)

        grid.addWidget(lbl_status, 2, 0)
        grid.addWidget(self.status_combo, 3, 0)
        grid.addWidget(lbl_type, 2, 1)
        grid.addWidget(self.type_combo, 3, 1)

        # Row 2: Salary & Location
        lbl_salary = QLabel("Salary / Compensation")
        lbl_salary.setObjectName("FieldLabel")
        self.salary_input = QLineEdit()
        self.salary_input.setPlaceholderText("e.g. $140k - $165k or $90/hr")

        lbl_location = QLabel("Location / Work Mode")
        lbl_location.setObjectName("FieldLabel")
        self.location_input = QLineEdit()
        self.location_input.setPlaceholderText("e.g. Remote, Hybrid, San Francisco")

        grid.addWidget(lbl_salary, 4, 0)
        grid.addWidget(self.salary_input, 5, 0)
        grid.addWidget(lbl_location, 4, 1)
        grid.addWidget(self.location_input, 5, 1)

        # Row 3: Date Applied
        lbl_date = QLabel("Date Applied")
        lbl_date.setObjectName("FieldLabel")
        self.date_edit = QDateEdit()
        self.date_edit.setCalendarPopup(True)
        self.date_edit.setDisplayFormat("yyyy-MM-dd")
        self.date_edit.setDate(QDate.currentDate())

        grid.addWidget(lbl_date, 6, 0)
        grid.addWidget(self.date_edit, 7, 0)

        # Job posting URL (with test button)
        lbl_link = QLabel("Job Posting URL")
        lbl_link.setObjectName("FieldLabel")
        url_layout = QHBoxLayout()
        url_layout.setSpacing(6)
        self.link_input = QLineEdit()
        self.link_input.setPlaceholderText("https://...")
        test_link_btn = QPushButton("↗ Open")
        test_link_btn.setToolTip("Open URL in browser to test")
        test_link_btn.clicked.connect(self._test_url)
        url_layout.addWidget(self.link_input)
        url_layout.addWidget(test_link_btn)

        grid.addWidget(lbl_link, 6, 1)
        grid.addLayout(url_layout, 7, 1)

        # Row 4: Dedicated CV & Application Documents Folder
        lbl_folder = QLabel("📁 Attached CV & Application Documents Folder")
        lbl_folder.setObjectName("FieldLabel")
        folder_layout = QHBoxLayout()
        folder_layout.setSpacing(6)

        self.folder_input = QLineEdit()
        self.folder_input.setPlaceholderText("/home/eyobed/Documents/Work/CVs/...")

        browse_folder_btn = QPushButton("Browse...")
        browse_folder_btn.setToolTip("Select folder on your computer")
        browse_folder_btn.setCursor(Qt.CursorShape.PointingHandCursor)
        browse_folder_btn.clicked.connect(self._browse_folder)

        open_folder_btn = QPushButton("📂 Open")
        open_folder_btn.setToolTip("Open folder in file manager")
        open_folder_btn.setCursor(Qt.CursorShape.PointingHandCursor)
        open_folder_btn.clicked.connect(self._open_folder)

        create_folder_btn = QPushButton("✨ Create")
        create_folder_btn.setToolTip("Create folder if it does not exist")
        create_folder_btn.setCursor(Qt.CursorShape.PointingHandCursor)
        create_folder_btn.clicked.connect(self._create_folder)

        folder_layout.addWidget(self.folder_input)
        folder_layout.addWidget(browse_folder_btn)
        folder_layout.addWidget(create_folder_btn)
        folder_layout.addWidget(open_folder_btn)

        grid.addWidget(lbl_folder, 8, 0, 1, 2)
        grid.addLayout(folder_layout, 9, 0, 1, 2)

        main_layout.addLayout(grid)

        # Notes / Interview Log
        lbl_notes = QLabel("Notes, Recruiter Contacts & Interview Log")
        lbl_notes.setObjectName("FieldLabel")
        main_layout.addWidget(lbl_notes)

        self.notes_input = QTextEdit()
        self.notes_input.setPlaceholderText(
            "Add interview feedback, questions asked, recruiter contact, timeline, or next steps..."
        )
        self.notes_input.setFixedHeight(95)
        main_layout.addWidget(self.notes_input)

        # Buttons footer
        btn_layout = QHBoxLayout()
        btn_layout.setSpacing(10)
        btn_layout.addStretch()

        cancel_btn = QPushButton("Cancel")
        cancel_btn.clicked.connect(self.reject)
        cancel_btn.setCursor(Qt.CursorShape.PointingHandCursor)

        save_btn = QPushButton("Save Application" if self.job else "Add to Pipeline")
        save_btn.setObjectName("PrimaryButton")
        save_btn.clicked.connect(self.save)
        save_btn.setCursor(Qt.CursorShape.PointingHandCursor)

        btn_layout.addWidget(cancel_btn)
        btn_layout.addWidget(save_btn)

        main_layout.addLayout(btn_layout)

    def _test_url(self):
        url = self.link_input.text().strip()
        if not url:
            return
        if not url.startswith("http://") and not url.startswith("https://"):
            url = "https://" + url
        QDesktopServices.openUrl(QUrl(url))

    def _browse_folder(self):
        from app.native_dialog import get_existing_directory
        current = self.folder_input.text().strip()
        start_dir = current if current else "/home/eyobed/Documents/Work/CVs"
        selected = get_existing_directory(self, "Select CV & Documents Folder", start_dir)
        if selected:
            self.folder_input.setText(selected)

    def _open_folder(self):
        import os
        path = self.folder_input.text().strip()
        if not path:
            QMessageBox.information(self, "No Folder", "Please specify or browse for a folder location first.")
            return

        if not os.path.exists(path):
            res = QMessageBox.question(
                self,
                "Folder Does Not Exist",
                f"The folder '{path}' does not exist yet. Would you like to create it now?",
                QMessageBox.StandardButton.Yes | QMessageBox.StandardButton.No
            )
            if res == QMessageBox.StandardButton.Yes:
                os.makedirs(path, exist_ok=True)
                QDesktopServices.openUrl(QUrl.fromLocalFile(path))
            return

        QDesktopServices.openUrl(QUrl.fromLocalFile(path))

    def _create_folder(self):
        import os
        path = self.folder_input.text().strip()
        if not path:
            company = self.company_input.text().strip()
            if not company:
                company = "NewApplication"
            path = f"/home/eyobed/Documents/Work/CVs/{company}"
            self.folder_input.setText(path)

        if os.path.exists(path):
            QMessageBox.information(self, "Folder Exists", f"Folder already exists:\n{path}")
            return

        try:
            os.makedirs(path, exist_ok=True)
            QMessageBox.information(self, "Folder Created", f"Successfully created folder:\n{path}")
        except Exception as e:
            QMessageBox.critical(self, "Error", f"Failed to create folder:\n{e}")

    def populate_data(self):
        if self.job:
            self.company_input.setText(self.job.company)
            self.title_input.setText(self.job.title)
            self.status_combo.setCurrentText(self.job.status)
            self.type_combo.setCurrentText(self.job.job_type)
            self.salary_input.setText(self.job.salary)
            self.location_input.setText(self.job.location)
            self.link_input.setText(self.job.link)
            self.folder_input.setText(getattr(self.job, 'folder_path', ''))
            self.notes_input.setPlainText(self.job.notes)

            if self.job.date_applied:
                try:
                    dt = datetime.strptime(self.job.date_applied, "%Y-%m-%d")
                    self.date_edit.setDate(QDate(dt.year, dt.month, dt.day))
                except Exception:
                    self.date_edit.setDate(QDate.currentDate())
        else:
            self.status_combo.setCurrentText(self.default_status)

    def save(self):
        company = self.company_input.text().strip()
        title = self.title_input.text().strip()

        if not company:
            QMessageBox.warning(self, "Validation Error", "Please provide a Company Name.")
            self.company_input.setFocus()
            return

        if not title:
            QMessageBox.warning(self, "Validation Error", "Please provide a Job Title.")
            self.title_input.setFocus()
            return

        qdate = self.date_edit.date()
        date_str = f"{qdate.year():04d}-{qdate.month():02d}-{qdate.day():02d}"

        if self.job:
            self.job.company = company
            self.job.title = title
            self.job.status = self.status_combo.currentText()
            self.job.job_type = self.type_combo.currentText()
            self.job.salary = self.salary_input.text().strip()
            self.job.location = self.location_input.text().strip()
            self.job.date_applied = date_str
            self.job.link = self.link_input.text().strip()
            self.job.folder_path = self.folder_input.text().strip()
            self.job.notes = self.notes_input.toPlainText()
            self.job.updated_at = datetime.now().isoformat()
        else:
            self.job = JobApplication(
                company=company,
                title=title,
                status=self.status_combo.currentText(),
                job_type=self.type_combo.currentText(),
                salary=self.salary_input.text().strip(),
                location=self.location_input.text().strip(),
                date_applied=date_str,
                link=self.link_input.text().strip(),
                folder_path=self.folder_input.text().strip(),
                notes=self.notes_input.toPlainText(),
            )

        self.accept()

    def get_job(self) -> JobApplication:
        return self.job
