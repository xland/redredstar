import typescript from "@rollup/plugin-typescript";
export default {
  input: "src/main/index.ts",
  output: {
    file: "bundle.js",
    format: "cjs",
  },
};
