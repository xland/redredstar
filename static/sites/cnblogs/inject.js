const fs = require('fs');
const path = require('path');
const {
    clipboard,
    ipcRenderer
} = require('electron');
const {
    BrowserWindow
} = require('electron').remote;

let imgProcessor = {
    mime: {
        "jpeg": "image/jpeg",
        "jpg": "image/jpeg",
        "png": "image/png",
        "gif": "image/gif",
        "bmp": 'image/bmp'
    },
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
        //xhr.setRequestHeader("Cache-Control", "no-cache");
        xhr.onreadystatechange = () => {
            if (xhr.readyState == 4 && (xhr.status == 200 || xhr.status == 304)) {
                var imgObj = JSON.parse(xhr.responseText);
                dom.dataset[this.siteId] = imgObj.message;
                this.guard -= 1;
                if (this.guard < 1) {
                    ipcRenderer.send('articleRefreshMain', {
                        content : this.doc.body.innerHTML
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
                    type: this.mime[extname]
                });
                this.uploadImg(v, file);
            }
            if (this.guard < 1) {
                this.end();
            }
        });
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

document.addEventListener("DOMContentLoaded", function () {
    console.log(BrowserWindow);
    var titleTb = document.getElementById("Editor_Edit_txbTitle");
    if (!titleTb) {
        return; //没有标题和内容区域，就认定不是文章编辑页面
    }
    ipcRenderer.on('message', (event, article) => {
        window.onbeforeunload = null;
        titleTb.value = article.title;
        imgProcessor.init(article);



        // var re = new RegExp("data-img_" + article.siteId, "gi");
        // content = article.content.replace(re, 'src');
        // content = content.replace(/data-img_.+?=".+?"/gi, '');

        // var win = BrowserWindow.fromId(article.winId);
        // win.focus();
        // clipboard.writeHTML(article.content)
        // tinyMCE.getInstanceById('Editor_Edit_EditorBody').focus();
        // win.webContents.paste();
    })
});