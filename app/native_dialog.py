"""Native desktop file and folder pickers (kdialog / zenity / XDG desktop portal)."""

import os
import shutil
import subprocess
from PyQt6.QtWidgets import QFileDialog


def get_existing_directory(parent=None, caption="Select Folder", start_dir=""):
    dir_path = start_dir if start_dir else os.path.expanduser("~")

    # 1. Native KDE kdialog
    kdialog = shutil.which("kdialog")
    if kdialog:
        res = subprocess.run([kdialog, "--title", caption, "--getexistingdirectory", dir_path], capture_output=True, text=True)
        if res.returncode == 0:
            return res.stdout.strip()
        elif res.returncode == 1:
            return ""

    # 2. Native GNOME zenity
    zenity = shutil.which("zenity")
    if zenity:
        res = subprocess.run([zenity, "--file-selection", "--directory", f"--title={caption}", f"--filename={dir_path}/"], capture_output=True, text=True)
        if res.returncode == 0:
            return res.stdout.strip()
        elif res.returncode == 1:
            return ""

    # 3. Fallback
    return QFileDialog.getExistingDirectory(parent, caption, dir_path, QFileDialog.Option.ShowDirsOnly)


def get_save_file_name(parent=None, caption="Save File", start_dir="", filter_pattern="*.json"):
    dir_path = start_dir if start_dir else os.path.expanduser("~")

    kdialog = shutil.which("kdialog")
    if kdialog:
        res = subprocess.run([kdialog, "--title", caption, "--getsavefilename", dir_path, filter_pattern], capture_output=True, text=True)
        if res.returncode == 0:
            return res.stdout.strip()
        elif res.returncode == 1:
            return ""

    zenity = shutil.which("zenity")
    if zenity:
        res = subprocess.run([zenity, "--file-selection", "--save", "--confirm-overwrite", f"--title={caption}", f"--filename={dir_path}"], capture_output=True, text=True)
        if res.returncode == 0:
            return res.stdout.strip()
        elif res.returncode == 1:
            return ""

    path, _ = QFileDialog.getSaveFileName(parent, caption, dir_path, filter_pattern)
    return path


def get_open_file_name(parent=None, caption="Open File", start_dir="", filter_pattern="*.json"):
    dir_path = start_dir if start_dir else os.path.expanduser("~")

    kdialog = shutil.which("kdialog")
    if kdialog:
        res = subprocess.run([kdialog, "--title", caption, "--getopenfilename", dir_path, filter_pattern], capture_output=True, text=True)
        if res.returncode == 0:
            return res.stdout.strip()
        elif res.returncode == 1:
            return ""

    zenity = shutil.which("zenity")
    if zenity:
        res = subprocess.run([zenity, "--file-selection", f"--title={caption}", f"--filename={dir_path}"], capture_output=True, text=True)
        if res.returncode == 0:
            return res.stdout.strip()
        elif res.returncode == 1:
            return ""

    path, _ = QFileDialog.getOpenFileName(parent, caption, dir_path, filter_pattern)
    return path
