"""Application stylesheets for Light and Dark themes."""

from app.styles_data import LIGHT_THEME, DARK_THEME

def get_stylesheet(dark_mode: bool = False) -> str:
    return DARK_THEME if dark_mode else LIGHT_THEME
