"""Punto de entrada del worker. Importar este módulo registra todos los actores."""

from antphy_services.modules.optimization.infrastructure import tasks  # noqa: F401
from antphy_services.shared.broker import broker
from antphy_services.shared.logging import configure_logging

configure_logging()

__all__ = ["broker"]
