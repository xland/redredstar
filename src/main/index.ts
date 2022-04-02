import { app, BrowserWindow } from "electron";
app.on("ready", () => {
  const mainWindow = new BrowserWindow();
  mainWindow.loadURL("http://localhost:8080");
  mainWindow.webContents.openDevTools();
});
