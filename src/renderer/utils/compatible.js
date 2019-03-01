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

export default {
    basePath,
    knex,
    getArticleData(articles) {
        knex.schema.createTable('articles', function (table) {
            table.increments('id');
            table.string('title');
            table.integer('tab_index');
            table.integer('is_selected');
            table.bigInteger('temp_id');
            table.timestamps();
        }).then(function () {
            let arr = articles.map(v => {
                return {
                    title: v.title,
                    tab_index: -1,
                    is_selected: 0,
                    temp_id: v.id
                }
            })
            return knex("articles").insert(arr);
        });
    },
    getTagData(tags) {
        knex.schema.createTable('tags', function (table) {
            table.increments('id');
            table.string('title');
            table.bigInteger('temp_id');
            table.timestamps();
        }).then(function () {
            let arr = tags.map(v => {
                return {
                    title: v.text,
                    temp_id: v.id
                }
            })
            return knex("tags").insert(arr);
        });
    },
    getUserData(obj) {
        knex.schema.createTable('settings', (table) => {
            table.increments('id');
            table.integer('autosave_interval');
            table.integer('img_w');
            table.integer('img_h');
            table.timestamps();
        }).then(function () {
            return knex('settings').insert(obj);
        })

        knex.schema.createTable('article_tag', function (table) {
            table.increments('id');
            table.integer('tag_id');
            table.integer('article_id');
            table.timestamps();
        }).then(function () {
            return null
        });
    },
    init() {
        if (fs.existsSync(path.join(basePath, "a.data"))) {
            var aStr = fs.readFileSync(path.join(basePath, "a.data"), readConfig);
            var aObj = JSON.parse(aStr);
            this.getArticleData(aObj);
            //todo del old data
        }else{
            this.getArticleData([]);
        }
        if (fs.existsSync(path.join(basePath, "t.data"))) {
            var tStr = fs.readFileSync(path.join(basePath, "t.data"), readConfig);
            var tObj = JSON.parse(tStr);
            this.getTagData(tObj);
            //todo del old data
        } else {
            this.getTagData([]);
        }
        if (fs.existsSync(path.join(basePath, "u.data"))) {
            var uStr = fs.readFileSync(path.join(basePath, "u.data"), readConfig);
            var uObj = JSON.parse(uStr);
            this.getUserData({
                autosave_interval: uObj.autoSaveIntervalSeconds,
                img_w: 1300,
                img_h: 800
            });
            //todo del old data
        } else {
            this.getUserData({
                autosave_interval: 8,
                img_w: 1300,
                img_h: 800
            })
        }
    }
}