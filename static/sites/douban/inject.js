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
        fd.append("note_id", window._NOTE_ID);
        fd.append("ck", document.getElementsByName("ck")[0].value);
        fd.append("folder", "/note/");
        fd.append("upload_auth_token", _POST_PARAMS.siteCookie.value);
        fd.append("image_file", file);
        let url = "https://www.douban.com/j/note/add_photo";
        base.post(url, fd, (r) => {
            var imgObj = JSON.parse(r);
            dom.src = imgObj.photo.url;
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
        var win = remote.BrowserWindow.fromId(this.winId);
        win.focus();
        let editorDoc = document.querySelector(".public-DraftEditor-content");
        editorDoc.click();
        var selection = window.getSelection();
        var range = document.createRange();
        range.selectNode(editorDoc.children[0])
        selection.removeAllRanges();
        selection.addRange(range);
        setTimeout(() => {
            clipboard.writeHTML(this.doc.body.innerHTML);
            win.webContents.paste();
            let titleTb = document.querySelector(".note-editor-input");
            titleTb.click();
            titleTb.focus();
            titleTb.value = "";
            setTimeout(() => {
                clipboard.writeText(this.title);
                win.webContents.paste();
            }, 580)
        }, 580)
        ipcRenderer.send('articlePublishMain', {
            siteId: this.siteId,
            url: 'https://www.douban.com/note/' + window._NOTE_ID + '/edit'
        });
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
        if (!Draft && !Draft.Editor) {
            waitForReady(cb);
            return;
        }
        cb();
    }, 380);
}
ipcRenderer.on('message', (event, article) => {
    base.removeBeforUnload();
    let url = window.location.href;
    if (url.endsWith("login")) {
        return;
    }
    if (url != article.url) {
        window.location.href = article.url;
        return;
    }
    waitForReady(() => {
        imgProcessor.init(article);
    })
})