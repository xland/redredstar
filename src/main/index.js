import {
  app,
  BrowserWindow,
  ipcMain,
  Menu
} from 'electron'
import {
  ebtMain
} from 'electron-baidu-tongji'
import menuData from './menu.js';
ebtMain(ipcMain)

const curVersion = require('../../package.json').version;

let winURL = ""
let mainWindow
if (process.env.NODE_ENV !== 'development') {
  global.__static = require('path').join(__dirname, '/static').replace(/\\/g, '\\\\')
  winURL = `file://${__dirname}/index.html`;
} else {
  app.getVersion = () => curVersion;
  winURL = `http://localhost:9080`;
}

function createWindow() {
  mainWindow = new BrowserWindow({
    "width": 980,
    "height": 600,
    "minWidth": 980,
    "minHeight": 600,
    "autoHideMenuBar": false,
    "webPreferences": {
      "webSecurity": false
    }
  })
  mainWindow.loadURL(winURL);
  mainWindow.on('closed', () => {
    mainWindow = null
  })
  mainWindow.on('close', (event) => {
    event.preventDefault();
    mainWindow.webContents.send('saveArticleRenderer', {});
  })
  if (process.platform == 'darwin') {
    Menu.setApplicationMenu(Menu.buildFromTemplate(menuData));
  }
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
  mainWindow.webContents.send('articleRefreshRenderer', message);
});
ipcMain.on('appQuit', (e) => {
  mainWindow.destroy();
  app.quit();
})

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