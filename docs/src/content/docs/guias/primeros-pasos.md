---
title: Primeros pasos
description: Levantar todo el proyecto en local.
sidebar:
  order: 1
---

## Requisitos

- CMake ≥ 3.25, Ninja y GCC ≥ 13 (o Clang con libc++)
- [uv](https://docs.astral.sh/uv/) para Python
- Node ≥ 24 y pnpm
- Docker (Redis y PostgreSQL)

## Pasos

```bash
just setup          # dependencias de Python y JS
just engine-release # compila el motor (lo usa el worker)
just up             # redis + postgres
just api            # http://localhost:8000/docs
just worker
just web            # http://localhost:5173
```

En el dashboard, pulsa **Resolver**: la API encola la instancia de ejemplo de Tumbes, el
worker ejecuta `antphy-solve` y la tabla muestra las rutas.
