---
title: Módulos del backend
description: Estructura del monolito modular.
---

| Módulo           | Responsabilidad                                 | Estado     |
| ---------------- | ----------------------------------------------- | ---------- |
| `optimization`   | Encolar y ejecutar trabajos, guardar soluciones | ✅         |
| `scenarios`      | Escenarios de urgencia                          | ✅ lectura |
| `incidents`      | Puntos afectados e indicadores                  | ⏳         |
| `fleet`          | Almacenes y vehículos                           | ⏳         |
| `routing_matrix` | Matrices OSRM con caché                         | ⏳         |
| `identity`       | Usuarios y permisos                             | ⏳         |

Cada módulo sigue `domain → application (puertos) → infrastructure / api`.
La referencia de la API está en `/docs` del servidor (Swagger) y en
`contracts/openapi/openapi.json`.
