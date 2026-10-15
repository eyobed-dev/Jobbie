"""Application configuration, stage metadata, and color themes."""

STATUSES = ["Wishlist", "Applied", "Interviewing", "Offered", "Rejected"]

JOB_TYPES = ["Full-time", "Part-time", "Contract", "Freelance", "Internship"]

STATUS_META = {
    "Wishlist": {
        "icon": "target",
        "label": "Wishlist",
        "light_color": "#6366f1",
        "dark_color": "#818cf8"
    },
    "Applied": {
        "icon": "send",
        "label": "Applied",
        "light_color": "#0284c7",
        "dark_color": "#38bdf8"
    },
    "Interviewing": {
        "icon": "briefcase",
        "label": "Interviewing",
        "light_color": "#d97706",
        "dark_color": "#fbbf24"
    },
    "Offered": {
        "icon": "award",
        "label": "Offered",
        "light_color": "#059669",
        "dark_color": "#34d399"
    },
    "Rejected": {
        "icon": "circle-x",
        "label": "Rejected",
        "light_color": "#e11d48",
        "dark_color": "#fb7185"
    }
}

TYPE_COLORS = {
    "Full-time": {
        "bg_light": "#eff6ff", "text_light": "#1d4ed8", "border_light": "#bfdbfe",
        "bg_dark": "#172554", "text_dark": "#93c5fd", "border_dark": "#1e40af"
    },
    "Part-time": {
        "bg_light": "#f5f3ff", "text_light": "#6d28d9", "border_light": "#ddd6fe",
        "bg_dark": "#2e1065", "text_dark": "#c4b5fd", "border_dark": "#5b21b6"
    },
    "Contract": {
        "bg_light": "#fffbeb", "text_light": "#b45309", "border_light": "#fde68a",
        "bg_dark": "#451a03", "text_dark": "#fcd34d", "border_dark": "#92400e"
    },
    "Freelance": {
        "bg_light": "#f0fdfa", "text_light": "#0f766e", "border_light": "#99f6e4",
        "bg_dark": "#042f2e", "text_dark": "#5eead4", "border_dark": "#115e59"
    },
    "Internship": {
        "bg_light": "#fdf4ff", "text_light": "#a21caf", "border_light": "#f5d0fe",
        "bg_dark": "#4a044e", "text_dark": "#f0abfc", "border_dark": "#86198f"
    }
}
