"""Exporta el contrato OpenAPI a contracts/openapi/openapi.json (just openapi)."""

import json
import sys
from pathlib import Path

from antphy_services.apps.api.main import app


def main() -> None:
    target = Path(sys.argv[1]) if len(sys.argv) > 1 else Path("../contracts/openapi/openapi.json")
    target.write_text(json.dumps(app.openapi(), indent=2, ensure_ascii=False) + "\n")
    print(f"OpenAPI escrito en {target}")


if __name__ == "__main__":
    main()
