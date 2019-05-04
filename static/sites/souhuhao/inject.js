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
        fd.append('file', file);
        var url = 'https://mp.sohu.com/commons/upload/file';
        base.post(url, fd, (r) => {
            var imgObj = JSON.parse(r);
            dom.src = "http:" + imgObj.url;
            ipcRenderer.send('imgUploadMain', {
                id: dom.id,
                siteId: this.siteId,
                url: dom.src
            });
            this.guard -= 1;
            if (this.guard < 1) {
                this.end();
            }
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
        setTimeout(() => {
            let ta = document.querySelector(".title>.title");
            ta.value = ""
            ta.focus();
            setTimeout(() => {
                clipboard.writeText(this.title);
                win.webContents.paste();
            }, 380);
        }, 880);
        editor.__quill.clipboard.dangerouslyPasteHTML(this.doc.body.innerHTML);
        base.ajaxInjector((obj, url) => {
            if (obj && url == 'https://mp.sohu.com/v3/news/draft')
                ipcRenderer.send('articlePublishMain', {
                    siteId: 'souhuhao',
                    url: 'https://mp.sohu.com/mpfe/v3/main/news/addarticle?contentStatus=2&id=' + obj
                });
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
        if (!document.querySelector(".ql-editor")) {
            waitForReady(cb);
            return;
        }
        cb();
    }, 380);
}
ipcRenderer.on('message', (event, article) => {
    base.removeBeforUnload();
    var url = window.location.href;
    if (url == "https://mp.sohu.com/mpfe/v3/main/first/page") {
        window.location.href = article.url;
        return;
    }
    waitForReady(function () {
        imgProcessor.init(article);
    })
    return;
})