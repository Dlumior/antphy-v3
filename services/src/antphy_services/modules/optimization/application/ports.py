"""Interfaces que la infraestructura debe implementar. Cambiar Redis, la cola o el modo
de invocar el motor (CLI hoy, bindings mañana) no toca los casos de uso."""

from typing import Any, Protocol

from antphy_services.modules.optimization.domain.job import Job


class JobRepository(Protocol):
    def save(self, job: Job) -> None: ...
    def get(self, job_id: str) -> Job | None: ...


class JobQueue(Protocol):
    def enqueue(self, job_id: str) -> None: ...


class Solver(Protocol):
    def solve(self, instance: dict[str, Any], solver: str) -> dict[str, Any]: ...


class InvalidInstanceError(ValueError):
    def __init__(self, errors: list[str]) -> None:
        super().__init__("; ".join(errors))
        self.errors = errors
