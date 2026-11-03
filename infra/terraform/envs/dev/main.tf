locals {
  name = "antphy-dev"
  tags = { Project = "antphy-v3", Environment = "dev", ManagedBy = "terraform" }
}

module "network" {
  source             = "../../modules/network"
  name               = local.name
  azs                = ["${var.region}a", "${var.region}b"]
  single_nat_gateway = true
  tags               = local.tags
}

module "ecr" {
  source = "../../modules/ecr"
  prefix = "antphy"
  tags   = local.tags
}

module "redis" {
  source       = "../../modules/elasticache_redis"
  name         = local.name
  vpc_id       = module.network.vpc_id
  subnet_ids   = module.network.private_subnet_ids
  allowed_cidr = module.network.vpc_cidr
  tags         = local.tags
}

# Pendiente (fase 8): rds_postgres, ecs_service (api, worker, osrm), alb, static_site.
