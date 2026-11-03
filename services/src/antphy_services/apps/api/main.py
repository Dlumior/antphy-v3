from fastapi import FastAPI
from fastapi.middleware.cors import CORSMiddleware

from antphy_services import __version__
from antphy_services.modules.optimization.api.router import router as optimization_router
from antphy_services.modules.scenarios.api import router as scenarios_router
from antphy_services.shared.config import get_settings
from antphy_services.shared.logging import configure_logging


def create_app() -> FastAPI:
    configure_logging()
    settings = get_settings()
    app = FastAPI(
        title="antphy-v3 API",
        version=__version__,
        description="Ruteo humanitario con colonia de hormigas.",
    )
    app.add_middleware(
        CORSMiddleware,
        allow_origins=settings.cors_origins,
        allow_methods=["*"],
        allow_headers=["*"],
    )

    @app.get("/health", tags=["system"], summary="Estado del servicio")
    def health() -> dict[str, str]:
        return {"status": "ok", "version": __version__, "env": settings.env}

    app.include_router(scenarios_router)
    app.include_router(optimization_router)
    return app


app = create_app()
