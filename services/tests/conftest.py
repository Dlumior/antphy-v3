import json
import os
from collections.abc import Iterator
from pathlib import Path
from typing import Any

os.environ.setdefault("ANTPHY_ENV", "test")  # antes de importar la app: StubBroker

import fakeredis
import pytest
from fastapi.testclient import TestClient

from antphy_services.apps.api.main import create_app
from antphy_services.modules.optimization.api.router import get_service
from antphy_services.modules.optimization.application.service import OptimizationService
from antphy_services.modules.optimization.infrastructure.redis_repository import (
    RedisJobRepository,
)
from antphy_services.shared.config import get_settings
from antphy_services.shared.contracts import validation_errors


class FakeQueue:
    def __init__(self) -> None:
        self.sent: list[str] = []

    def enqueue(self, job_id: str) -> None:
        self.sent.append(job_id)


@pytest.fixture
def demo_instance() -> dict[str, Any]:
    path = get_settings().contracts_dir / "examples" / "tumbes-demo.json"
    data: dict[str, Any] = json.loads(Path(path).read_text(encoding="utf-8"))
    return data


@pytest.fixture
def repository() -> RedisJobRepository:
    return RedisJobRepository(fakeredis.FakeRedis(decode_responses=True), ttl_s=60)


@pytest.fixture
def queue() -> FakeQueue:
    return FakeQueue()


@pytest.fixture
def client(repository: RedisJobRepository, queue: FakeQueue) -> Iterator[TestClient]:
    app = create_app()
    app.dependency_overrides[get_service] = lambda: OptimizationService(
        repository, queue=queue, validate_instance=lambda d: validation_errors("instance", d)
    )
    with TestClient(app) as c:
        yield c
