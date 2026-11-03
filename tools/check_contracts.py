"""Valida los documentos de contracts/ y la salida real del motor contra los esquemas.

    uv run --project services python tools/check_contracts.py [ruta/antphy-solve]
"""

import json
import subprocess
import sys
from pathlib import Path

import yaml
from jsonschema import Draft202012Validator

ROOT = Path(__file__).resolve().parents[1]
SCHEMAS = ROOT / "contracts" / "schemas"


def validator(name: str) -> Draft202012Validator:
    schema = json.loads((SCHEMAS / f"{name}.schema.json").read_text(encoding="utf-8"))
    Draft202012Validator.check_schema(schema)
    return Draft202012Validator(schema)


def check(name: str, label: str, document: object) -> int:
    errors = list(validator(name).iter_errors(document))
    for e in errors:
        print(f"✗ {label}: {'/'.join(map(str, e.absolute_path))}: {e.message}")
    if not errors:
        print(f"✓ {label}")
    return len(errors)


def main() -> int:
    failures = 0
    for path in sorted((ROOT / "contracts" / "scenarios").glob("*.yaml")):
        failures += check("scenario", str(path.relative_to(ROOT)), yaml.safe_load(path.read_text()))
    for path in sorted((ROOT / "contracts" / "examples").glob("*.json")):
        failures += check("instance", str(path.relative_to(ROOT)), json.loads(path.read_text()))
        if len(sys.argv) > 1:
            out = subprocess.run([sys.argv[1], str(path)], capture_output=True, text=True, check=True)
            failures += check("solution", f"antphy-solve({path.name})", json.loads(out.stdout))
    return 1 if failures else 0


if __name__ == "__main__":
    sys.exit(main())
