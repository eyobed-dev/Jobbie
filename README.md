# Jobbie 💼
### Modern Kanban Job Application Tracker

Jobbie is a fast, lightweight, and modern **desktop application** built for job seekers who want a distraction-free workflow. It places the **Kanban board, your applications, and your CV folders at the center**, discarding wasteful full-page stats dashboards in favor of direct pipeline productivity.

---

## ✨ Features

- **100% Native Compiled C++**: Zero web engine, zero Electron, zero Python runtime needed. Built with Qt 6.10 + C++17, starting in milliseconds and consuming minimal RAM (~30MB).
- **Dedicated CV & Docs Folder Attachment**:
  - Attach a local computer directory (e.g. `/home/eyobed/Documents/Work/CVs/CompanyX`) to each job application.
  - One-click **📁** button directly on each Kanban card opens that folder in your native file manager (Nautilus, Dolphin, etc.) so you can instantly access your tailored CVs, cover letters, and notes.
  - In the Add/Edit dialog, easily **Browse**, **Open**, or **✨ Auto-create** the folder if it doesn't exist yet!
- **Kanban-First Workflow**: Full-viewport board across 5 pipeline stages:
  - 🎯 **Wishlist**
  - ⏱️ **Applied**
  - 💼 **Interviewing**
  - 🎉 **Offered**
  - ❌ **Rejected**
- **Native Drag-and-Drop**: Fluid drag-and-drop between columns with active drop zones and card drag previews.
- **Rich Application Cards**:
  - Role Title & Company name with work mode tags (Remote / Hybrid / On-site)
  - Color-coded job type badges (`Full-time`, `Contract`, `Internship`, etc.)
  - Salary range / compensation pills
  - Clickable direct link button (`🔗`) that launches your system's default browser
  - Applied date indicator & notes preview
  - One-click Edit (`✏️`) and Delete (`🗑️`) actions
- **Compact Command Bar**:
  - Live search across company names, roles, and notes
  - Instant job type filter chips (`All`, `Full-time`, `Contract`, etc.)
  - Minimalist metric pills right in the header bar (`Total`, `Active`, `Offers`)
- **Data Safety & Portability**:
  - Auto-saved locally to `jobs.json`
  - Built-in **Backup (Export)** and **Import** to JSON
- **Light & Dark Mode**: One-click theme toggle (`Ctrl+T`) with refined neutral styling and modern typography.

---

## 🚀 Building & Running (Compiled C++)

### Prerequisites
- CMake 3.16+
- Ninja (or Make)
- Qt 6 Widgets (`qt6-base-dev` or `qt6-base`)

### Build
From the `Jobbie` directory:
```bash
cmake -B build -GNinja
cmake --build build
```
This produces the native binary `./jobbie` directly in the project directory.

### Run
```bash
./jobbie
```

### Launch Options
- **Dark Mode**:
  ```bash
  ./jobbie --dark
  ```
- **Custom Data File**:
  ```bash
  ./jobbie --data /path/to/my_applications.json
  ```

---

## ⌨️ Keyboard Shortcuts

| Shortcut | Action |
| :--- | :--- |
| <kbd>Ctrl</kbd> + <kbd>N</kbd> | Add New Job Application |
| <kbd>Ctrl</kbd> + <kbd>F</kbd> | Focus Live Search Bar |
| <kbd>Ctrl</kbd> + <kbd>T</kbd> | Toggle Dark / Light Theme |
| <kbd>Ctrl</kbd> + <kbd>E</kbd> | Backup (Export to JSON) |
| <kbd>Ctrl</kbd> + <kbd>I</kbd> | Import Backup from JSON |
