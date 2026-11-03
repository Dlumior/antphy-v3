# infra/terraform

| Carpeta | Contenido |
|---|---|
| `global/` | Bucket del estado remoto y rol de GitHub Actions (OIDC). Se aplica una sola vez. |
| `modules/` | Módulos propios. Implementados: `network`, `ecr`, `elasticache_redis`. Pendientes: `ecs_service`, `rds_postgres`, `alb`, `static_site`, `observability`. |
| `envs/` | Un estado por entorno (`dev`, `staging`, `prod`) con bloqueo nativo de S3. |

```bash
cd global && terraform init && terraform apply          # 1 vez
cd ../envs/dev
terraform init -backend-config="bucket=$(terraform -chdir=../../global output -raw tfstate_bucket)"
terraform plan
```

> Costos en `dev`: el NAT Gateway y ElastiCache cobran aunque no haya tráfico. Destruye el
> entorno (`terraform destroy`) cuando no lo uses.
