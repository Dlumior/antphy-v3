# services

Backend en Python: **monolito modular** con dos puntos de entrada.

```
src/antphy_services/
├─ apps/
│  ├─ api/        FastAPI: valida contra contracts/, encola y consulta trabajos
│  └─ worker/     Dramatiq + Redis: ejecuta el motor C++
├─ modules/       contextos de negocio, independientes entre sí
│  ├─ optimization/   domain · application (puertos) · infrastructure · api
│  ├─ scenarios/      escenarios de urgencia (lectura de contracts/scenarios)
│  ├─ incidents/ fleet/ routing_matrix/ identity/   (pendientes)
└─ shared/        config, logging, broker, acceso a contracts
```

Las reglas de dependencia (módulos independientes, capas, nada importa `apps`) las comprueba
`uv run lint-imports` en el CI. Para separar un módulo como servicio: crear una app nueva que
solo lo cargue y cambiar el adaptador del puerto (en memoria → HTTP/cola).

## Desarrollo

```bash
uv sync
uv run pytest
uv run ruff check . && uv run mypy && uv run lint-imports
uv run uvicorn antphy_services.apps.api.main:app --reload       # API
uv run dramatiq antphy_services.apps.worker.main --processes 1   # worker
```

Variables (prefijo `ANTPHY_`): `REDIS_URL`, `SOLVER_BIN`, `CONTRACTS_DIR`, `ENV`, `CORS_ORIGINS`.
Ver [`.env.example`](.env.example).
