import {
  app,
  BrowserWindow,
  ipcMain,
  Menu
} from 'electron'
import {
  ebtMain
} from 'electron-baidu-tongji'
import {
  autoUpdater
} from 'electron-updater'
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
  if (process.platform == 'darwin') {
    Menu.setApplicationMenu(Menu.buildFromTemplate(menuData));
  } else {
    Menu.setApplicationMenu(null);
  }
  setTimeout(() => {
    if(process.env.NODE_ENV === 'production') {
      autoUpdater.checkForUpdates();
    }
  }, 6000);
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

ipcMain.on('imgUploadMain', (event, message) => {
  mainWindow.webContents.send('imgUploadRenderer', message);
});
ipcMain.on('articlePublishMain', (event, message) => {
  mainWindow.webContents.send('articlePublishRenderer', message);
});

ipcMain.on('updateMain', (event, message) => {
  autoUpdater.quitAndInstall()
})

autoUpdater.on('update-downloaded', () => {
  mainWindow.webContents.send('updateRenderer');
})