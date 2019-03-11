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
        let fd = new FormData();
        fd.append("type", "image");
        fd.append("app_id", window.MP.appInfo.appid);
        fd.append("save_material", 1);
        fd.append("is_waterlog", 1);
        fd.append('no_compress', 0);
        fd.append('media', file);
        let url = "https://baijiahao.baidu.com/builderinner/api/content/file/upload";
        base.post(url, fd, (r) => {
            var imgObj = JSON.parse(r);
            dom.src = imgObj.ret.bos_url;
            console.log(dom.src);
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
            if(v.dataset[this.siteId]){
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
        let titleTb = document.querySelector("input.ant-input");
        titleTb.focus();
        titleTb.value = "";
        clipboard.writeText(this.title);
        win.webContents.paste();
        base.ajaxInjector(obj => {
            if (obj && obj.ret) {
                let id = obj.ret.article_id;
                ipcRenderer.send('articlePublishMain', {
                    siteId: this.siteId,
                    url: 'https://baijiahao.baidu.com/builder/rc/edit?type=news&article_id=' + id
                });
            }
        })
    },
    start() {
        this.imgs.forEach(v => {
            if (!v.dataset[this.siteId]) {
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
        this.siteId = article.siteId;
        this.winId = article.winId;
        this.title = article.title;
        if (this.imgs.length > 0) {
            this.start();
        } else {
            this.end();
        }
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

//editor.setContent("allen");
//https://baijiahao.baidu.com/builderinner/api/content/file/upload
//img up response success","ret":{"app_id":1587171278593857,"bos_url":"http:\/\/pic.rmb.bdstatic.com\
ipcRenderer.on('message', (event, article) => {
    window.onbeforeunload = null;
    let url = window.location.href;
    if (url.startsWith('https://baijiahao.baidu.com/builder/rc/edit')) {
        if (article.type == "edit" && url != article.url) {
            window.location.href = article.url
            return;
        }
        waitForReady(() => {
            window.onbeforeunload = null;
            imgProcessor.init(article);
        })
    }
})