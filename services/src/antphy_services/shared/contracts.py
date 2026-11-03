"""Acceso de solo lectura a los esquemas de contracts/ (fuente única de verdad)."""

import json
from functools import lru_cache
from pathlib import Path
from typing import Any

from jsonschema import Draft202012Validator

from antphy_services.shared.config import get_settings


@lru_cache
def schema_validator(name: str) -> Draft202012Validator:
    path: Path = get_settings().contracts_dir / "schemas" / f"{name}.schema.json"
    return Draft202012Validator(json.loads(path.read_text(encoding="utf-8")))


def validation_errors(name: str, document: Any) -> list[str]:
    """Devuelve los errores de validación (lista vacía si el documento es válido)."""
    validator = schema_validator(name)
    return [
        f"{'/'.join(map(str, e.absolute_path)) or '<raíz>'}: {e.message}"
        for e in validator.iter_errors(document)
    ]
