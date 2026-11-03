# clients

Todo lo que consume la API, en un workspace JS gestionado con [Vite+](https://viteplus.dev)
(`vp`): Vite 8, Vitest, Oxlint, Oxfmt y el ejecutor de tareas con caché.

| Carpeta                | Contenido                                          |
| ---------------------- | -------------------------------------------------- |
| `web/`                 | Dashboard React + TypeScript                       |
| `packages/api-client/` | Cliente tipado, generado desde `contracts/openapi` |
| `packages/ui/`         | Componentes compartidos (pendiente)                |

```bash
pnpm exec vp check        # formato + lint + tipos (raíz del repo)
pnpm exec vp run -r test
pnpm exec vp run -r build
```
