# engine

Núcleo C++23 del algoritmo. No conoce la red, la base de datos ni Python: recibe una
instancia (matrices ya calculadas + urgencias) y devuelve una solución, ambas en el formato
de [`contracts/schemas`](../contracts/schemas).

## Compilar y probar

```bash
cmake --preset debug          # o release / sanitize
cmake --build --preset debug
ctest --preset debug
./build/debug/antphy-solve ../contracts/examples/tumbes-demo.json
```

## Estado

| Componente | Estado |
|---|---|
| Modelo (`Instance`, `SquareMatrix`, `NodeIndex`) | ✅ |
| Evaluación de objetivos y `validate()` | ✅ |
| Línea base: vecino más cercano | ✅ |
| E/S JSON + CLI `antphy-solve` | ✅ |
| Clarke-Wright, lector CVRPLIB | ⏳ fase 1 |
| Modelo de urgencia por escenario | ⏳ fase 2 |
| ACO MAX-MIN + búsqueda local + paralelismo | ⏳ fase 3 |
| Archivo de Pareto | ⏳ fase 4 |
| Bindings Python (nanobind) | ⏳ fase 6 — mientras tanto el backend usa el CLI |

## Convenciones

- C++23, sin `new`/`delete` manuales, `const` por defecto, `std::expected` para errores esperados.
- Toda solución que produce un solver debe pasar `antphy::validate()` en los tests.
- Formato con `.clang-format` de la raíz; análisis con `.clang-tidy`.
