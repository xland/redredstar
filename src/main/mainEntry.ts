import { app, BrowserWindow } from "electron";
import { ConfigWindow } from "./ConfigWindow";
import { CommonWindowEvent } from "./CommonWindowEvent";
process.env.ELECTRON_DISABLE_SECURITY_WARNINGS = "true";
app.commandLine.appendSwitch("--disable-site-isolation-trials");
let mainWindow;
app.on("ready", () => {
  let config = new ConfigWindow();
  mainWindow = new BrowserWindow(config);
  mainWindow.loadURL("http://localhost:5916");
  mainWindow.webContents.openDevTools();
  CommonWindowEvent.listen();
});
