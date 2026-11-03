from datetime import datetime
from typing import Annotated, Any

from fastapi import APIRouter, Depends, HTTPException, status
from pydantic import BaseModel, Field

from antphy_services.modules.optimization.application.ports import InvalidInstanceError
from antphy_services.modules.optimization.application.service import OptimizationService
from antphy_services.modules.optimization.domain.job import Job, JobStatus
from antphy_services.modules.optimization.infrastructure.tasks import DramatiqJobQueue
from antphy_services.modules.optimization.infrastructure.wiring import job_repository
from antphy_services.shared.contracts import validation_errors

router = APIRouter(prefix="/optimization", tags=["optimization"])


def get_service() -> OptimizationService:
    return OptimizationService(
        job_repository(),
        queue=DramatiqJobQueue(),
        validate_instance=lambda doc: validation_errors("instance", doc),
    )


Service = Annotated[OptimizationService, Depends(get_service)]


class SubmitJobRequest(BaseModel):
    instance: dict[str, Any] = Field(description="Instancia según contracts/schemas/instance")
    solver: str = "nearest_neighbor"


class JobResponse(BaseModel):
    id: str
    status: JobStatus
    solver: str
    created_at: datetime
    updated_at: datetime
    solution: dict[str, Any] | None = None
    error: str | None = None

    @classmethod
    def from_job(cls, job: Job) -> "JobResponse":
        return cls(
            id=job.id,
            status=job.status,
            solver=job.solver,
            created_at=job.created_at,
            updated_at=job.updated_at,
            solution=job.solution,
            error=job.error,
        )


@router.post(
    "/jobs",
    status_code=status.HTTP_202_ACCEPTED,
    summary="Encolar una instancia para resolver",
)
def submit_job(body: SubmitJobRequest, service: Service) -> JobResponse:
    try:
        job = service.submit(body.instance, body.solver)
    except InvalidInstanceError as exc:
        raise HTTPException(status_code=422, detail={"invalid_instance": exc.errors}) from exc
    return JobResponse.from_job(job)


@router.get("/jobs/{job_id}", summary="Consultar el estado y la solución de un trabajo")
def get_job(job_id: str, service: Service) -> JobResponse:
    job = service.get(job_id)
    if job is None:
        raise HTTPException(status_code=404, detail="trabajo no encontrado")
    return JobResponse.from_job(job)
