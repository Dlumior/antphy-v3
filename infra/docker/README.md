# infra/docker

- `services.Dockerfile`: imágenes `api` y `worker` (el worker incluye `antphy-solve`).
  Se construyen desde la raíz del repo con `--target api|worker`.
- OSRM usa la imagen oficial `ghcr.io/project-osrm/osrm-backend`; los datos se preparan con
  `tools/osrm-prepare.sh`.
- Web y docs no necesitan imagen: son estáticos (S3 + CloudFront).
