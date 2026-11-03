terraform {
  required_version = ">= 1.10"
  required_providers {
    aws = { source = "hashicorp/aws", version = "~> 5.80" }
  }
  # El estado de "global" es local la primera vez (crea el bucket); luego puede migrarse.
}

provider "aws" {
  region = var.region
  default_tags { tags = { Project = "antphy-v3", ManagedBy = "terraform", Stack = "global" } }
}
