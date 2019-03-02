const electron = require('electron');
const fs = require('fs');
const path = require('path');
const basePath = path.join(electron.remote.app.getPath('userData'), "/xxm");
if (!fs.existsSync(basePath)) {
    fs.mkdirSync(db.basePath);
}
const knex = require('knex')({
    dialect: 'sqlite3',
    connection: {
        filename: path.join(basePath, "db")
    }
});
knex.xxm_ready = false;

export default {
    basePath,
    knex,
    setArticleData(articles, cb) {
        knex.schema.createTable('articles', function (table) {
            table.increments('id');
            table.string('title');
            table.bigInteger('temp_id');
            table.datetime('created_at').defaultTo(knex.fn.now());
            table.datetime('updated_at').defaultTo(knex.fn.now());
        }).then(function () {
            let arr = articles.map(v => {
                return {
                    title: v.title,
                    tab_index: -1,
                    is_selected: 0,
                    temp_id: v.id,
                    created_at: new Date(v.id),
                    updated_at: new Date(v.update)
                }
            })
            return knex("articles").insert(arr);
        }).then((rows) => {
            cb();
        }).catch(function (e) {
            console.log(e);
        });;
    },
    setTagData(tags, cb) {
        knex.schema.createTable('tags', function (table) {
            table.increments('id');
            table.string('title');
            table.bigInteger('temp_id');
            table.datetime('created_at').defaultTo(knex.fn.now());
        }).then(function () {
            let arr = tags.map(v => {
                return {
                    title: v.text,
                    temp_id: v.id,
                    created_at: new Date(v.id),
                }
            })
            return knex("tags").insert(arr);
        }).then(() => {
            cb();
        }).catch(function (e) {
            console.log(e);
        });;
    },
    setTagReferData(refers, cb) {
        knex.schema.createTable('article_tag', function (table) {
            table.increments('id');
            table.integer('tag_id');
            table.integer('article_id');
            table.datetime('created_at').defaultTo(knex.fn.now());
        }).then(function () {
            return knex("article_tag").insert(refers);
        }).then(() => {
            cb();
        }).catch(function (e) {
            console.log(e);
        });
    },
    getTagReferData(tags, cb) {
        let result = [];
        knex('tags').select("id", "temp_id").then(tagRows => {
            knex('articles').select("id", "temp_id").then(articleRows => {
                articleRows.forEach(v => {
                    let oldPath = path.join(basePath, v.temp_id.toString());
                    let newPath = path.join(basePath, v.id.toString());
                    fs.renameSync(oldPath, newPath);
                })
                tags.forEach(tag => {
                    var tagRow = tagRows.find(v => v.temp_id == tag.id);
                    tag.articleIds.forEach(articleId => {
                        var articleRow = articleRows.find(v => v.temp_id == articleId);
                        var obj = {
                            tag_id: tagRow.id,
                            article_id: articleRow.id
                        }
                        result.push(obj);
                    })
                })
                cb(result);
            })
        });
    },
    setUserData(uObj, cb) {
        knex.schema.createTable('settings', (table) => {
            table.increments('id');
            table.integer('autosave_interval');
            table.integer('img_w');
            table.integer('img_h');
            table.datetime('created_at').defaultTo(knex.fn.now());
        }).then(function () {
            let setting = {
                autosave_interval: uObj.autoSaveIntervalSeconds || 8,
                img_w: 1300,
                img_h: 800,
                created_at: uObj.createAt ? new Date(uObj.createAt) : new Date()
            }
            return knex('settings').insert(setting);
        }).then(() => {
            cb();
        }).catch(function (e) {
            console.error(e);
        });
    },
    setTabData(tabs, tabIndex, cb) {
        knex.schema.createTable('tabs', (table) => {
            table.increments('id');
            table.string('title');
            table.string('url');
            table.integer('order_num');
            table.integer('selected')
            table.datetime('created_at').defaultTo(knex.fn.now());
        }).then(function () {
            let arr = tabs.map((v, index) => {
                var obj = {
                    title: v.text,
                    url: v.url,
                    order_num: index,
                    selected: 0
                }
                if (index == tabIndex) {
                    obj.selected = 1
                }
                return obj
            });
            debugger;
            return knex('tabs').insert(arr);
        }).next(() => {
            cb();
        }).catch(function (e) {
            console.error(e);
        });
    },
    backUp(src, dst) {
        let paths = fs.readdirSync(src);
        if (!fs.existsSync(dst)) {
            fs.mkdirSync(dst);
        }
        paths.forEach((dirName) => {
            var _src = src + '/' + dirName;
            var _dst = dst + '/' + dirName;
            fs.stat(_src, (err, stats) => {
                if (err) throw err;
                if (stats.isFile()) {
                    let readable = fs.createReadStream(_src); //创建读取流
                    let writable = fs.createWriteStream(_dst); //创建写入流
                    readable.pipe(writable);
                } else if (stats.isDirectory()) {
                    this.backUp(_src, _dst);
                }
            });
        });
    },
    init() {
        //todo: 在6.2.x的时候删掉此目录
        let bakDir = path.join(electron.remote.app.getPath('userData'), "/xxm_bak");
        if (fs.existsSync(bakDir)) {
            console.error("目录已存在");
            this.knex.xxm_ready = true;
            return;
        }
        this.backUp(basePath, bakDir);
        let getObj = function (name) {
            let fullName = path.join(basePath, name + ".data");
            if (fs.existsSync(fullName)) {
                let str = fs.readFileSync(fullName, {
                    encoding: 'utf8'
                });
                let result = JSON.parse(str);
                fs.unlink(fullName, (err) => {})
                return result;
            } else {
                return null;
            }
        }
        let a = getObj("a") || [];
        let t = getObj("t") || [];
        let u = getObj("u") || {};
        this.setArticleData(a, () => {
            this.setTagData(t, () => {
                this.getTagReferData(t, (refers) => {
                    this.setTagReferData(refers, () => {
                        this.setUserData(u, () => {
                            this.setTabData(u.tabs, u.tabIndex, () => {
                                this.knex.xxm_ready = true;
                            });
                        });
                        knex.schema.table("tags", t => {
                            t.dropColumn("temp_id");
                        }).catch(function (e) {
                            console.error(e);
                        });
                        knex.schema.table("articles", t => {
                            t.dropColumn("temp_id");
                        }).catch(function (e) {
                            console.error(e);
                        });
                    })
                });
            })
        });
    }
}