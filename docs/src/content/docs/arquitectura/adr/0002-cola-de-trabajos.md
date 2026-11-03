---
title: "ADR 0002: Dramatiq sobre Redis para los trabajos de optimización"
description: Por qué la API no resuelve directamente.
---

**Estado:** aceptada · **Fecha:** 2026-11-02

## Contexto

Resolver una instancia grande puede tardar minutos y consume CPU. Si la API lo hace en el
mismo proceso, se bloquea y no escala por separado.

## Decisión

La API valida la instancia, la guarda y encola el trabajo; un worker con Dramatiq lo ejecuta.
Redis cumple dos papeles: broker de la cola y almacén temporal de trabajos (con TTL).

## Consecuencias

- API y worker escalan por separado (dos servicios ECS en AWS).
- El cliente consulta el estado del trabajo (polling); más adelante, eventos con SSE.
