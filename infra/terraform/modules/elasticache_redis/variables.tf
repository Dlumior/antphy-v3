variable "name" { type = string }
variable "vpc_id" { type = string }
variable "subnet_ids" { type = list(string) }
variable "allowed_cidr" { type = string }

variable "node_type" {
  type    = string
  default = "cache.t4g.micro"
}

variable "replicas" {
  type    = number
  default = 0
}

variable "tags" {
  type    = map(string)
  default = {}
}
