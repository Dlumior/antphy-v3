# Comandos comunes de antphy-v3. Requiere https://github.com/casey/just
# `just` sin argumentos lista las recetas.

set dotenv-load := false

# La vp global solo instala dependencias; todo lo demás usa la del proyecto.
vp := "node_modules/.bin/vp"
compose := "docker compose -f infra/compose/compose.yaml"

default:
    @just --list

# Instala dependencias de Python y JS
setup:
    cd services && uv sync
    command -v vp >/dev/null || (cd /tmp && npm install -g vite-plus@1.4.0)
    vp install

# --- engine -----------------------------------------------------------------

# Compila y prueba el motor (Debug)
engine:
    cd engine && cmake --preset debug && cmake --build --preset debug && ctest --preset debug

# Compila el motor en Release (lo usa el worker en local)
engine-release:
    cd engine && cmake --preset release && cmake --build --preset release

# Pruebas del motor con ASan + UBSan
engine-sanitize:
    cd engine && cmake --preset sanitize && cmake --build --preset sanitize && ctest --preset sanitize

# Formatea el código C++
engine-fmt:
    find engine/include engine/src engine/cli engine/tests -name '*.?pp' | xargs clang-format -i

# --- services ---------------------------------------------------------------

# API en http://localhost:8000/docs
api:
    cd services && uv run uvicorn antphy_services.apps.api.main:app --reload

# Worker de Dramatiq
worker:
    cd services && uv run dramatiq antphy_services.apps.worker.main --processes 1 --threads 2

# Pruebas, lint, tipos y reglas de dependencia del backend
services-check:
    cd services && uv run ruff check . && uv run ruff format --check . && uv run mypy && uv run lint-imports && uv run pytest

# --- contracts --------------------------------------------------------------

# Exporta el OpenAPI y regenera el cliente TypeScript
openapi:
    cd services && ANTPHY_ENV=test uv run python -m antphy_services.apps.api.openapi ../contracts/openapi/openapi.json
    {{vp}} run @antphy/api-client#generate

# Valida contracts/ y la salida del motor
contracts: engine-release
    uv run --project services python tools/check_contracts.py engine/build/release/antphy-solve

# --- clients y docs ---------------------------------------------------------

# Dashboard en http://localhost:5173
web:
    {{vp}} run web#dev

# Documentación en http://localhost:4321/antphy-v3
docs:
    {{vp}} run docs#dev

# Formato, lint, tipos, pruebas y build de todo el workspace JS
js-check:
    {{vp}} check && {{vp}} run -r test && {{vp}} run -r build

# --- infra ------------------------------------------------------------------

# Levanta redis + postgres
up:
    {{compose}} up -d

# Levanta todo en contenedores (redis, postgres, api, worker)
up-app:
    {{compose}} --profile app up -d --build

# Levanta OSRM (antes: tools/osrm-prepare.sh)
osrm:
    {{compose}} --profile osrm up -d osrm

# Detiene los contenedores
down:
    {{compose}} --profile app --profile osrm down

# --- report -----------------------------------------------------------------

# Compila el informe LaTeX → report/build/main.pdf
report:
    cd report && latexmk

# --- todo -------------------------------------------------------------------

# Lo mismo que corre el CI
ci: engine services-check js-check contracts report
