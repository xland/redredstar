const electron = require('electron');
const fs = require('fs-extra')
const path = require('path');
const basePath = electron.remote.app.getPath('userData');
var firstTime = false;
if (fs.existsSync(path.join(basePath, 'xxm_import'))) {
    fs.removeSync(path.join(basePath, 'xxm'));
    fs.renameSync(path.join(basePath, 'xxm_import'), path.join(basePath, 'xxm'));
} else if (!fs.existsSync(path.join(basePath, 'xxm'))) {
    firstTime = true;
    fs.mkdirSync(path.join(basePath, 'xxm'));
}
const knex = require('knex')({
    client: 'sqlite3',
    useNullAsDefault: true,
    connection: {
        filename: path.join(basePath, 'xxm', "db")
    },
    log: {
        info(message) {
            console.err(message);
        }
    }
});

const initializer = {
    async initTable() {
        await knex.schema.createTable('articles', table => {
            table.increments('id');
            table.string('title');
            table.string('from_url');
            table.datetime('created_at').defaultTo(knex.fn.now());
            table.datetime('updated_at').defaultTo(knex.fn.now());
            table.datetime('visited_at').defaultTo(knex.fn.now());
            table.string('editor_type').defaultTo("html");
        }).createTable('tags', table => {
            table.increments('id');
            table.string('title');
            table.datetime('created_at').defaultTo(knex.fn.now());
        }).createTable('article_tag', table => {
            table.increments('id');
            table.integer('tag_id');
            table.integer('article_id');
        }).createTable('settings', (table) => {
            table.increments('id');
            table.integer('autosave_interval');
            table.integer('img_w');
            table.integer('img_h');
            table.string("editor_type").defaultTo("html");
            table.boolean('jna_sync').defaultTo(true);
            table.string('jna_token');
            table.boolean('jna_login_show').defaultTo(false);
            table.datetime('created_at').defaultTo(knex.fn.now());
        }).createTable('article_site', (table) => {
            table.increments('id');
            table.integer('article_id');
            table.integer('site_id');
            table.integer('edit_url');
            table.datetime('created_at').defaultTo(knex.fn.now());
        }).createTable('flowers', (table) => {
            table.increments('id');
            table.integer('content');
            table.integer('from_url');
            table.datetime('updated_at').defaultTo(knex.fn.now());
            table.datetime('created_at').defaultTo(knex.fn.now());
        }).createTable('flower_tag', table => {
            table.increments('id');
            table.integer('tag_id');
            table.integer('flower_id');
        }).createTable('minds', table => {
            table.increments('id');
            table.string('title');
            table.datetime('created_at').defaultTo(knex.fn.now());
            table.datetime('updated_at').defaultTo(knex.fn.now());
            table.datetime('visited_at').defaultTo(knex.fn.now());
        }).createTable('mind_tag', table => {
            table.increments('id');
            table.integer('tag_id');
            table.integer('mind_id');
        })
    },
    async initDefaultData() {
        let defaultSetting = {
            autosave_interval: 8,
            img_w: 1300,
            img_h: 800,
            editor_type: 'html',
            jna_sync: true,
        };
        await knex.insert(defaultSetting).into("settings");
    },
    async extarColumns() {
        let flag = await knex.schema.hasColumn("articles", "visited_at");
        if (flag) return;
        await knex.schema.alterTable('articles', table => {
            table.datetime('visited_at');
        });
        flag = knex.schema.hasColumn("articles", "from_url");
        if (flag) return;
        await knex.schema.alterTable('articles', table => {
            table.string('from_url');
        });
        flag = knex.schema.hasTable('flowers');
        if (flag) return
        await knex.schema.createTable('flowers', function(table) {
            table.increments('id');
            table.integer('content');
            table.string('from_url');
            table.datetime('updated_at').defaultTo(knex.fn.now());
            table.datetime('created_at').defaultTo(knex.fn.now());
        }).createTable('flower_tag', table => {
            table.increments('id');
            table.integer('tag_id');
            table.integer('flower_id');
        });
        flag = knex.schema.hasTable('minds');
        if (flag) return
        await knex.schema.createTable('minds', table => {
            table.increments('id');
            table.string('title');
            table.datetime('created_at').defaultTo(knex.fn.now());
            table.datetime('updated_at').defaultTo(knex.fn.now());
            table.datetime('visited_at').defaultTo(knex.fn.now());
        }).createTable('mind_tag', table => {
            table.increments('id');
            table.integer('tag_id');
            table.integer('mind_id');
        });
    },
    async init(cb) {
        if (!firstTime) {
            cb(knex);
            this.extarColumns();
            return;
        }
        await this.initTable();
        await this.initDefaultData();
        cb(knex)
    }
}

export default initializer;