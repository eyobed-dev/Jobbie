"""Data models for Jobbie."""

from dataclasses import dataclass, asdict, field
from datetime import datetime
import uuid
from typing import Dict, Any, Optional


@dataclass
class JobApplication:
    application_number: int = 0
    id: str = field(default_factory=lambda: str(uuid.uuid4()))
    company: str = ""
    title: str = ""
    status: str = "Wishlist"
    job_type: str = "Full-time"
    salary: str = ""
    date_applied: str = field(default_factory=lambda: datetime.now().strftime("%Y-%m-%d"))
    link: str = ""
    folder_path: str = ""
    notes: str = ""
    location: str = "Remote"
    created_at: str = field(default_factory=lambda: datetime.now().isoformat())
    updated_at: str = field(default_factory=lambda: datetime.now().isoformat())

    def to_dict(self) -> Dict[str, Any]:
        return asdict(self)

    @classmethod
    def from_dict(cls, data: Dict[str, Any]) -> "JobApplication":
        # Handle backward compatibility / variations in key names
        job_id = str(data.get("id") or uuid.uuid4())
        company = data.get("company", "").strip()
        title = data.get("title", "").strip()
        status = data.get("status", "Wishlist")
        job_type = data.get("type") or data.get("job_type", "Full-time")
        salary = data.get("salary", "")
        date_applied = data.get("dateApplied") or data.get("date_applied") or datetime.now().strftime("%Y-%m-%d")
        link = data.get("link", "")
        folder_path = data.get("folder_path") or data.get("folderPath") or ""
        if "/Documents/Work/CVs/" in folder_path:
            folder_path = folder_path.replace("/Documents/Work/CVs/", "/Documents/Work/Applications/")
        app_num = int(data.get("application_number") or data.get("applicationNumber") or 0)
        if app_num <= 0 and folder_path:
            import os, re
            base = os.path.basename(folder_path.rstrip("/ "))
            m = re.match(r"^(\d+)", base)
            if m:
                app_num = int(m.group(1))

        notes = data.get("notes", "")
        location = data.get("location", "Remote")
        created_at = data.get("createdAt") or data.get("created_at") or datetime.now().isoformat()
        updated_at = data.get("updatedAt") or data.get("updated_at") or datetime.now().isoformat()

        return cls(
            application_number=app_num,
            id=job_id,
            company=company,
            title=title,
            status=status,
            job_type=job_type,
            salary=salary,
            date_applied=date_applied,
            link=link,
            folder_path=folder_path,
            notes=notes,
            location=location,
            created_at=created_at,
            updated_at=updated_at,
        )
