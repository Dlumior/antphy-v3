import createClient from "openapi-fetch";

import type { components, paths } from "./schema.d.ts";

export type Schemas = components["schemas"];
export type JobResponse = Schemas["JobResponse"];
export type JobStatus = Schemas["JobStatus"];

/** Crea un cliente tipado contra la API de antphy-v3. */
export function createApiClient(baseUrl: string) {
  return createClient<paths>({ baseUrl });
}

export type ApiClient = ReturnType<typeof createApiClient>;

/** Un trabajo terminó (con éxito o con error) y ya no hay que seguir consultándolo. */
export function isFinished(status: JobStatus): boolean {
  return status === "succeeded" || status === "failed";
}
