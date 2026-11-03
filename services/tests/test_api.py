from typing import Any

from conftest import FakeQueue
from fastapi.testclient import TestClient


def test_health(client: TestClient) -> None:
    response = client.get("/health")
    assert response.status_code == 200
    assert response.json()["status"] == "ok"


def test_list_scenarios(client: TestClient) -> None:
    names = {s["scenario"] for s in client.get("/scenarios").json()}
    assert {"pandemia", "terremoto", "nino_inundacion"} <= names


def test_reference_scenarios_match_schema(client: TestClient) -> None:
    for name in ("pandemia", "terremoto", "nino_inundacion"):
        assert client.get(f"/scenarios/{name}").status_code == 200


def test_submit_and_get_job(
    client: TestClient, queue: FakeQueue, demo_instance: dict[str, Any]
) -> None:
    response = client.post("/optimization/jobs", json={"instance": demo_instance})
    assert response.status_code == 202
    job = response.json()
    assert job["status"] == "queued"
    assert queue.sent == [job["id"]]

    fetched = client.get(f"/optimization/jobs/{job['id']}").json()
    assert fetched["id"] == job["id"]


def test_invalid_instance_is_rejected(client: TestClient, queue: FakeQueue) -> None:
    response = client.post("/optimization/jobs", json={"instance": {"name": "x"}})
    assert response.status_code == 422
    assert queue.sent == []


def test_unknown_job_returns_404(client: TestClient) -> None:
    assert client.get("/optimization/jobs/nope").status_code == 404
