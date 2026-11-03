import { type JobResponse, isFinished } from "@antphy/api-client";
import { useEffect, useState } from "react";

import { api } from "./lib/api.ts";
import demoInstance from "../../../contracts/examples/tumbes-demo.json";

type Scenario = { scenario: string; version: number; description?: string | null };

const POLL_MS = 1000;

export default function App() {
  const [health, setHealth] = useState<"cargando" | "ok" | "sin conexión">("cargando");
  const [scenarios, setScenarios] = useState<Scenario[]>([]);
  const [job, setJob] = useState<JobResponse | null>(null);
  const [error, setError] = useState<string | null>(null);

  useEffect(() => {
    api
      .GET("/health")
      .then(({ data }) => setHealth(data ? "ok" : "sin conexión"))
      .catch(() => setHealth("sin conexión"));
    api
      .GET("/scenarios")
      .then(({ data }) => setScenarios((data as Scenario[] | undefined) ?? []))
      .catch(() => setScenarios([]));
  }, []);

  useEffect(() => {
    if (!job || isFinished(job.status)) return;
    const timer = setTimeout(async () => {
      const { data } = await api.GET("/optimization/jobs/{job_id}", {
        params: { path: { job_id: job.id } },
      });
      if (data) setJob(data);
    }, POLL_MS);
    return () => clearTimeout(timer);
  }, [job]);

  async function solveDemo() {
    setError(null);
    const { data, error: err } = await api.POST("/optimization/jobs", {
      body: { instance: demoInstance, solver: "nearest_neighbor" },
    });
    if (err || !data) {
      setError("No se pudo encolar la instancia. ¿Está corriendo la API?");
      return;
    }
    setJob(data);
  }

  const solution = job?.solution as SolutionView | null | undefined;

  return (
    <main>
      <header>
        <h1>antphy-v3</h1>
        <p>
          Ruteo humanitario con colonia de hormigas · API: <strong>{health}</strong>
        </p>
      </header>

      <section>
        <h2>Escenarios de urgencia</h2>
        {scenarios.length === 0 ? (
          <p>Sin escenarios (la API no responde).</p>
        ) : (
          <ul>
            {scenarios.map((s) => (
              <li key={s.scenario}>
                <strong>{s.scenario}</strong> v{s.version} — {s.description}
              </li>
            ))}
          </ul>
        )}
      </section>

      <section>
        <h2>Resolver la instancia de ejemplo (Tumbes)</h2>
        <button
          type="button"
          onClick={solveDemo}
          disabled={job !== null && !isFinished(job.status)}
        >
          Resolver
        </button>
        {error && <p role="alert">{error}</p>}
        {job && (
          <p>
            Trabajo <code>{job.id.slice(0, 8)}</code>: <strong>{job.status}</strong>
            {job.error && ` — ${job.error}`}
          </p>
        )}
        {solution && <SolutionTable solution={solution} />}
      </section>
    </main>
  );
}

type SolutionView = {
  feasible: boolean;
  objectives: { distance_m: number; makespan_s: number; urgency_penalty: number };
  routes: {
    vehicle: string;
    stops: string[];
    load: number;
    distance_m: number;
    duration_s: number;
  }[];
  unserved: string[];
};

function SolutionTable({ solution }: { solution: SolutionView }) {
  const { objectives: o } = solution;
  return (
    <>
      <p>
        {solution.feasible ? "Factible" : "No factible"} · {(o.distance_m / 1000).toFixed(1)} km ·
        ruta más larga {(o.makespan_s / 60).toFixed(0)} min · penalización por urgencia{" "}
        {o.urgency_penalty.toFixed(2)}
      </p>
      <table>
        <thead>
          <tr>
            <th>#</th>
            <th>Vehículo</th>
            <th>Paradas</th>
            <th>Carga</th>
            <th>km</th>
            <th>min</th>
          </tr>
        </thead>
        <tbody>
          {solution.routes.map((r, i) => (
            <tr key={i}>
              <td>{i + 1}</td>
              <td>{r.vehicle}</td>
              <td>{r.stops.join(" → ")}</td>
              <td>{r.load}</td>
              <td>{(r.distance_m / 1000).toFixed(1)}</td>
              <td>{(r.duration_s / 60).toFixed(0)}</td>
            </tr>
          ))}
        </tbody>
      </table>
      {solution.unserved.length > 0 && <p>Sin atender: {solution.unserved.join(", ")}</p>}
    </>
  );
}
