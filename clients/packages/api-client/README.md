# @antphy/api-client

Cliente tipado de la API. Los tipos (`src/schema.d.ts`) se generan desde
[`contracts/openapi/openapi.json`](../../../contracts/openapi/openapi.json):

```bash
just openapi          # exporta el OpenAPI desde services y regenera este paquete
```

No edites `schema.d.ts` a mano.
