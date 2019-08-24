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
            table.integer('autosave_interval').defaultTo(8);
            table.integer('img_w').defaultTo(1300);
            table.integer('img_h').defaultTo(800);
            table.string("editor_type").defaultTo("html");
            table.boolean('jna_sync').defaultTo(true);
            table.string('jna_token');
            table.string('win_size').defaultTo('1216*830');
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
        await knex.insert({}).into("settings");
    },
    async extarColumns() {
        let flag = await knex.schema.hasColumn("settings", "win_size");
        if (flag) return;
        await knex.schema.alterTable('settings', table => {
            table.string('win_size').defaultTo('1216*830');
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
//todo:  sqlite是有rowid的，不需要再搞个ID出来