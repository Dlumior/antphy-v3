"""Broker de Dramatiq. Debe configurarse antes de declarar actores."""

import dramatiq
from dramatiq.brokers.redis import RedisBroker
from dramatiq.brokers.stub import StubBroker

from antphy_services.shared.config import get_settings


def configure_broker() -> dramatiq.Broker:
    settings = get_settings()
    broker: dramatiq.Broker = (
        StubBroker() if settings.env == "test" else RedisBroker(url=settings.redis_url)  # type: ignore[no-untyped-call]
    )
    dramatiq.set_broker(broker)
    return broker


broker = configure_broker()
