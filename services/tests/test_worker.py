from typing import Any

import pytest

from antphy_services.modules.optimization.application.service import OptimizationService
from antphy_services.modules.optimization.domain.job import Job, JobStatus
from antphy_services.modules.optimization.infrastructure.cli_solver import CliSolver
from antphy_services.modules.optimization.infrastructure.redis_repository import (
    RedisJobRepository,
)
from antphy_services.shared.config import get_settings


class FailingSolver:
    def solve(self, instance: dict[str, Any], solver: str) -> dict[str, Any]:
        raise RuntimeError("explotó")


def test_failed_solver_marks_job_as_failed(
    repository: RedisJobRepository, demo_instance: dict[str, Any]
) -> None:
    job = Job(instance=demo_instance)
    repository.save(job)
    result = OptimizationService(repository, solver=FailingSolver()).run(job.id)
    assert result.status is JobStatus.FAILED
    assert result.error == "explotó"


@pytest.mark.skipif(
    not get_settings().solver_bin.exists(), reason="motor no compilado (just engine-release)"
)
def test_run_job_with_real_engine(
    repository: RedisJobRepository, demo_instance: dict[str, Any]
) -> None:
    job = Job(instance=demo_instance)
    repository.save(job)
    settings = get_settings()
    result = OptimizationService(
        repository, solver=CliSolver(settings.solver_bin, settings.solver_timeout_s)
    ).run(job.id)
    assert result.status is JobStatus.SUCCEEDED
    assert result.solution is not None
    assert result.solution["feasible"] is True
    assert repository.get(job.id) == result
