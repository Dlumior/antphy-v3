variable "prefix" { type = string }

variable "repositories" {
  type    = list(string)
  default = ["api", "worker"]
}

variable "keep_images" {
  type    = number
  default = 20
}

variable "tags" {
  type    = map(string)
  default = {}
}
