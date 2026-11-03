from dataclasses import dataclass, field, replace
from datetime import UTC, datetime
from enum import StrEnum
from typing import Any
from uuid import uuid4


class JobStatus(StrEnum):
    QUEUED = "queued"
    RUNNING = "running"
    SUCCEEDED = "succeeded"
    FAILED = "failed"


def _now() -> datetime:
    return datetime.now(UTC)


@dataclass(frozen=True, slots=True)
class Job:
    """Un trabajo de optimización. Inmutable: cada transición devuelve uno nuevo."""

    instance: dict[str, Any]
    solver: str = "nearest_neighbor"
    id: str = field(default_factory=lambda: uuid4().hex)
    status: JobStatus = JobStatus.QUEUED
    created_at: datetime = field(default_factory=_now)
    updated_at: datetime = field(default_factory=_now)
    solution: dict[str, Any] | None = None
    error: str | None = None

    def start(self) -> "Job":
        return replace(self, status=JobStatus.RUNNING, updated_at=_now())

    def succeed(self, solution: dict[str, Any]) -> "Job":
        return replace(self, status=JobStatus.SUCCEEDED, solution=solution, updated_at=_now())

    def fail(self, error: str) -> "Job":
        return replace(self, status=JobStatus.FAILED, error=error, updated_at=_now())
