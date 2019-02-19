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
        var xhr = new XMLHttpRequest();
        xhr.open("GET", 'https://www.jianshu.com/upload_images/token.json?filename=' + file.name, true);
        xhr.onreadystatechange = () => {
            if (xhr.readyState == 4 && (xhr.status == 200 || xhr.status == 304)) {
                var r = JSON.parse(xhr.responseText);
                var formData = new FormData();
                formData.append("token", r.token);
                formData.append("key", r.key);
                formData.append("file", file);
                formData.append("x:protocol", 'https');
                var xhr2 = new XMLHttpRequest();
                xhr2.open("POST", 'https://upload.qiniup.com/', true);
                xhr2.setRequestHeader("Cache-Control", "no-cache");
                xhr2.onreadystatechange = () => {
                    if (xhr2.readyState == 4 && (xhr2.status == 200 || xhr2.status == 304)) {
                        var imgObj = JSON.parse(xhr2.responseText);
                        dom.dataset[this.siteId] = imgObj.url;
                        this.guard -= 1;
                        if (this.guard < 1) {
                            ipcRenderer.send('contentRefreshMain', {
                                content: this.doc.body.innerHTML
                            });
                            this.end();
                        }
                    }
                };
                xhr2.send(formData);
            }
        }
        xhr.send();
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
            if (!v.dataset[this.siteId]) {
                this.guard += 1;
                var filePath = decodeURI(v.src).substr(7);
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
                siteId:"jianshu",
                url: window.location.href
            });
        }else{
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
    }else{
        waitForEdit((titleTb) => {
            imgProcessor.init(article);
        });
    }
})