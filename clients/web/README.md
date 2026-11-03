# clients/web

Dashboard en React + TypeScript con Vite+. Por ahora: estado de la API, escenarios
disponibles y resolución de la instancia de ejemplo. Pendiente (fase 7): mapa con MapLibre,
editor de escenarios, pesos de los objetivos y frente de Pareto.

```bash
pnpm exec vp run web#dev     # http://localhost:5173 (proxy /api → localhost:8000)
pnpm exec vp run web#build
```

Variables: `VITE_API_URL` (URL de la API en producción), `ANTPHY_API_URL` (destino del proxy
en desarrollo).
