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
        let getUrl = 'https://www.jianshu.com/upload_images/token.json?filename=' + file.name;
        let postUrl = 'https://upload.qiniup.com/';
        base.get(getUrl, (rt) => {
            let r = JSON.parse(rt);
            let fd = new FormData();
            fd.append("token", r.token);
            fd.append("key", r.key);
            fd.append("file", file);
            fd.append("x:protocol", 'https');
            base.post(postUrl, fd, rt2 => {
                var imgObj = JSON.parse(rt2);
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
            }, {
                'Cache-Control': 'no-cache'
            }, false)
        });
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
        var win = remote.BrowserWindow.fromId(this.winId);
        win.focus();
        clipboard.writeHTML(this.doc.body.innerHTML);
        var contentDom = document.getElementsByClassName("kalamu-area")[0];
        contentDom.innerHTML = "";
        contentDom.focus();
        win.webContents.paste();
        setTimeout(function() {
            var titleTb = document.getElementsByClassName("_24i7u")[0];
            titleTb.focus();
            titleTb.value = "";
            clipboard.writeText(this.title);
            win.webContents.paste();
            base.clearMask();
        }.bind(this), 960)
    },
    start() {
        base.maskPage();
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
        var contentDom = document.getElementsByClassName("kalamu-area")[0];
        if (!contentDom) {
            alert("抱歉：目前还不支持简书的markdown编辑器!")
            return;
        }
        var parser = new DOMParser();
        this.doc = parser.parseFromString(article.content, "text/html");
        this.imgs = this.doc.querySelectorAll('img');
        Object.assign(this, article);
        this.start();
    }
}

var waitForReady = function(cb) {
    setTimeout(function() {
        var titleTb = document.getElementsByClassName("_24i7u")[0];
        var str = window.getSelection().toString()
        if (!titleTb || !str) {
            waitForReady(cb);
            return;
        }
        cb(titleTb);
    }, 380);
}
var waitForEdit = function(cb) {
    setTimeout(function() {
        var titleTb = document.getElementsByClassName("_24i7u")[0];
        if (!titleTb) {
            waitForEdit(cb);
            return;
        }
        cb(titleTb);
    }, 380);
}
var waitForSave = function() {
    setTimeout(function() {
        var str = document.getElementsByClassName('_3-3KB')[0].innerHTML;
        if (str == "已保存") {
            ipcRenderer.send('articlePublishMain', {
                siteId: "jianshu",
                url: window.location.href
            });
        } else {
            waitForSave();
        }
    }, 380);
}

ipcRenderer.on('message', (event, article) => {
    base.removeBeforUnload();
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