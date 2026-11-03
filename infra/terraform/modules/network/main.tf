# VPC con subredes públicas y privadas en dos zonas, usando el módulo comunitario.
module "vpc" {
  source  = "terraform-aws-modules/vpc/aws"
  version = "~> 5.16"

  name = var.name
  cidr = var.cidr

  azs             = var.azs
  public_subnets  = [for i, _ in var.azs : cidrsubnet(var.cidr, 8, i)]
  private_subnets = [for i, _ in var.azs : cidrsubnet(var.cidr, 8, i + 10)]

  enable_nat_gateway = true
  single_nat_gateway = var.single_nat_gateway # true en dev: un NAT es el mayor costo fijo

  tags = var.tags
}
