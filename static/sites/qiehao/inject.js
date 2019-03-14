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
        fd.append("subModule", "normal_zhengwen");
        fd.append("id", "WU_FILE_" + this.guard);
        fd.append("name", file.name);
        fd.append("type", file.type);
        fd.append('lastModifiedDate', new Date());
        fd.append('Filename', file.name);
        fd.append("Filedata", file);
        let url = "https://om.qq.com/image/archscaleupload?isRetImgAttr=1&relogin=1";
        base.post(url, fd, (r) => {
            var imgObj = JSON.parse(r);
            dom.src = imgObj.data.url.size['641'].imgurl;
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
        let iframe = document.getElementById("ueditor_0").contentWindow
        iframe.editor.setContent(this.doc.body.innerHTML);
        var win = remote.BrowserWindow.fromId(this.winId);
        win.focus();
        let titleTb = document.querySelector("label.input-control-title").children[0];
        titleTb.focus();
        titleTb.value = "";
        clipboard.writeText(this.title);
        win.webContents.paste();
        base.ajaxInjector(obj => {
            if (obj && obj.response && obj.response.msg == "Save success" && obj.data) {
                let id = obj.data.articleId;
                ipcRenderer.send('articlePublishMain', {
                    siteId: this.siteId,
                    url: 'https://om.qq.com/article/articlePublish?articleId=' + id + '&atype=0'
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
        if (!document.getElementById("ueditor_0")) {
            waitForReady(cb);
            return;
        }
        let win = document.getElementById("ueditor_0").contentWindow;
        if (!win.editor) {
            waitForReady(cb);
            return;
        }
        cb();
    }, 280);
}
ipcRenderer.on('message', (event, article) => {
    window.onbeforeunload = null;
    let url = window.location.href;
    if (url == "https://om.qq.com/userAuth/index") {
        return;
    }
    if (!url.startsWith('https://om.qq.com/article/articlePublish')) {
        window.location.href = article.url
        return;
    }
    waitForReady(() => {
        window.onbeforeunload = null;
        imgProcessor.init(article);
    })
})