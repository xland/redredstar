import type { BrowserWindowConstructorOptions } from "electron";
import { ConfigWebPreferences } from "./ConfigWebPreferences";

export class ConfigWindow implements BrowserWindowConstructorOptions {
  width?: number;
  height?: number;
  maximizable = true;
  resizable = true;
  center = true;
  x?: number;
  y?: number;
  alwaysOnTop?: boolean;
  skipTaskbar?: boolean;
  frame = false;
  show = false;
  webPreferences = new ConfigWebPreferences();
  nodeIntegrationInSubFrames = true;
  nativeWindowOpen = true;
  momodalable = false;
  parent?: any;
  movable = true;
  thickFrame = true;
  minHeight?: number;
  minWidth?: number;
}
