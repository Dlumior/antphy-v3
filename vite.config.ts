import { defineConfig } from "vite-plus";

const ignorePatterns = [
  // Cada ecosistema formatea lo suyo: clang-format (engine), ruff (services), terraform fmt (infra).
  "engine/**",
  "services/**",
  "infra/**",
  "report/**",
  "**/dist/**",
  "**/.astro/**",
  "contracts/openapi/openapi.json",
  "clients/packages/api-client/src/schema.d.ts",
];

// Configuración compartida de Vite+ para todo el workspace JS (clients/* y docs).
export default defineConfig({
  fmt: {
    ignorePatterns,
  },
  lint: {
    ignorePatterns,
    jsPlugins: [{ name: "vite-plus", specifier: "vite-plus/oxlint-plugin" }],
    rules: { "vite-plus/prefer-vite-plus-imports": "error" },
    options: { typeAware: true, typeCheck: true },
  },
  run: {
    cache: true,
  },
});
