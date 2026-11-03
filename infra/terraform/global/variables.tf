variable "region" {
  type    = string
  default = "us-east-1"
}

variable "github_repository" {
  description = "Repositorio autorizado a desplegar vía OIDC (owner/repo)."
  type        = string
  default     = "Dlumior/antphy-v3"
}
