import logging
from collections.abc import Callable
from typing import Any

from antphy_services.modules.optimization.application.ports import (
    InvalidInstanceError,
    JobQueue,
    JobRepository,
    Solver,
)
from antphy_services.modules.optimization.domain.job import Job

log = logging.getLogger(__name__)

Validator = Callable[[dict[str, Any]], list[str]]


class OptimizationService:
    def __init__(
        self,
        repository: JobRepository,
        queue: JobQueue | None = None,
        solver: Solver | None = None,
        validate_instance: Validator | None = None,
    ) -> None:
        self._repo = repository
        self._queue = queue
        self._solver = solver
        self._validate = validate_instance or (lambda _: [])

    def submit(self, instance: dict[str, Any], solver: str = "nearest_neighbor") -> Job:
        if errors := self._validate(instance):
            raise InvalidInstanceError(errors)
        if self._queue is None:
            raise RuntimeError("OptimizationService.submit requiere una cola")
        job = Job(instance=instance, solver=solver)
        self._repo.save(job)
        self._queue.enqueue(job.id)
        return job

    def get(self, job_id: str) -> Job | None:
        return self._repo.get(job_id)

    def run(self, job_id: str) -> Job:
        """Ejecuta un trabajo encolado. Lo llama el worker."""
        if self._solver is None:
            raise RuntimeError("OptimizationService.run requiere un solver")
        job = self._repo.get(job_id)
        if job is None:
            raise LookupError(f"trabajo {job_id} no existe")
        job = job.start()
        self._repo.save(job)
        try:
            job = job.succeed(self._solver.solve(job.instance, job.solver))
        except Exception as exc:
            log.exception("falló el trabajo %s", job_id)
            job = job.fail(str(exc))
        self._repo.save(job)
        return job
