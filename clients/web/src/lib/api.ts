import { createApiClient } from "@antphy/api-client";

// En desarrollo el proxy de Vite+ redirige /api al backend (ver vite.config.ts).
export const api = createApiClient(import.meta.env.VITE_API_URL ?? "/api");
