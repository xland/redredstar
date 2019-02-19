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
    uploadImg(dom, file) {
        var formData = new FormData();
        formData.append('imageFile', file);
        formData.append("mimeType", file.type);
        var xhr = new XMLHttpRequest();
        xhr.open("POST", 'https://upload.cnblogs.com/imageuploader/CorsUpload', true);
        xhr.withCredentials = true;
        xhr.onreadystatechange = () => {
            if (xhr.readyState == 4 && (xhr.status == 200 || xhr.status == 304)) {
                var imgObj = JSON.parse(xhr.responseText);
                dom.dataset[this.siteId] = imgObj.message;
                this.guard -= 1;
                if (this.guard < 1) {
                    ipcRenderer.send('contentRefreshMain', {
                        content: this.doc.body.innerHTML
                    });
                    this.end();
                }
            }
        };
        xhr.send(formData);
    },
    end() {
        //todo: give it to main window
        this.imgs.forEach(v => {
            v.src = v.dataset[this.siteId]
            Object.keys(v.dataset).forEach(ds => {
                delete v.dataset[ds];
            })
        });
        blogEditor.setContent(this.doc.body.innerHTML);
        // var win = BrowserWindow.fromId(this.winId);
        // win.focus();
        // clipboard.writeHTML(this.doc.body.innerHTML);
        // tinyMCE.getInstanceById('Editor_Edit_EditorBody').focus();
        // win.webContents.paste();
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
        this.siteId = article.siteId;
        this.winId = article.winId;
        if (this.imgs.length > 0) {
            this.start();
        } else {
            this.end();
        }
    }
}

ipcRenderer.on('message', (event, article) => {
    window.onbeforeunload = null;
    if (window.location.href.startsWith('https://i.cnblogs.com/PostDone.aspx')) {
        var url = document.getElementById("TipsPanel_LinkEdit").href
        ipcRenderer.send('articleRefreshMain', {
            url: url
        });
        alert("发布成功!");
        remote.shell.openExternal(document.getElementById("TipsPanel_LinkViewPost").href);
        remote.getCurrentWindow().close();
    }
    //编辑文章的逻辑
    var titleTb = document.getElementById("Editor_Edit_txbTitle");
    if (!titleTb) {
        return; //没有标题和内容区域，就认定不是文章编辑页面
    }
    titleTb.value = article.title;
    imgProcessor.init(article);
})