import typescript from "@rollup/plugin-typescript";
import { terser } from "rollup-plugin-terser";
const external = require("./external");
export default {
  input: "src/main/mainEntry.ts",
  output: {
    sourcemap: false,
    format: "cjs",
    name: "app",
    file: "public/main.js",
  },
  plugins: [
    typescript({
      sourceMap: false,
      inlineSources: false,
    }),
    terser(),
  ],
  external,
};
