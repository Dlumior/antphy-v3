# infra

| Carpeta | Contenido |
|---|---|
| `terraform/` | AWS: `global/` (estado remoto + OIDC), `modules/`, `envs/{dev,staging,prod}` |
| `docker/` | Imágenes `api` y `worker` (el worker incluye el motor) |
| `compose/` | Entorno local: Redis, PostgreSQL/PostGIS y, con perfiles, api, worker y OSRM |
