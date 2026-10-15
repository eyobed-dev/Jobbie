#include "styles.h"

namespace Styles {

static const char* LIGHT_THEME = R"(
/* Level 0: Main Window Workspace Canvas */
QMainWindow, QWidget#CentralCanvas, KanbanBoardWidget, #KanbanBoard, QWidget {
    font-family: -apple-system, BlinkMacSystemFont, "Inter", "Segoe UI", "Noto Sans", sans-serif;
    font-size: 10pt;
    color: #09090b;
    background-color: #fafafa;
}

QLabel {
    background-color: transparent;
}

/* Header Bar: Crisp clean surface with subtle border */
#HeaderBar {
    background-color: #ffffff;
    border-bottom: 1px solid #e4e4e7;
    padding: 12px 20px;
}

#AppTitle {
    font-size: 13pt;
    font-weight: 700;
    color: #09090b;
    letter-spacing: -0.3px;
}

/* Inset Search and Dialog inputs */
QLineEdit, QDateEdit, QComboBox, QTextEdit {
    background-color: #ffffff;
    border: 1px solid #e4e4e7;
    border-radius: 6px;
    padding: 7px 12px;
    font-size: 10pt;
    color: #09090b;
    selection-background-color: #09090b;
    selection-color: #ffffff;
}

QLineEdit:focus, QDateEdit:focus, QComboBox:focus, QTextEdit:focus {
    border: 1.5px solid #09090b;
    background-color: #ffffff;
}

QLineEdit::placeholder {
    color: #a1a1aa;
}

/* Header Action Buttons (Shadcn Outline Style) */
QPushButton {
    background-color: #ffffff;
    border: 1px solid #e4e4e7;
    border-radius: 6px;
    padding: 6px 14px;
    font-size: 10pt;
    font-weight: 500;
    color: #09090b;
}

QPushButton:hover {
    background-color: #f4f4f5;
    border-color: #d4d4d8;
    color: #09090b;
}

QPushButton:pressed {
    background-color: #e4e4e7;
}

/* Primary "Add Job" Button (Solid Black in Light Mode) */
QPushButton#PrimaryButton {
    background-color: #09090b;
    color: #fafafa;
    border: 1px solid #09090b;
    padding: 7px 16px;
    border-radius: 6px;
    font-size: 10pt;
    font-weight: 600;
}

QPushButton#PrimaryButton:hover {
    background-color: #27272a;
    border-color: #27272a;
    color: #fafafa;
}

QPushButton#PrimaryButton:pressed {
    background-color: #3f3f46;
}

/* Danger Button */
QPushButton#DangerButton {
    background-color: #09090b;
    color: #fafafa;
    border: 1px solid #09090b;
    border-radius: 6px;
    padding: 7px 16px;
    font-size: 10pt;
    font-weight: 600;
}

QPushButton#DangerButton:hover {
    background-color: #ef4444;
    border-color: #ef4444;
    color: #ffffff;
}

/* Filter Chips (Shadcn Segmented / Tabs Style) */
QPushButton.filter-chip {
    background-color: transparent;
    border: 1px solid transparent;
    border-radius: 6px;
    padding: 5px 12px;
    font-size: 9.5pt;
    font-weight: 500;
    color: #71717a;
}

QPushButton.filter-chip:hover {
    background-color: #f4f4f5;
    border-color: #f4f4f5;
    color: #09090b;
}

QPushButton.filter-chip:checked {
    background-color: #09090b;
    color: #fafafa;
    border: 1px solid #09090b;
    font-weight: 600;
}

/* Header Stat Metric Badges */
QLabel.metric-badge {
    background-color: #f4f4f5;
    border: 1px solid #e4e4e7;
    border-radius: 6px;
    padding: 4px 10px;
    font-size: 9pt;
    font-weight: 600;
    color: #71717a;
}

QLabel#BadgeActive {
    background-color: #f0f9ff;
    border: 1px solid #bae6fd;
    color: #0284c7;
}

QLabel#BadgeOffers {
    background-color: #ecfdf5;
    border: 1px solid #a7f3d0;
    color: #059669;
}

/* Transparent Scoped Scroll Areas */
QScrollArea#BoardScrollArea,
QScrollArea#ColumnScrollArea,
QWidget#BoardContent,
QWidget#ColumnCardsContainer {
    background: transparent;
    border: none;
}

/* Level 1: Kanban Column Trays */
KanbanColumnWidget, #KanbanColumn {
    background-color: #f4f4f5;
    border: 1px solid #e4e4e7;
    border-radius: 8px;
}

#KanbanColumn[dragOver="true"] {
    background-color: #ebebee;
    border: 1.5px dashed #09090b;
}

#ColumnHeader {
    background-color: transparent;
    border-bottom: 1px solid #e4e4e7;
    padding: 10px 14px;
}

#ColumnTitle {
    font-weight: 600;
    font-size: 10.5pt;
    color: #09090b;
    letter-spacing: -0.2px;
}

#ColumnCount {
    background-color: #ffffff;
    border: 1px solid #e4e4e7;
    color: #71717a;
    font-size: 9pt;
    font-weight: 600;
    border-radius: 10px;
    padding: 2px 7px;
}

QPushButton#ColumnAddBtn {
    background-color: #ffffff;
    border: 1px solid #e4e4e7;
    border-radius: 5px;
    color: #71717a;
    padding: 0px;
}

QPushButton#ColumnAddBtn:hover {
    background-color: #e4e4e7;
    border-color: #d4d4d8;
    color: #09090b;
}

/* Level 2: Job Cards */
JobCardWidget, #JobCard {
    background-color: #ffffff;
    border: 1px solid #e4e4e7;
    border-radius: 8px;
    padding: 14px;
}

#JobCard:hover {
    border: 1px solid #a1a1aa;
    background-color: #ffffff;
}

#JobCard[dragging="true"] {
    opacity: 0.5;
    border: 1.5px dashed #09090b;
    background-color: #f4f4f5;
}

#CardTitle {
    font-size: 11pt;
    font-weight: 600;
    color: #09090b;
    line-height: 1.35;
}

#CardCompany {
    font-size: 9.5pt;
    font-weight: 500;
    color: #71717a;
}

#CardLocation {
    font-size: 9.5pt;
    font-weight: 400;
    color: #a1a1aa;
}

#CardNotes {
    background-color: #fffbeb;
    border: 1px solid #fef3c7;
    border-left: 2.5px solid #f59e0b;
    border-radius: 4px;
    padding: 5px 8px;
}

#CardNotesText {
    font-size: 9pt;
    color: #92400e;
    font-style: normal;
}

#SalaryBadge {
    background-color: #ecfdf5;
    border: 1px solid #a7f3d0;
    color: #047857;
    border-radius: 4px;
    padding: 3px 7px;
    font-size: 9pt;
    font-weight: 600;
}

#DateBadge {
    font-size: 9pt;
    font-weight: 500;
    color: #71717a;
}

/* Card Action Icon Buttons (Shadcn Ghost Style) */
QPushButton.card-action-btn {
    background-color: transparent;
    border: 1px solid transparent;
    border-radius: 5px;
    padding: 0px;
    color: #71717a;
}

QPushButton.card-action-btn:hover {
    background-color: #f4f4f5;
    border-color: #e4e4e7;
    color: #09090b;
}

QPushButton.card-action-btn#folderBtn:hover {
    background-color: #eef2ff;
    border-color: #c7d2fe;
    color: #6366f1;
}

QPushButton.card-action-btn#linkBtn:hover {
    background-color: #f0f9ff;
    border-color: #bae6fd;
    color: #0284c7;
}

QPushButton.card-action-btn#deleteBtn:hover {
    background-color: #fee2e2;
    border-color: #fca5a5;
    color: #ef4444;
}

/* Scrollbars */
QScrollBar:vertical {
    border: none;
    background-color: transparent;
    width: 6px;
    margin: 2px 0;
}

QScrollBar::handle:vertical {
    background-color: #d4d4d8;
    min-height: 25px;
    border-radius: 3px;
}

QScrollBar::handle:vertical:hover {
    background-color: #a1a1aa;
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
    background-color: #d4d4d8;
    min-width: 25px;
    border-radius: 3px;
}

QScrollBar::handle:horizontal:hover {
    background-color: #a1a1aa;
}

QScrollBar::add-line:horizontal, QScrollBar::sub-line:horizontal {
    width: 0px;
}

/* Dialog Styling */
QDialog {
    background-color: #ffffff;
}

QLabel#DialogHeaderTitle {
    font-size: 13pt;
    font-weight: 600;
    color: #09090b;
}

QLabel#FieldLabel {
    font-size: 9.5pt;
    font-weight: 500;
    color: #71717a;
}
)";

static const char* DARK_THEME = R"(
/* Level 0: Main Window Workspace Canvas */
QMainWindow, QWidget#CentralCanvas, KanbanBoardWidget, #KanbanBoard, QWidget {
    font-family: -apple-system, BlinkMacSystemFont, "Inter", "Segoe UI", "Noto Sans", sans-serif;
    font-size: 10pt;
    color: #fafafa;
    background-color: #09090b;
}

QLabel {
    background-color: transparent;
}

/* Header Bar: Crisp dark surface with subtle zinc border */
#HeaderBar {
    background-color: #09090b;
    border-bottom: 1px solid #27272a;
    padding: 12px 20px;
}

#AppTitle {
    font-size: 13pt;
    font-weight: 700;
    color: #fafafa;
    letter-spacing: -0.3px;
}

/* Inset Search and Dialog inputs */
QLineEdit, QDateEdit, QComboBox, QTextEdit {
    background-color: #09090b;
    border: 1px solid #27272a;
    border-radius: 6px;
    padding: 7px 12px;
    font-size: 10pt;
    color: #fafafa;
    selection-background-color: #fafafa;
    selection-color: #09090b;
}

QLineEdit:focus, QDateEdit:focus, QComboBox:focus, QTextEdit:focus {
    border: 1.5px solid #fafafa;
    background-color: #09090b;
}

QLineEdit::placeholder {
    color: #52525b;
}

/* Header Action Buttons (Shadcn Outline Style) */
QPushButton {
    background-color: #09090b;
    border: 1px solid #27272a;
    border-radius: 6px;
    padding: 6px 14px;
    font-size: 10pt;
    font-weight: 500;
    color: #fafafa;
}

QPushButton:hover {
    background-color: #27272a;
    border-color: #3f3f46;
    color: #ffffff;
}

QPushButton:pressed {
    background-color: #3f3f46;
}

/* Primary "Add Job" Button (Solid White in Dark Mode) */
QPushButton#PrimaryButton {
    background-color: #fafafa;
    color: #09090b;
    border: 1px solid #fafafa;
    padding: 7px 16px;
    border-radius: 6px;
    font-size: 10pt;
    font-weight: 600;
}

QPushButton#PrimaryButton:hover {
    background-color: #e4e4e7;
    border-color: #e4e4e7;
    color: #09090b;
}

QPushButton#PrimaryButton:pressed {
    background-color: #d4d4d8;
}

/* Danger Button */
QPushButton#DangerButton {
    background-color: #fafafa;
    color: #09090b;
    border: 1px solid #fafafa;
    border-radius: 6px;
    padding: 7px 16px;
    font-size: 10pt;
    font-weight: 600;
}

QPushButton#DangerButton:hover {
    background-color: #ef4444;
    border-color: #ef4444;
    color: #ffffff;
}

/* Filter Chips (Shadcn Segmented / Tabs Style) */
QPushButton.filter-chip {
    background-color: transparent;
    border: 1px solid transparent;
    border-radius: 6px;
    padding: 5px 12px;
    font-size: 9.5pt;
    font-weight: 500;
    color: #a1a1aa;
}

QPushButton.filter-chip:hover {
    background-color: #27272a;
    border-color: #27272a;
    color: #fafafa;
}

QPushButton.filter-chip:checked {
    background-color: #fafafa;
    color: #09090b;
    border: 1px solid #fafafa;
    font-weight: 600;
}

/* Header Stat Metric Badges */
QLabel.metric-badge {
    background-color: #18181b;
    border: 1px solid #27272a;
    border-radius: 6px;
    padding: 4px 10px;
    font-size: 9pt;
    font-weight: 600;
    color: #a1a1aa;
}

QLabel#BadgeActive {
    background-color: #082f49;
    border: 1px solid #0369a1;
    color: #38bdf8;
}

QLabel#BadgeOffers {
    background-color: #064e3b;
    border: 1px solid #065f46;
    color: #34d399;
}

/* Transparent Scoped Scroll Areas */
QScrollArea#BoardScrollArea,
QScrollArea#ColumnScrollArea,
QWidget#BoardContent,
QWidget#ColumnCardsContainer {
    background: transparent;
    border: none;
}

/* Level 1: Kanban Column Trays */
KanbanColumnWidget, #KanbanColumn {
    background-color: #121215;
    border: 1px solid #27272a;
    border-radius: 8px;
}

#KanbanColumn[dragOver="true"] {
    background-color: #1c1c21;
    border: 1.5px dashed #fafafa;
}

#ColumnHeader {
    background-color: transparent;
    border-bottom: 1px solid #27272a;
    padding: 10px 14px;
}

#ColumnTitle {
    font-weight: 600;
    font-size: 10.5pt;
    color: #fafafa;
    letter-spacing: -0.2px;
}

#ColumnCount {
    background-color: #18181b;
    border: 1px solid #27272a;
    color: #a1a1aa;
    font-size: 9pt;
    font-weight: 600;
    border-radius: 10px;
    padding: 2px 7px;
}

QPushButton#ColumnAddBtn {
    background-color: #18181b;
    border: 1px solid #27272a;
    border-radius: 5px;
    color: #a1a1aa;
    padding: 0px;
}

QPushButton#ColumnAddBtn:hover {
    background-color: #27272a;
    border-color: #3f3f46;
    color: #fafafa;
}

/* Level 2: Job Cards */
JobCardWidget, #JobCard {
    background-color: #18181b;
    border: 1px solid #27272a;
    border-radius: 8px;
    padding: 14px;
}

#JobCard:hover {
    border: 1px solid #52525b;
    background-color: #18181b;
}

#JobCard[dragging="true"] {
    opacity: 0.5;
    border: 1.5px dashed #fafafa;
    background-color: #121215;
}

#CardTitle {
    font-size: 11pt;
    font-weight: 600;
    color: #fafafa;
    line-height: 1.35;
}

#CardCompany {
    font-size: 9.5pt;
    font-weight: 500;
    color: #a1a1aa;
}

#CardLocation {
    font-size: 9.5pt;
    font-weight: 400;
    color: #71717a;
}

#CardNotes {
    background-color: #291e0a;
    border: 1px solid #453310;
    border-left: 2.5px solid #fbbf24;
    border-radius: 4px;
    padding: 5px 8px;
}

#CardNotesText {
    font-size: 9pt;
    color: #fde68a;
    font-style: normal;
}

#SalaryBadge {
    background-color: #064e3b;
    border: 1px solid #065f46;
    color: #6ee7b7;
    border-radius: 4px;
    padding: 3px 7px;
    font-size: 9pt;
    font-weight: 600;
}

#DateBadge {
    font-size: 9pt;
    font-weight: 500;
    color: #a1a1aa;
}

/* Card Action Icon Buttons (Shadcn Ghost Style) */
QPushButton.card-action-btn {
    background-color: transparent;
    border: 1px solid transparent;
    border-radius: 5px;
    padding: 0px;
    color: #a1a1aa;
}

QPushButton.card-action-btn:hover {
    background-color: #27272a;
    border-color: #3f3f46;
    color: #fafafa;
}

QPushButton.card-action-btn#folderBtn:hover {
    background-color: #1e1b4b;
    border-color: #3730a3;
    color: #818cf8;
}

QPushButton.card-action-btn#linkBtn:hover {
    background-color: #082f49;
    border-color: #0369a1;
    color: #38bdf8;
}

QPushButton.card-action-btn#deleteBtn:hover {
    background-color: #450a0a;
    border-color: #7f1d1d;
    color: #ef4444;
}

/* Scrollbars */
QScrollBar:vertical {
    border: none;
    background-color: transparent;
    width: 6px;
    margin: 2px 0;
}

QScrollBar::handle:vertical {
    background-color: #27272a;
    min-height: 25px;
    border-radius: 3px;
}

QScrollBar::handle:vertical:hover {
    background-color: #3f3f46;
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
    background-color: #27272a;
    min-width: 25px;
    border-radius: 3px;
}

QScrollBar::handle:horizontal:hover {
    background-color: #3f3f46;
}

QScrollBar::add-line:horizontal, QScrollBar::sub-line:horizontal {
    width: 0px;
}

/* Dialog Styling */
QDialog {
    background-color: #09090b;
}

QLabel#DialogHeaderTitle {
    font-size: 13pt;
    font-weight: 600;
    color: #fafafa;
}

QLabel#FieldLabel {
    font-size: 9.5pt;
    font-weight: 500;
    color: #a1a1aa;
}
)";

QString getStylesheet(bool darkMode) {
    return QString::fromUtf8(darkMode ? DARK_THEME : LIGHT_THEME);
}

} // namespace Styles
