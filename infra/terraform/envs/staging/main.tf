locals {
  name = "antphy-staging"
  tags = { Project = "antphy-v3", Environment = "staging", ManagedBy = "terraform" }
}

# Pendiente: misma composición que dev, con NAT por zona, réplicas de Redis y RDS multi-AZ.
