---
title: Despliegue en AWS
description: Infraestructura con Terraform.
---

| Componente          | Servicio AWS                           |
| ------------------- | -------------------------------------- |
| API, worker, OSRM   | ECS Fargate (módulo `ecs_service`)     |
| Base de datos       | RDS PostgreSQL + PostGIS               |
| Cola y caché        | ElastiCache Redis                      |
| Web y docs          | S3 + CloudFront                        |
| Imágenes            | ECR                                    |
| Estado de Terraform | S3 con bloqueo nativo (`use_lockfile`) |

```bash
cd infra/terraform/envs/dev
terraform init && terraform plan
```

GitHub Actions se autentica con OIDC (sin llaves guardadas).
