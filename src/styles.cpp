#include "styles.h"

namespace Styles {

static const char* LIGHT_THEME = R"(
QWidget {
    font-family: -apple-system, BlinkMacSystemFont, "Segoe UI", Roboto, "Inter", Ubuntu, sans-serif;
    font-size: 13px;
    color: #1e293b;
    background-color: #f8fafc;
}

#HeaderBar {
    background-color: #ffffff;
    border-bottom: 1px solid #e2e8f0;
    padding: 10px 16px;
}

#AppTitle {
    font-size: 18px;
    font-weight: 700;
    color: #4f46e5;
    letter-spacing: -0.5px;
}

QLineEdit, QDateEdit, QComboBox, QTextEdit {
    background-color: #ffffff;
    border: 1px solid #cbd5e1;
    border-radius: 8px;
    padding: 7px 12px;
    color: #0f172a;
    selection-background-color: #6366f1;
}

QLineEdit:focus, QDateEdit:focus, QComboBox:focus, QTextEdit:focus {
    border: 1.5px solid #6366f1;
    background-color: #ffffff;
}

QPushButton {
    background-color: #ffffff;
    border: 1px solid #cbd5e1;
    border-radius: 8px;
    padding: 6px 14px;
    font-weight: 600;
    color: #334155;
}

QPushButton:hover {
    background-color: #f1f5f9;
    border-color: #94a3b8;
    color: #0f172a;
}

QPushButton:pressed {
    background-color: #e2e8f0;
}

QPushButton#PrimaryButton {
    background-color: #4f46e5;
    color: #ffffff;
    border: 1px solid #4338ca;
    padding: 7px 16px;
    border-radius: 8px;
    font-weight: 600;
}

QPushButton#PrimaryButton:hover {
    background-color: #4338ca;
    border-color: #3730a3;
}

QPushButton#DangerButton {
    background-color: #e11d48;
    color: #ffffff;
    border: 1px solid #be123c;
    border-radius: 8px;
    font-weight: 600;
}

QPushButton#DangerButton:hover {
    background-color: #be123c;
}

QPushButton.filter-chip {
    background-color: #ffffff;
    border: 1px solid #e2e8f0;
    border-radius: 14px;
    padding: 4px 12px;
    font-size: 12px;
    font-weight: 500;
    color: #475569;
}

QPushButton.filter-chip:hover {
    background-color: #f1f5f9;
    border-color: #cbd5e1;
    color: #1e293b;
}

QPushButton.filter-chip:checked {
    background-color: #0f172a;
    color: #ffffff;
    border-color: #0f172a;
    font-weight: 600;
}

QLabel.metric-badge {
    background-color: #f1f5f9;
    border: 1px solid #e2e8f0;
    border-radius: 12px;
    padding: 3px 10px;
    font-size: 11px;
    font-weight: 600;
    color: #475569;
}

#KanbanColumn {
    background-color: #f1f5f9;
    border: 1.5px solid transparent;
    border-radius: 14px;
}

#KanbanColumn[dragOver="true"] {
    background-color: #eef2ff;
    border: 1.5px dashed #6366f1;
}

#ColumnHeader {
    background-color: transparent;
    border-bottom: 1px solid #e2e8f0;
    padding: 10px 14px;
}

#ColumnTitle {
    font-weight: 700;
    font-size: 13px;
    color: #334155;
    letter-spacing: -0.2px;
}

#ColumnCount {
    background-color: #e2e8f0;
    color: #475569;
    font-size: 11px;
    font-weight: 700;
    border-radius: 10px;
    padding: 2px 7px;
}

#JobCard {
    background-color: #ffffff;
    border: 1px solid #e2e8f0;
    border-radius: 10px;
    padding: 12px;
}

#JobCard:hover {
    border: 1px solid #818cf8;
    background-color: #ffffff;
}

#JobCard[dragging="true"] {
    opacity: 0.5;
    border: 1.5px dashed #6366f1;
    background-color: #f8fafc;
}

#CardTitle {
    font-size: 14px;
    font-weight: 700;
    color: #0f172a;
    line-height: 1.2;
}

#CardCompany {
    font-size: 12px;
    font-weight: 500;
    color: #64748b;
}

#TypeBadge {
    border-radius: 4px;
    padding: 2px 6px;
    font-size: 10px;
    font-weight: 700;
    letter-spacing: 0.5px;
}

#SalaryBadge {
    background-color: #ecfdf5;
    color: #047857;
    border: 1px solid #a7f3d0;
    border-radius: 4px;
    padding: 2px 6px;
    font-size: 11px;
    font-weight: 600;
}

#DateBadge {
    font-size: 11px;
    color: #94a3b8;
}

QPushButton.card-action-btn {
    background-color: transparent;
    border: none;
    border-radius: 4px;
    padding: 3px 6px;
    color: #64748b;
    font-size: 12px;
}

QPushButton.card-action-btn:hover {
    background-color: #e2e8f0;
    color: #0f172a;
}

QPushButton.card-action-btn#deleteBtn:hover {
    background-color: #ffe4e6;
    color: #e11d48;
}

QPushButton.card-action-btn#folderBtn:hover {
    background-color: #e0e7ff;
    color: #4338ca;
}

QScrollBar:vertical {
    border: none;
    background-color: transparent;
    width: 6px;
    margin: 2px 0;
}

QScrollBar::handle:vertical {
    background-color: #cbd5e1;
    min-height: 25px;
    border-radius: 3px;
}

QScrollBar::handle:vertical:hover {
    background-color: #94a3b8;
}

QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {
    height: 0px;
}

QScrollBar:horizontal {
    border: none;
    background-color: transparent;
    height: 6px;
}

QScrollBar::handle:horizontal {
    background-color: #cbd5e1;
    min-width: 25px;
    border-radius: 3px;
}

QScrollBar::handle:horizontal:hover {
    background-color: #94a3b8;
}

QScrollBar::add-line:horizontal, QScrollBar::sub-line:horizontal {
    width: 0px;
}

QDialog {
    background-color: #ffffff;
}

QLabel#DialogHeaderTitle {
    font-size: 16px;
    font-weight: 700;
    color: #0f172a;
}

QLabel#FieldLabel {
    font-size: 12px;
    font-weight: 600;
    color: #334155;
}
)";

static const char* DARK_THEME = R"(
QWidget {
    font-family: -apple-system, BlinkMacSystemFont, "Segoe UI", Roboto, "Inter", Ubuntu, sans-serif;
    font-size: 13px;
    color: #f1f5f9;
    background-color: #090d16;
}

#HeaderBar {
    background-color: #0f172a;
    border-bottom: 1px solid #1e293b;
    padding: 10px 16px;
}

#AppTitle {
    font-size: 18px;
    font-weight: 700;
    color: #818cf8;
    letter-spacing: -0.5px;
}

QLineEdit, QDateEdit, QComboBox, QTextEdit {
    background-color: #1e293b;
    border: 1px solid #334155;
    border-radius: 8px;
    padding: 7px 12px;
    color: #f8fafc;
    selection-background-color: #4f46e5;
}

QLineEdit:focus, QDateEdit:focus, QComboBox:focus, QTextEdit:focus {
    border: 1.5px solid #818cf8;
    background-color: #1e293b;
}

QPushButton {
    background-color: #1e293b;
    border: 1px solid #334155;
    border-radius: 8px;
    padding: 6px 14px;
    font-weight: 600;
    color: #e2e8f0;
}

QPushButton:hover {
    background-color: #334155;
    border-color: #475569;
    color: #ffffff;
}

QPushButton:pressed {
    background-color: #475569;
}

QPushButton#PrimaryButton {
    background-color: #4f46e5;
    color: #ffffff;
    border: 1px solid #6366f1;
    padding: 7px 16px;
    border-radius: 8px;
    font-weight: 600;
}

QPushButton#PrimaryButton:hover {
    background-color: #4338ca;
    border-color: #818cf8;
}

QPushButton#DangerButton {
    background-color: #e11d48;
    color: #ffffff;
    border: 1px solid #f43f5e;
    border-radius: 8px;
    font-weight: 600;
}

QPushButton#DangerButton:hover {
    background-color: #be123c;
}

QPushButton.filter-chip {
    background-color: #1e293b;
    border: 1px solid #334155;
    border-radius: 14px;
    padding: 4px 12px;
    font-size: 12px;
    font-weight: 500;
    color: #94a3b8;
}

QPushButton.filter-chip:hover {
    background-color: #334155;
    border-color: #475569;
    color: #f8fafc;
}

QPushButton.filter-chip:checked {
    background-color: #818cf8;
    color: #0f172a;
    border-color: #818cf8;
    font-weight: 700;
}

QLabel.metric-badge {
    background-color: #1e293b;
    border: 1px solid #334155;
    border-radius: 12px;
    padding: 3px 10px;
    font-size: 11px;
    font-weight: 600;
    color: #94a3b8;
}

#KanbanColumn {
    background-color: #111827;
    border: 1.5px solid transparent;
    border-radius: 14px;
}

#KanbanColumn[dragOver="true"] {
    background-color: #1e1b4b;
    border: 1.5px dashed #818cf8;
}

#ColumnHeader {
    background-color: transparent;
    border-bottom: 1px solid #1f2937;
    padding: 10px 14px;
}

#ColumnTitle {
    font-weight: 700;
    font-size: 13px;
    color: #e2e8f0;
    letter-spacing: -0.2px;
}

#ColumnCount {
    background-color: #1f2937;
    color: #94a3b8;
    font-size: 11px;
    font-weight: 700;
    border-radius: 10px;
    padding: 2px 7px;
}

#JobCard {
    background-color: #1e293b;
    border: 1px solid #334155;
    border-radius: 10px;
    padding: 12px;
}

#JobCard:hover {
    border: 1px solid #818cf8;
    background-color: #1e293b;
}

#JobCard[dragging="true"] {
    opacity: 0.5;
    border: 1.5px dashed #818cf8;
    background-color: #0f172a;
}

#CardTitle {
    font-size: 14px;
    font-weight: 700;
    color: #f8fafc;
    line-height: 1.2;
}

#CardCompany {
    font-size: 12px;
    font-weight: 500;
    color: #94a3b8;
}

#TypeBadge {
    border-radius: 4px;
    padding: 2px 6px;
    font-size: 10px;
    font-weight: 700;
    letter-spacing: 0.5px;
}

#SalaryBadge {
    background-color: #064e3b;
    color: #6ee7b7;
    border: 1px solid #047857;
    border-radius: 4px;
    padding: 2px 6px;
    font-size: 11px;
    font-weight: 600;
}

#DateBadge {
    font-size: 11px;
    color: #64748b;
}

QPushButton.card-action-btn {
    background-color: transparent;
    border: none;
    border-radius: 4px;
    padding: 3px 6px;
    color: #94a3b8;
    font-size: 12px;
}

QPushButton.card-action-btn:hover {
    background-color: #334155;
    color: #f8fafc;
}

QPushButton.card-action-btn#deleteBtn:hover {
    background-color: #4c0519;
    color: #fb7185;
}

QPushButton.card-action-btn#folderBtn:hover {
    background-color: #312e81;
    color: #c7d2fe;
}

QScrollBar:vertical {
    border: none;
    background-color: transparent;
    width: 6px;
    margin: 2px 0;
}

QScrollBar::handle:vertical {
    background-color: #334155;
    min-height: 25px;
    border-radius: 3px;
}

QScrollBar::handle:vertical:hover {
    background-color: #475569;
}

QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {
    height: 0px;
}

QScrollBar:horizontal {
    border: none;
    background-color: transparent;
    height: 6px;
}

QScrollBar::handle:horizontal {
    background-color: #334155;
    min-width: 25px;
    border-radius: 3px;
}

QScrollBar::handle:horizontal:hover {
    background-color: #475569;
}

QScrollBar::add-line:horizontal, QScrollBar::sub-line:horizontal {
    width: 0px;
}

QDialog {
    background-color: #0f172a;
}

QLabel#DialogHeaderTitle {
    font-size: 16px;
    font-weight: 700;
    color: #f8fafc;
}

QLabel#FieldLabel {
    font-size: 12px;
    font-weight: 600;
    color: #cbd5e1;
}
)";

QString getStylesheet(bool darkMode) {
    return darkMode ? QString::fromUtf8(DARK_THEME) : QString::fromUtf8(LIGHT_THEME);
}

} // namespace Styles
