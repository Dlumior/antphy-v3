# contracts

Fuente única de verdad entre `engine`, `services` y `clients`. Si cambias un esquema, sube
`schema_version`/`version` y actualiza los tres lados en el mismo PR (el CI lo comprueba).

- `schemas/`: JSON Schema de la instancia (entrada del motor), la solución (salida) y el escenario de urgencia.
- `scenarios/`: escenarios de referencia. Los pesos son decisiones de política: deben revisarlos especialistas del área.
- `openapi/`: contrato HTTP de la API. Se genera desde `services` con `just openapi` y de él sale `clients/packages/api-client`.
- `examples/`: instancias pequeñas para pruebas y demos.
