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
        let fd = new FormData();
        fd.append('imageFile', file);
        fd.append("mimeType", file.type);
        let url = 'https://upload.cnblogs.com/imageuploader/CorsUpload';
        base.post(url, fd, (r) => {
            var imgObj = JSON.parse(r);
            dom.src = imgObj.message;
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
            if(v.dataset[this.siteId]){
                v.src = v.dataset[this.siteId];
            }
            Object.keys(v.dataset).forEach(ds => {
                delete v.dataset[ds];
            })
        });
        blogEditor.setContent(this.doc.body.innerHTML);
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
        if (this.imgs.length > 0) {
            this.start();
        } else {
            blogEditor.setContent(this.doc.body.innerHTML);
        }
    }
}

ipcRenderer.on('message', (event, article) => {
    window.onbeforeunload = null;
    if (window.location.href.startsWith('https://i.cnblogs.com/PostDone.aspx')) {
        var url = document.getElementById("TipsPanel_LinkEdit").href
        ipcRenderer.send('articlePublishMain', {
            siteId: 'cnblogs',
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