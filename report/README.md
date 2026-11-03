# report

Informe técnico del algoritmo en LaTeX (biblatex + biber, estilo APA).

```bash
latexmk            # genera build/main.pdf
latexmk -c         # limpia auxiliares
```

- `chapters/`: un archivo por capítulo. Notación común en `macros.tex`.
- `figures/` y `tables/`: **generadas** por `engine/experiments`; no editarlas a mano.
- El CI compila el PDF y lo publica como artefacto (y en la documentación).
