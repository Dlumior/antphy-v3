locals {
  name = "antphy-prod"
  tags = { Project = "antphy-v3", Environment = "prod", ManagedBy = "terraform" }
}

# Pendiente: misma composición que dev, con NAT por zona, réplicas de Redis y RDS multi-AZ.
