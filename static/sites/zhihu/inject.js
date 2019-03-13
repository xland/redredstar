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
    uploadImg(dom, file) {
        let fd = new FormData();
        fd.append('picture', file);
        fd.append("source", "article");
        var url = 'https://zhuanlan.zhihu.com/api/uploaded_images';
        base.post(url, fd, (r) => {
            var imgObj = JSON.parse(r);
            dom.src = imgObj.src;
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
            'X-Requested-With': 'Fetch',
            'accept': 'application/json, text/plain, */*'
        });
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
        var win = remote.BrowserWindow.fromId(this.winId);
        win.focus();
        setTimeout(function () {
            document.getElementsByClassName("WriteIndex-titleInput")[0].children[0].focus();
            clipboard.writeText(this.title);
            win.webContents.paste();
            setTimeout(function () {
                document.getElementsByClassName("public-DraftEditor-content")[0].click();
                clipboard.writeHTML(this.doc.body.innerHTML);
                win.webContents.paste();
                ipcRenderer.send('articlePublishMain', {
                    siteId: 'zhihu',
                    url: window.location.href
                });
            }.bind(this), 800)
        }.bind(this), 800)
    },
    start() {
        this.imgs.forEach(v => {
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
        this.siteId = article.siteId;
        this.winId = article.winId;
        this.title = article.title;
        this.start();
    }
}

ipcRenderer.on('message', (event, article) => {
    window.onbeforeunload = null;
    var url = window.location.href;
    if (url.startsWith("https://www.zhihu.com/signin")) {
        return;
    }
    if (document.getElementsByClassName("WriteIndex-titleInput")[0]) {
        imgProcessor.init(article);
    }
    return;
})