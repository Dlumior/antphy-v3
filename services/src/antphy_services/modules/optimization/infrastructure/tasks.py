"""Actores de Dramatiq. El worker los importa; la API solo los encola."""

import dramatiq

from antphy_services.modules.optimization.application.service import OptimizationService
from antphy_services.modules.optimization.infrastructure.wiring import cli_solver, job_repository
from antphy_services.shared.broker import broker  # noqa: F401 - configura el broker primero


@dramatiq.actor(queue_name="optimization", max_retries=0, time_limit=15 * 60 * 1000)
def solve_job(job_id: str) -> None:
    OptimizationService(job_repository(), solver=cli_solver()).run(job_id)


class DramatiqJobQueue:
    def enqueue(self, job_id: str) -> None:
        solve_job.send(job_id)
