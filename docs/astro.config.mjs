// @ts-check
import starlight from "@astrojs/starlight";
import { defineConfig } from "astro/config";

export default defineConfig({
  site: "https://dlumior.github.io",
  base: "/antphy-v3",
  integrations: [
    starlight({
      title: "antphy-v3",
      description: "Ruteo humanitario con colonia de hormigas",
      defaultLocale: "root",
      locales: { root: { label: "Español", lang: "es" } },
      social: [{ icon: "github", label: "GitHub", href: "https://github.com/Dlumior/antphy-v3" }],
      editLink: { baseUrl: "https://github.com/Dlumior/antphy-v3/edit/main/docs/" },
      sidebar: [
        { label: "Guías", items: [{ autogenerate: { directory: "guias" } }] },
        { label: "Arquitectura", items: [{ autogenerate: { directory: "arquitectura" } }] },
        { label: "Motor (C++)", items: [{ autogenerate: { directory: "motor" } }] },
        { label: "Backend", items: [{ autogenerate: { directory: "backend" } }] },
        { label: "Clientes", items: [{ autogenerate: { directory: "clientes" } }] },
        { label: "Operaciones", items: [{ autogenerate: { directory: "operaciones" } }] },
      ],
    }),
  ],
});
