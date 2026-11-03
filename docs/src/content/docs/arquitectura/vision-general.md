---
title: Visión general
description: Componentes y reglas de dependencia.
sidebar:
  order: 1
---

```
clients/web ──HTTP──▶ services/api ──Redis──▶ services/worker ──▶ engine (antphy-solve)
                              │                      │
                              └────── contracts/ ────┘  (esquemas compartidos)
```

- **engine** no conoce la red, la base de datos ni Python.
- **services** es un monolito modular: los módulos no se importan entre sí
  (`lint-imports` lo verifica), así que cualquiera puede separarse como servicio.
- **clients** solo conoce la API, a través del cliente generado desde OpenAPI.
- **contracts** es la frontera: un cambio de esquema rompe el CI de todos los lados.
