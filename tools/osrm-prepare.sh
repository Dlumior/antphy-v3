#!/usr/bin/env bash
# Descarga el mapa de Perú (OpenStreetMap, Geofabrik) y lo preprocesa para OSRM (MLD).
# Uso: tools/osrm-prepare.sh   → deja los archivos en infra/compose/osrm-data/
set -euo pipefail

DATA_DIR="$(cd "$(dirname "$0")/.." && pwd)/infra/compose/osrm-data"
IMAGE="ghcr.io/project-osrm/osrm-backend:v5.27.1"
PBF="peru-latest.osm.pbf"

mkdir -p "$DATA_DIR"
cd "$DATA_DIR"
[ -f "$PBF" ] || curl -fL -o "$PBF" "https://download.geofabrik.de/south-america/$PBF"

docker run --rm -v "$DATA_DIR:/data" "$IMAGE" osrm-extract -p /opt/car.lua "/data/$PBF"
docker run --rm -v "$DATA_DIR:/data" "$IMAGE" osrm-partition /data/peru-latest.osrm
docker run --rm -v "$DATA_DIR:/data" "$IMAGE" osrm-customize /data/peru-latest.osrm
echo "Listo. Levanta OSRM con: just osrm"
