"""Configuration, constants, and color definitions for Jobbie."""

STATUSES = ["Wishlist", "Applied", "Interviewing", "Offered", "Rejected"]

JOB_TYPES = ["Full-time", "Part-time", "Contract", "Freelance", "Internship"]

STATUS_META = {
    "Wishlist": {
        "icon": "🎯",
        "label": "Wishlist",
        "light_color": "#6366f1",  # Indigo
        "light_bg": "#eef2ff",
        "light_border": "#c7d2fe",
        "dark_color": "#818cf8",
        "dark_bg": "#1e1b4b",
        "dark_border": "#312e81",
    },
    "Applied": {
        "icon": "⏱️",
        "label": "Applied",
        "light_color": "#0284c7",  # Sky / Blue
        "light_bg": "#f0f9ff",
        "light_border": "#bae6fd",
        "dark_color": "#38bdf8",
        "dark_bg": "#082f49",
        "dark_border": "#0369a1",
    },
    "Interviewing": {
        "icon": "💼",
        "label": "Interviewing",
        "light_color": "#d97706",  # Amber
        "light_bg": "#fffbeb",
        "light_border": "#fde68a",
        "dark_color": "#fbbf24",
        "dark_bg": "#451a03",
        "dark_border": "#78350f",
    },
    "Offered": {
        "icon": "🎉",
        "label": "Offered",
        "light_color": "#059669",  # Emerald
        "light_bg": "#ecfdf5",
        "light_border": "#a7f3d0",
        "dark_color": "#34d399",
        "dark_bg": "#064e3b",
        "dark_border": "#047857",
    },
    "Rejected": {
        "icon": "❌",
        "label": "Rejected",
        "light_color": "#e11d48",  # Rose
        "light_bg": "#fff1f2",
        "light_border": "#fecdd3",
        "dark_color": "#fb7185",
        "dark_bg": "#4c0519",
        "dark_border": "#881337",
    },
}

TYPE_COLORS = {
    "Full-time": {"bg_light": "#e0e7ff", "text_light": "#4338ca", "bg_dark": "#312e81", "text_dark": "#c7d2fe"},
    "Part-time": {"bg_light": "#f3e8ff", "text_light": "#7e22ce", "bg_dark": "#581c87", "text_dark": "#e9d5ff"},
    "Contract": {"bg_light": "#ffedd5", "text_light": "#c2410c", "bg_dark": "#7c2d12", "text_dark": "#fed7aa"},
    "Freelance": {"bg_light": "#fce7f3", "text_light": "#be185d", "bg_dark": "#831843", "text_dark": "#fbcfe8"},
    "Internship": {"bg_light": "#ccfbf1", "text_light": "#0f766e", "bg_dark": "#134e4a", "text_dark": "#99f6e4"},
}

DEFAULT_DATA_PATH = "jobs.json"
