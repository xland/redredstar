"use strict";var i=require("electron");class e{constructor(){this.nodeIntegration=!0,this.devTools=!0,this.webSecurity=!1,this.nodeIntegrationInSubFrames=!0,this.nodeIntegrationInWorker=!0,this.worldSafeExecuteJavaScript=!0,this.contextIsolation=!1,this.allowRunningInsecureContent=!0,this.center=!0,this.webgl=!1,this.disableHtmlFullscreenWindowResize=!0,this.enableWebSQL=!1,this.spellcheck=!1}}class n{constructor(){this.maximizable=!0,this.resizable=!0,this.center=!0,this.frame=!1,this.show=!1,this.webPreferences=new e,this.nodeIntegrationInSubFrames=!0,this.nativeWindowOpen=!0,this.momodalable=!1,this.movable=!0,this.thickFrame=!0}}let t;process.env.ELECTRON_DISABLE_SECURITY_WARNINGS="true",i.app.commandLine.appendSwitch("--disable-site-isolation-trials"),i.app.on("ready",(()=>{let e=new n;t=new i.BrowserWindow(e),t.loadURL("http://localhost:8080"),t.webContents.openDevTools(),class{static getWin(e){return i.BrowserWindow.fromWebContents(e.sender)}static listen(){i.ipcMain.handle("minimizeWindow",(i=>{var e;null===(e=this.getWin(i))||void 0===e||e.minimize()})),i.ipcMain.handle("maxmizeWindow",(i=>{var e;null===(e=this.getWin(i))||void 0===e||e.maximize()})),i.ipcMain.handle("unmaximizeWindow",(i=>{var e;null===(e=this.getWin(i))||void 0===e||e.unmaximize()})),i.ipcMain.handle("hideWindow",(i=>{var e;null===(e=this.getWin(i))||void 0===e||e.hide()})),i.ipcMain.handle("showWindow",(i=>{var e;null===(e=this.getWin(i))||void 0===e||e.show()})),i.ipcMain.handle("closeWindow",(i=>{var e;null===(e=this.getWin(i))||void 0===e||e.close()})),i.ipcMain.handle("resizable",(i=>{var e;return null===(e=this.getWin(i))||void 0===e?void 0:e.isResizable()}))}static windowMaximizeChange(i){i.on("maximize",(()=>{i.webContents.send("windowMaximized")})),i.on("unmaximize",(()=>{i.webContents.send("windowUnmaximized")}))}}.listen()}));
