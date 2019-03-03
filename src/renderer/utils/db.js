const electron = require('electron');
const fs = require('fs-extra')
const path = require('path');
const basePath = path.join(electron.remote.app.getPath('userData'), "/xxm");
import swal from 'sweetalert';
var alertText = "为了更好的兼容未来的新功能\n我得好好练练内功";
if (!fs.existsSync(basePath)) {
    fs.mkdirSync(basePath);
    alertText = "初次见面\n请容我稍事整理"
}
const knex = require('knex')({
    client: 'sqlite3',
    connection: {
        filename: path.join(basePath, "db")
    },
    log: {
        info(message) {
            console.err(message);
        }
    }
});
const rwOption = {
    encoding: 'utf8'
}

const initializer = {
    processArticle(rows) {
        rows.forEach(v => {
            let oldPath = path.join(basePath, v.temp_id.toString());
            let docPath = path.join(oldPath, "a.data");
            let content = fs.readFileSync(docPath, rwOption);
            content = content.replace(/\/xxm\/\d+\/img(?=\d+\.)/g, '/xxm/' + v.id + '/img');
            fs.writeFileSync(docPath, content, rwOption)
            let newPath = path.join(basePath, v.id.toString());
            fs.renameSync(oldPath, newPath);
        })
    },
    setArticleData(articles, cb) {
        knex.schema.createTable('articles', function (table) {
            table.increments('id');
            table.string('title');
            table.bigInteger('temp_id');
            table.datetime('created_at').defaultTo(knex.fn.now());
            table.datetime('updated_at').defaultTo(knex.fn.now());
        }).then(function () {
            if (articles.length < 1) {
                return;
            }
            let arr = articles.map(v => {
                return {
                    title: v.title,
                    temp_id: v.id,
                    created_at: new Date(v.id),
                    updated_at: new Date(v.update)
                }
            })
            return knex("articles").insert(arr);
        }).then((rows) => {
            cb();
        }).catch(e => {
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
            if (tags.length < 1) {
                return;
            }
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
            if (refers.length < 1) {
                return;
            }
            return knex("article_tag").insert(refers);
        }).then(() => {
            cb();
        }).catch(e => {
            console.log(e);
        });
    },
    getTagReferData(tags, cb) {
        let result = [];
        knex('tags').select("id", "temp_id").then(tagRows => {
            knex('articles').select("id", "temp_id").then(articleRows => {
                this.processArticle(articleRows);
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
                });
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
        }).catch(e => {
            console.error(e);
        });
    },
    setTabData(tabs, tabIndex, cb) {
        knex.schema.createTable('tabs', (table) => {
            table.increments('id');
            table.string('title');
            table.string('url');
            table.integer('order_num');
            table.boolean('selected')
            table.datetime('created_at').defaultTo(knex.fn.now());
        }).then(function () {
            if (!tabs) {
                tabs = [{
                    text: "我的知识",
                    url: '/'
                }]
            }
            let arr = tabs.map((v, index) => {
                var obj = {
                    title: v.text,
                    url: v.url,
                    order_num: index,
                    selected: false
                }
                if (index == tabIndex) {
                    obj.selected = true
                }
                return obj
            });
            return knex('tabs').insert(arr);
        }).then(() => {
            cb();
        }).catch(e => {
            console.error(e);
        });
    },
    setArticleSite(cb) {
        knex.schema.createTable('article_site', (table) => {
            table.increments('id');
            table.integer('article_id');
            table.integer('site_id');
            table.integer('edit_url');
            table.datetime('created_at').defaultTo(knex.fn.now());
        }).then(function () {
            cb();
        })
    },
    getObj(name) {
        let fullName = path.join(basePath, name + ".data");
        if (fs.existsSync(fullName)) {
            let str = fs.readFileSync(fullName, rwOption);
            let result = JSON.parse(str);
            fs.unlink(fullName, (err) => {})
            return result;
        } else {
            return null;
        }
    },
    init(cb) {
        //todo: 在6.2.x的时候删掉此目录
        let bakDir = path.join(electron.remote.app.getPath('userData'), "/xxm_bak");
        if (fs.existsSync(bakDir)) {
            cb(knex);
            return;
        }
        swal({
            icon: "info",
            title: '请稍后',
            text: alertText,
            closeOnClickOutside: false,
            closeOnEsc: false,
            timer: 8600,
            buttons: false,
        })
        fs.copySync(basePath, bakDir)
        let a = this.getObj("a") || [];
        let t = this.getObj("t") || [];
        let u = this.getObj("u") || {};
        this.setArticleData(a, () => {
            this.setTagData(t, () => {
                this.getTagReferData(t, (refers) => {
                    this.setTagReferData(refers, () => {
                        this.setUserData(u, () => {
                            this.setTabData(u.tabs, u.tabIndex, () => {
                                this.setArticleSite(() => {
                                    cb(knex);
                                    knex.schema.table("tags", t => {
                                        t.dropColumn("temp_id");
                                    }).catch(e => {
                                        console.error(e);
                                    });
                                    knex.schema.table("articles", t => {
                                        t.dropColumn("temp_id");
                                    }).catch(e => {
                                        console.error(e);
                                    });
                                })
                            });
                        });
                    })
                });
            })
        });
    }
}

export default initializer;