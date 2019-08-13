/**
 * This file is used specifically and only for development. It installs
 * `electron-debug` & `vue-devtools`. There shouldn't be any need to
 *  modify this file, but it can be used to extend your development
 *  environment.
 */

/* eslint-disable */

// Install `electron-debug` with `devtron`
require('electron-debug')({ showDevTools: true })

// Install `vue-devtools`
require('electron').app.on('ready', async() => {
    let installExtension = require('electron-devtools-installer')
    await installExtension.default(installExtension.VUEJS_DEVTOOLS);
})

// Require `main` process to boot app
require('./index')