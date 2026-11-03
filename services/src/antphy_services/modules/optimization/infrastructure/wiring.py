"""Construcción de dependencias reales a partir de la configuración."""

from functools import lru_cache

from redis import Redis

from antphy_services.modules.optimization.infrastructure.cli_solver import CliSolver
from antphy_services.modules.optimization.infrastructure.redis_repository import (
    RedisJobRepository,
)
from antphy_services.shared.config import get_settings


@lru_cache
def redis_client() -> Redis:
    return Redis.from_url(get_settings().redis_url, decode_responses=True)


def job_repository() -> RedisJobRepository:
    return RedisJobRepository(redis_client(), get_settings().job_ttl_s)


def cli_solver() -> CliSolver:
    settings = get_settings()
    return CliSolver(settings.solver_bin, settings.solver_timeout_s)
