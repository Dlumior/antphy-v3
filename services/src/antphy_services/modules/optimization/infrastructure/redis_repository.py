import json
from dataclasses import asdict
from datetime import datetime
from typing import Any

from redis import Redis

from antphy_services.modules.optimization.domain.job import Job, JobStatus

_PREFIX = "antphy:job:"


class RedisJobRepository:
    def __init__(self, client: Redis, ttl_s: int) -> None:
        self._redis = client
        self._ttl = ttl_s

    def save(self, job: Job) -> None:
        payload = asdict(job)
        payload["created_at"] = job.created_at.isoformat()
        payload["updated_at"] = job.updated_at.isoformat()
        self._redis.set(_PREFIX + job.id, json.dumps(payload), ex=self._ttl)

    def get(self, job_id: str) -> Job | None:
        raw = self._redis.get(_PREFIX + job_id)
        if raw is None:
            return None
        data: dict[str, Any] = json.loads(raw)
        data["status"] = JobStatus(data["status"])
        data["created_at"] = datetime.fromisoformat(data["created_at"])
        data["updated_at"] = datetime.fromisoformat(data["updated_at"])
        return Job(**data)
