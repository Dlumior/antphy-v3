---
title: "ADR 0001: Monorepo con contratos compartidos"
description: Por qué un solo repositorio y una carpeta contracts/.
---

**Estado:** aceptada · **Fecha:** 2026-11-02

## Contexto

El proyecto tiene un motor C++, un backend Python, clientes TypeScript, infraestructura e
informe. Los tres primeros intercambian los mismos documentos (instancia, solución, escenario).

## Decisión

Un monorepo con una carpeta `contracts/` que contiene los JSON Schema y el OpenAPI. Cada
componente valida contra ellos; el CI corre por carpeta con filtros de rutas.

## Consecuencias

- Un cambio de contrato se hace en un solo PR que toca todos los lados.
- El CI debe filtrar por carpeta para no recompilar todo en cada cambio.
