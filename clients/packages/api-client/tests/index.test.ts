import { expect, test } from "vite-plus/test";

import { isFinished } from "../src/index.ts";

test("isFinished distingue estados terminales", () => {
  expect(isFinished("succeeded")).toBe(true);
  expect(isFinished("failed")).toBe(true);
  expect(isFinished("queued")).toBe(false);
  expect(isFinished("running")).toBe(false);
});
