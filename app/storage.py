"""Storage manager for Jobbie with JSON persistence and backup/export/import."""

import json
import os
from pathlib import Path
from typing import List, Optional
from datetime import datetime
from app.models import JobApplication


DEFAULT_SEED_JOBS = [
    {
        "id": "1",
        "company": "Linear",
        "title": "Senior Systems Engineer",
        "status": "Interviewing",
        "job_type": "Full-time",
        "salary": "$160k - $185k",
        "date_applied": datetime.now().strftime("%Y-%m-%d"),
        "link": "https://linear.app/careers",
        "notes": "Spoke with hiring manager. System architecture round scheduled for next Tuesday.",
        "location": "Remote",
    },
    {
        "id": "2",
        "company": "Vercel",
        "title": "Core Runtime Engineer",
        "status": "Applied",
        "job_type": "Full-time",
        "salary": "$170k - $200k",
        "date_applied": datetime.now().strftime("%Y-%m-%d"),
        "link": "https://vercel.com/careers",
        "notes": "Applied via referral link.",
        "location": "Remote",
    },
    {
        "id": "3",
        "company": "Stripe",
        "title": "Infrastructure Engineer",
        "status": "Offered",
        "job_type": "Full-time",
        "salary": "$190k + Equity",
        "date_applied": datetime.now().strftime("%Y-%m-%d"),
        "link": "https://stripe.com/jobs",
        "notes": "Offer letter received! Reviewing benefits and equity package.",
        "location": "Hybrid",
    },
    {
        "id": "4",
        "company": "Supabase",
        "title": "Backend / Database Dev",
        "status": "Wishlist",
        "job_type": "Contract",
        "salary": "$110/hr",
        "date_applied": "",
        "link": "https://supabase.com/careers",
        "notes": "Need to polish PostgreSQL & Rust portfolio before submitting.",
        "location": "Remote",
    },
    {
        "id": "5",
        "company": "Datadog",
        "title": "APM Platform Engineer",
        "status": "Rejected",
        "job_type": "Full-time",
        "salary": "$150k",
        "date_applied": "2024-01-10",
        "link": "https://www.datadoghq.com",
        "notes": "Role filled internally, keep in touch for Q3 pipeline.",
        "location": "On-site",
    },
]


class StorageManager:
    def __init__(self, file_path: Optional[str] = None):
        if file_path is None:
            # Save in current working directory or user's local share
            self.file_path = Path("jobs.json").resolve()
        else:
            self.file_path = Path(file_path).resolve()

    def load_jobs(self) -> List[JobApplication]:
        if not self.file_path.exists():
            # Create with default seed jobs
            jobs = [JobApplication.from_dict(j) for j in DEFAULT_SEED_JOBS]
            self.save_jobs(jobs)
            return jobs

        try:
            with open(self.file_path, "r", encoding="utf-8") as f:
                data = json.load(f)
                if isinstance(data, list):
                    return [JobApplication.from_dict(item) for item in data]
                return []
        except Exception as e:
            print(f"Error loading jobs from {self.file_path}: {e}")
            return [JobApplication.from_dict(j) for j in DEFAULT_SEED_JOBS]

    def save_jobs(self, jobs: List[JobApplication]) -> bool:
        try:
            self.file_path.parent.mkdir(parents=True, exist_ok=True)
            temp_path = self.file_path.with_suffix(".tmp")
            with open(temp_path, "w", encoding="utf-8") as f:
                json.dump([j.to_dict() for j in jobs], f, indent=2, ensure_ascii=False)
            temp_path.replace(self.file_path)
            return True
        except Exception as e:
            print(f"Error saving jobs to {self.file_path}: {e}")
            return False

    def export_to_file(self, export_path: str, jobs: List[JobApplication]) -> bool:
        try:
            with open(export_path, "w", encoding="utf-8") as f:
                json.dump([j.to_dict() for j in jobs], f, indent=2, ensure_ascii=False)
            return True
        except Exception as e:
            print(f"Error exporting jobs to {export_path}: {e}")
            return False

    def import_from_file(self, import_path: str) -> Optional[List[JobApplication]]:
        try:
            with open(import_path, "r", encoding="utf-8") as f:
                data = json.load(f)
                if isinstance(data, list):
                    return [JobApplication.from_dict(item) for item in data]
            return None
        except Exception as e:
            print(f"Error importing jobs from {import_path}: {e}")
            return None
