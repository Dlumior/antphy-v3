"""Lectura de los escenarios de referencia. La edición llegará con la persistencia."""

from pathlib import Path
from typing import Any

import yaml
from fastapi import APIRouter, HTTPException

from antphy_services.shared.config import get_settings
from antphy_services.shared.contracts import validation_errors

router = APIRouter(prefix="/scenarios", tags=["scenarios"])


def _scenario_files() -> dict[str, Path]:
    folder = get_settings().contracts_dir / "scenarios"
    return {p.stem: p for p in sorted(folder.glob("*.yaml"))}


def _load(path: Path) -> dict[str, Any]:
    data: dict[str, Any] = yaml.safe_load(path.read_text(encoding="utf-8"))
    return data


@router.get("", summary="Listar escenarios disponibles")
def list_scenarios() -> list[dict[str, Any]]:
    return [
        {"scenario": s["scenario"], "version": s["version"], "description": s.get("description")}
        for s in map(_load, _scenario_files().values())
    ]


@router.get("/{name}", summary="Obtener un escenario")
def get_scenario(name: str) -> dict[str, Any]:
    path = _scenario_files().get(name)
    if path is None:
        raise HTTPException(status_code=404, detail=f"escenario '{name}' no existe")
    scenario = _load(path)
    if errors := validation_errors("scenario", scenario):
        raise HTTPException(status_code=500, detail={"invalid_scenario": errors})
    return scenario
