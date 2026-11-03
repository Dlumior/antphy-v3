# syntax=docker/dockerfile:1.7
# Imágenes del backend. Contexto de construcción: raíz del repositorio.
#   docker build -f infra/docker/services.Dockerfile --target api    -t antphy-api .
#   docker build -f infra/docker/services.Dockerfile --target worker -t antphy-worker .

# --- 1. Motor C++ ---------------------------------------------------------------
FROM ubuntu:24.04 AS engine-build
RUN apt-get update && apt-get install -y --no-install-recommends \
        build-essential cmake ninja-build git ca-certificates \
    && rm -rf /var/lib/apt/lists/*
WORKDIR /src
COPY engine/ engine/
COPY contracts/ contracts/
RUN cmake -S engine -B build -G Ninja -DCMAKE_BUILD_TYPE=Release -DANTPHY_BUILD_TESTS=OFF \
    && cmake --build build --target antphy-solve

# --- 2. Base Python ---------------------------------------------------------------
FROM python:3.12-slim AS python-base
COPY --from=ghcr.io/astral-sh/uv:0.5 /uv /usr/local/bin/uv
ENV UV_COMPILE_BYTECODE=1 UV_LINK_MODE=copy PYTHONUNBUFFERED=1 \
    ANTPHY_CONTRACTS_DIR=/app/contracts
WORKDIR /app/services
COPY services/pyproject.toml services/uv.lock ./
RUN uv sync --frozen --no-dev --no-install-project
COPY services/ ./
COPY contracts/ /app/contracts/
RUN uv sync --frozen --no-dev
RUN useradd --system --uid 10001 antphy
USER antphy
ENV PATH="/app/services/.venv/bin:$PATH"

# --- 3a. API ----------------------------------------------------------------------
FROM python-base AS api
EXPOSE 8000
HEALTHCHECK --interval=30s --timeout=3s CMD python -c "import urllib.request;urllib.request.urlopen('http://localhost:8000/health')"
CMD ["uvicorn", "antphy_services.apps.api.main:app", "--host", "0.0.0.0", "--port", "8000"]

# --- 3b. Worker (incluye el motor) ------------------------------------------------
FROM python-base AS worker
COPY --from=engine-build /src/build/antphy-solve /usr/local/bin/antphy-solve
ENV ANTPHY_SOLVER_BIN=/usr/local/bin/antphy-solve
CMD ["dramatiq", "antphy_services.apps.worker.main", "--processes", "1", "--threads", "2"]
