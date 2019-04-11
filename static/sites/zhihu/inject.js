const {
    clipboard,
    ipcRenderer,
    remote
} = require('electron');
const os = require("os").platform();

const base = require('../base');

let imgProcessor = {
    imgs: null,
    doc: null,
    guard: 0,
    cleanContent(win) {
        let modifiers = os == "darwin" ? ['meta'] : ['control'];
        //let keyCode = os == "darwin" ? 'Meta' : 'Control';
        let keyCode = 'a';
        win.webContents.sendInputEvent({
            type: 'keyDown',
            modifiers,
            keyCode
        })
        win.webContents.sendInputEvent({
            type: 'char',
            modifiers,
            keyCode
        })
        win.webContents.sendInputEvent({
            type: 'keyUp',
            modifiers,
            keyCode
        })
        // browserWindow.webContents.sendInputEvent({
        //     type: 'keyDown',
        //     keyCode
        // });
        // browserWindow.webContents.sendInputEvent({
        //     type: 'keyDown',
        //     keyCode: 'A',
        //     modifiers
        // });
        // browserWindow.webContents.sendInputEvent({
        //     type: 'keyUp',
        //     keyCode: 'A',
        //     modifiers
        // });
        // browserWindow.webContents.sendInputEvent({
        //     type: 'keyUp',
        //     keyCode
        // });
    },
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
            let ta = document.querySelector(".WriteIndex-titleInput").children[0];
            ta.value = ""
            ta.focus();
            setTimeout(() => {
                clipboard.writeText(this.title);
                win.webContents.paste();
            }, 380);
        }, 880);
        setTimeout(function () {
            win.focus();
            let editorDoc = document.querySelector(".public-DraftStyleDefault-block");
            editorDoc.click();
            setTimeout(function () {
                //this.cleanContent(win);
                const selection = window.getSelection();
                const range = document.createRange();
                range.selectNodeContents(editorDoc.parentNode.parentNode.parentNode);
                selection.removeAllRanges();
                selection.addRange(range);
                clipboard.writeHTML(this.doc.body.innerHTML);
                win.webContents.paste();
                ipcRenderer.send('articlePublishMain', {
                    siteId: 'zhihu',
                    url: window.location.href
                });
            }.bind(this), 980);
        }.bind(this), 1600);
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