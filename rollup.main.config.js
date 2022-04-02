import typescript from "@rollup/plugin-typescript";
import { terser } from "rollup-plugin-terser";
export default {
  input: "src/main/index.ts",
  output: {
    sourcemap: false,
    format: "cjs",
    name: "app",
    file: "public/build/main.js",
  },
  plugins: [
    typescript({
      sourceMap: false,
      inlineSources: false,
    }),
    terser(),
  ],
};
