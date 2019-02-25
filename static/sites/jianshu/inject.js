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
    siteId: null,
    doc: null,
    guard: 0,
    title: '',
    uploadImg(dom, file) {
        let getUrl = 'https://www.jianshu.com/upload_images/token.json?filename=' + file.name;
        let postUrl = 'https://upload.qiniup.com/';
        base.get(getUrl, (rt) => {
            let r = JSON.parse(rt);
            let fd = new FormData();
            fd.append("token", r.token);
            fd.append("key", r.key);
            fd.append("file", file);
            fd.append("x:protocol", 'https');
            base.post(postUrl, fd, (rt2) => {
                var imgObj = JSON.parse(rt2);
                dom.dataset[this.siteId] = imgObj.url;
                this.guard -= 1;
                if (this.guard < 1) {
                    var html = this.doc.body.innerHTML;
                    ipcRenderer.send('contentRefreshMain', {
                        content: html
                    });
                    this.end();
                }
            }, {
                'Cache-Control': 'no-cache'
            },false)
        });
    },
    end() {
        this.imgs.forEach(v => {
            v.src = v.dataset[this.siteId]
            Object.keys(v.dataset).forEach(ds => {
                delete v.dataset[ds];
            })
        });
        var win = remote.BrowserWindow.fromId(this.winId);
        win.focus();
        clipboard.writeHTML(this.doc.body.innerHTML);
        var contentDom = document.getElementsByClassName("kalamu-area")[0];
        contentDom.innerHTML = "";
        contentDom.focus();
        win.webContents.paste();
        setTimeout(function () {
            var titleTb = document.getElementsByClassName("_24i7u")[0];
            titleTb.focus();
            titleTb.value = "";
            clipboard.writeText(this.title);
            win.webContents.paste();
        }.bind(this), 1200)
    },
    start() {
        this.imgs.forEach(v => {
            if (!v.dataset[this.siteId] && v.src.startsWith("file")) {
                this.guard += 1;
                var pathIndex = remote.process.platform == "win32" ? 8 : 7
                var filePath = decodeURI(v.src).substr(pathIndex);
                var extname = path.extname(filePath).substr(1);
                var buffer = fs.readFileSync(filePath);
                var file = new window.File([new Uint8Array(buffer)], path.basename(filePath), {
                    type: base.mime[extname]
                });
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
        this.title = article.title;
        this.siteId = article.siteId;
        this.winId = article.winId;
        if (this.imgs.length > 0) {
            this.start();
        } else {
            this.end();
        }
    }
}

var waitForReady = function (cb) {
    setTimeout(function () {
        var titleTb = document.getElementsByClassName("_24i7u")[0];
        var str = window.getSelection().toString()
        if (!titleTb || !str) {
            waitForReady(cb);
            return;
        }
        cb(titleTb);
    }, 600);
}
var waitForEdit = function (cb) {
    setTimeout(function () {
        var titleTb = document.getElementsByClassName("_24i7u")[0];
        if (!titleTb) {
            waitForEdit(cb);
            return;
        }
        cb(titleTb);
    }, 600);
}
var waitForSave = function () {
    setTimeout(function () {
        var str = document.getElementsByClassName('_3-3KB')[0].innerHTML;
        if (str == "已保存") {
            ipcRenderer.send('articleRefreshMain', {
                siteId: "jianshu",
                url: window.location.href
            });
        } else {
            waitForSave();
        }
    }, 600);
}

ipcRenderer.on('message', (event, article) => {
    var url = window.location.href;
    if (url == 'https://www.jianshu.com/sign_in') {
        return;
    }
    if (!url.startsWith('https://www.jianshu.com/writer#/')) {
        window.location.href = article.url;
        return;
    }
    if (article.type == "new") {
        alert("您需要先选择文集且新建文章");
        waitForReady((titleTb) => {
            imgProcessor.init(article);
            waitForSave();
        });
    } else {
        waitForEdit((titleTb) => {
            imgProcessor.init(article);
        });
    }
})