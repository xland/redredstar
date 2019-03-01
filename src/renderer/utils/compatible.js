const electron = require('electron');
const fs = require('fs');
const path = require('path');
const readConfig = {
    encoding: 'utf8'
}
const basePath = path.join(electron.remote.app.getPath('userData'), "/xxm");
const knex = require('knex')({
    dialect: 'sqlite3',
    connection: {
        filename: path.join(basePath, "db")
    }
});

let getArticleData = function () {
    var aStr = fs.readFileSync(path.join(basePath, "a.data"), readConfig);
    var aObj = JSON.parse(aStr);
}
let getTagData = function () {
    var tStr = fs.readFileSync(path.join(basePath, "t.data"), readConfig);
    var tObj = JSON.parse(tStr);
}
let getUserData = function () {
    var uStr = fs.readFileSync(path.join(basePath, "u.data"), readConfig);
    var uObj = JSON.parse(uStr);
    knex.schema.createTable('settings', (table) => {
        table.increments('id');
        table.integer('autosave_interval');
        table.integer('img_w');
        table.integer('img_h');
    }).then(function () {
        return knex.insert({
            autosave_interval: uObj.autoSaveIntervalSeconds,
            img_w: 1300,
            img_h: 800
        }).into('settings');
    })
}
let getOldData = function () {
    if (fs.existsSync(path.join(basePath, "a.data"))) {
        getArticleData();
        //todo del old data
    }
    if (fs.existsSync(path.join(basePath, "t.data"))) {
        getTagData();
        //todo del old data
    }
    if (fs.existsSync(path.join(basePath, "u.data"))) {
        getUserData();
        //todo del old data
    }
}
getOldData();