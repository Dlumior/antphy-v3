# antphy-v3

Motor de ruteo para ayuda humanitaria basado en colonia de hormigas. Sirve para cualquier
desastre que requiera atención por urgencias: pandemias, terremotos, Fenómeno El Niño,
huaicos, etc. Cada tipo de desastre es un **escenario** configurable que define cómo se
calcula la urgencia de cada punto de atención.

> Sucesor de [antphy](https://github.com/Dlumior/antphy) (tesis PUCP, 2024).

## Estructura

| Carpeta                    | Contenido                                                                        | Tecnología                        |
| -------------------------- | -------------------------------------------------------------------------------- | --------------------------------- |
| [`engine/`](engine/)       | Núcleo del algoritmo (ACO MAX-MIN, modelo de urgencia, validación)               | C++23, CMake                      |
| [`services/`](services/)   | Backend: monolito modular, preparado para separarse en servicios                 | Python, FastAPI, Dramatiq + Redis |
| [`clients/`](clients/)     | Dashboard y librerías compartidas de los clientes                                | React, TypeScript, Vite+          |
| [`contracts/`](contracts/) | Esquemas compartidos entre componentes (instancia, escenario, solución, OpenAPI) | JSON Schema                       |
| [`infra/`](infra/)         | Infraestructura como código y entorno local                                      | Terraform (AWS), Docker Compose   |
| [`docs/`](docs/)           | Documentación de todo el proyecto                                                | Astro + Starlight                 |
| [`report/`](report/)       | Informe técnico sobre el algoritmo                                               | LaTeX                             |
| [`tools/`](tools/)         | Scripts de mantenimiento del repositorio                                         | —                                 |

Dependencias permitidas (en un solo sentido):

```
clients ──HTTP──▶ services/api ──Redis──▶ services/worker ──antphy-solve──▶ engine
   └──────────────────────────── contracts ◀──────────────────────────────────┘
```

## Requisitos

- C++: CMake ≥ 3.25, Ninja y GCC ≥ 13 (o Clang 18 con libc++; ver `engine/CMakePresets.json`)
- Python: [uv](https://docs.astral.sh/uv/) ≥ 0.5 (instala Python 3.12+ por ti)
- JS: Node ≥ 24 (ver `.node-version`) y pnpm (Vite+ descarga la versión fijada)
- Docker (para Redis, PostgreSQL y OSRM en local)
- Opcional: [just](https://github.com/casey/just), Terraform ≥ 1.10, TeX Live

## Inicio rápido

```bash
just setup           # instala dependencias de Python y JS
just engine          # compila y prueba el motor C++
just engine-release  # binario que usa el worker
just up              # levanta redis + postgres en Docker
just api             # API en http://localhost:8000/docs
just worker          # worker que procesa trabajos de optimización
just web             # dashboard en http://localhost:5173
just docs            # documentación en http://localhost:4321/antphy-v3
```

En el dashboard, **Resolver** encola la instancia de ejemplo de Tumbes y muestra las rutas.
`just ci` corre lo mismo que el CI. Sin `just`, cada receta del [`justfile`](justfile) es un comando normal que puedes copiar.

## Estado

Esqueleto funcional de punta a punta: el motor resuelve con una línea base (vecino más
cercano), la API encola, el worker ejecuta el motor y el dashboard muestra el resultado.
El ACO, el modelo de urgencia por escenario y el frente de Pareto llegan en las fases 1–5
(ver [docs](docs/src/content/docs/motor/estado.md)).

## Licencia

[MIT](LICENSE)
