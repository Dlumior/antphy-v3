"""Invoca el ejecutable antphy-solve con el contrato JSON. Se reemplazará por los bindings
nanobind (fase 6) sin cambiar la interfaz Solver."""

import json
import subprocess
from pathlib import Path
from typing import Any


class SolverError(RuntimeError):
    pass


class CliSolver:
    def __init__(self, binary: Path, timeout_s: float) -> None:
        self._binary = binary
        self._timeout = timeout_s

    def solve(self, instance: dict[str, Any], solver: str) -> dict[str, Any]:
        if not self._binary.exists():
            raise SolverError(f"no se encontró el motor en {self._binary} (compila engine/)")
        result = subprocess.run(  # noqa: S603 - binario propio, argumentos fijos
            [str(self._binary), "--solver", solver, "-"],
            input=json.dumps(instance),
            capture_output=True,
            text=True,
            timeout=self._timeout,
            check=False,
        )
        if result.returncode != 0:
            raise SolverError(result.stderr.strip() or f"código de salida {result.returncode}")
        solution: dict[str, Any] = json.loads(result.stdout)
        return solution
