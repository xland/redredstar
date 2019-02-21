import { app, BrowserWindow,ipcMain } from 'electron'
const curVersion = require('../../package.json').version;
console.log(curVersion);

let winURL = ""
let mainWindow
if (process.env.NODE_ENV !== 'development') {
  global.__static = require('path').join(__dirname, '/static').replace(/\\/g, '\\\\')
  winURL = `file://${__dirname}/index.html`;
}else{
  app.getVersion = ()=> curVersion;
  winURL = `http://localhost:9080`;
}

function createWindow () {
  mainWindow = new BrowserWindow({
    "width": 1000,
    "height": 600,
    "minWidth": 1000,
    "minHeight": 600,
    "autoHideMenuBar":false,
    "webPreferences": {
      "webSecurity": false
    }
  })
  mainWindow.setMenu(null);

  mainWindow.loadURL(winURL)

  mainWindow.on('closed', () => {
    mainWindow = null
  })
}

app.on('ready', createWindow)

app.on('window-all-closed', () => {
  if (process.platform !== 'darwin') {
    app.quit()
  }
})

app.on('activate', () => {
  if (mainWindow === null) {
    createWindow()
  }
})

ipcMain.on('contentRefreshMain', (event, message) => {
  mainWindow.webContents.send('contentRefreshRenderer', message);
});
ipcMain.on('articleRefreshMain', (event, message) => {
  console.log(message);
  mainWindow.webContents.send('articleRefreshRenderer', message);
});

/**
 * Auto Updater
 *
 * Uncomment the following code below and install `electron-updater` to
 * support auto updating. Code Signing with a valid certificate is required.
 * https://simulatedgreg.gitbooks.io/electron-vue/content/en/using-electron-builder.html#auto-updating
 */

/*
import { autoUpdater } from 'electron-updater'

autoUpdater.on('update-downloaded', () => {
  autoUpdater.quitAndInstall()
})

app.on('ready', () => {
  if (process.env.NODE_ENV === 'production') autoUpdater.checkForUpdates()
})
 */
