output "vpc_id" { value = module.network.vpc_id }
output "ecr_repositories" { value = module.ecr.repository_urls }
output "redis_url" { value = module.redis.url }
