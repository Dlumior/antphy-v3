---
title: Formulación del problema
description: CVRP con urgencia, flota heterogénea y atención parcial.
sidebar:
  order: 1
---

**Restricciones:** capacidad, cada punto atendido a lo sumo una vez, rutas que salen y
vuelven al almacén (el regreso cuenta), duración máxima de ruta, tamaño de flota. Un punto
puede quedar **sin atender** con penalización.

**Objetivos:**

| Objetivo  | Definición                                                        |
| --------- | ----------------------------------------------------------------- |
| Distancia | $C = \sum$ distancia de los tramos                                |
| Makespan  | $T = \max_r$ duración de la ruta $r$                              |
| Urgencia  | $U = \sum_i u_i \cdot \varphi(t_i)$, con $t_i$ la hora de llegada |

La severidad $u_i \in [0,1]$ y la forma $\varphi$ dependen del escenario
(ver [`contracts/scenarios`](https://github.com/Dlumior/antphy-v3/tree/main/contracts/scenarios)).

El desarrollo completo está en el informe LaTeX (`report/`).
