"""Configuración por variables de entorno (prefijo ANTPHY_)."""

from functools import lru_cache
from pathlib import Path

from pydantic_settings import BaseSettings, SettingsConfigDict

_REPO_ROOT = Path(__file__).resolve().parents[4]


class Settings(BaseSettings):
    model_config = SettingsConfigDict(env_prefix="ANTPHY_", env_file=".env", extra="ignore")

    env: str = "local"
    redis_url: str = "redis://localhost:6379/0"
    contracts_dir: Path = _REPO_ROOT / "contracts"
    solver_bin: Path = _REPO_ROOT / "engine" / "build" / "release" / "antphy-solve"
    solver_timeout_s: float = 300.0
    job_ttl_s: int = 7 * 24 * 3600
    cors_origins: list[str] = ["http://localhost:5173"]


@lru_cache
def get_settings() -> Settings:
    return Settings()
