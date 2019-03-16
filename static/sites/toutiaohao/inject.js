const fs = require('fs');
const path = require('path');
const {
    clipboard,
    ipcRenderer,
    remote
} = require('electron');
const base = require('../base');

let imgProcessor = {
    imgs: null,
    doc: null,
    guard: 0,
    uploadImg(dom, file) {
        let fd = new FormData();
        fd.append("type", file.type);
        fd.append("upfile", file);
        let url = "https://mp.toutiao.com/tools/upload_picture/?type=ueditor&pgc_watermark=1&action=uploadimage&encode=utf-8";
        base.post(url, fd, (r) => {
            var imgObj = JSON.parse(r);
            dom.src = imgObj.url;
            ipcRenderer.send('imgUploadMain', {
                id: dom.id,
                siteId: this.siteId,
                url: dom.src
            });
            this.guard -= 1;
            if (this.guard < 1) {
                this.end();
            }
        })
    },
    end() {
        this.imgs.forEach(v => {
            if (v.dataset[this.siteId]) {
                v.src = v.dataset[this.siteId];
            }
            Object.keys(v.dataset).forEach(ds => {
                delete v.dataset[ds];
            })
        });
        window.onbeforeunload = null;
        editor.setContent(this.doc.body.innerHTML);
        var win = remote.BrowserWindow.fromId(this.winId);
        win.focus();
        let titleTb = document.querySelector("#title");
        titleTb.focus();
        titleTb.value = "";
        clipboard.writeText(this.title);
        win.webContents.paste();
        base.ajaxInjector(obj => {
            if (obj && obj.data && obj.data.pgc_id) {
                let id = obj.data.pgc_id;
                let indexUrl = document.querySelector(".tui2-menu-item").children[0].href;
                let firstStr = indexUrl.substr(0, indexUrl.lastIndexOf('/') + 1);
                ipcRenderer.send('articlePublishMain', {
                    siteId: this.siteId,
                    url: firstStr + 'graphic/publish/?pgc_id=' + id
                });
            }
        })
    },
    start() {
        this.imgs.forEach(v => {
            if (this.type == 'new') {
                delete v.dataset[this.siteId];
            }
            if (!v.dataset[this.siteId]) {
                this.guard += 1;
                let file = base.getFileObjByLocalUrl(v.src);
                this.uploadImg(v, file);
            }
        });
        if (this.guard < 1) {
            this.end();
        }
    },
    init(article) {
        var parser = new DOMParser();
        this.doc = parser.parseFromString(article.content, "text/html");
        this.imgs = this.doc.querySelectorAll('img');
        Object.assign(this, article);
        this.start();
    }
}

var waitForReady = function (cb) {
    setTimeout(function () {
        if (typeof editor == "undefined") {
            waitForReady(cb);
            return;
        }
        cb();
    }, 280);
}
var waitForIndex = function (cb) {
    setTimeout(function () {
        let goBtn = document.querySelector(".home-go-write");
        if (!goBtn) {
            waitForIndex(cb);
            return;
        }
        cb(goBtn);
    }, 280);
}

ipcRenderer.on('message', (event, article) => {
    //base.removeBeforUnload();
    let url = window.location.href;
    if (article.type == "edit" && url != article.url) {
        window.location.href = article.url;
        return;
    }
    if (article.type == "new" && !url.includes("publish")) {
        waitForIndex(btn => {
            window.location.href = btn.href;
        })
        return;
    }
    waitForReady(() => {
        imgProcessor.init(article);
    });
})