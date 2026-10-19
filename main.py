#!/usr/bin/env python3
"""Jobbie - Native Desktop Kanban Job Application Tracker."""

import sys
import os
import argparse
from pathlib import Path

# Add project root to path
PROJECT_ROOT = Path(__file__).resolve().parent
if str(PROJECT_ROOT) not in sys.path:
    sys.path.insert(0, str(PROJECT_ROOT))

from PyQt6.QtWidgets import QApplication
from PyQt6.QtCore import Qt

from app.storage import StorageManager
from app.ui.main_window import MainWindow


def main():
    parser = argparse.ArgumentParser(description="Jobbie - Native Kanban Job Tracker")
    parser.add_argument("--dark", action="store_true", help="Launch in dark mode")
    parser.add_argument("--data", type=str, default=None, help="Custom path to jobs.json")
    args = parser.parse_args()

    # Enable High DPI scaling
    os.environ["QT_AUTO_SCREEN_SCALE_FACTOR"] = "1"

    app = QApplication(sys.argv)
    app.setApplicationName("Jobbie")
    app.setApplicationDisplayName("Jobbie — Job Application Tracker")
    app.setOrganizationName("Jobbie")

    icon_path = str(PROJECT_ROOT / "resources" / "icon.png")
    if os.path.exists(icon_path):
        from PyQt6.QtGui import QIcon
        app.setWindowIcon(QIcon(icon_path))

    # Use Fusion base style for consistent, modern cross-desktop rendering
    app.setStyle("Fusion")

    # Determine data file location: default to local jobs.json in project dir
    data_path = args.data
    if data_path is None:
        data_path = str(PROJECT_ROOT / "jobs.json")

    storage = StorageManager(file_path=data_path)
    window = MainWindow(storage=storage, dark_mode=args.dark)
    window.showMaximized()

    sys.exit(app.exec())


if __name__ == "__main__":
    main()
