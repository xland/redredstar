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
        fd.append("id", "WU_FILE_" + this.guard);
        fd.append("name", file.name);
        fd.append("type", file.type);
        fd.append("lastModifiedDate", new Date());
        fd.append("size", file.size);
        fd.append("upfile", file);
        let url = `https://ns.dayu.com/article/imageUpload?appid=website&fromMaterial=0&wmid=${globalConfig.wmid}&wmname=${encodeURIComponent(globalConfig.weMediaName)}&sign=${globalConfig.nsImageUploadSign}`;
        base.post(url, fd, (r) => {
            var imgObj = JSON.parse(r);
            dom.src = imgObj.data.imgInfo.url;
            ipcRenderer.send('imgUploadMain', {
                id: dom.id,
                siteId: this.siteId,
                url: dom.src
            });
            this.guard -= 1;
            if (this.guard < 1) {
                this.end();
            }
        }, {}, false)
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
        UE.instants.ueditorInstant0.setContent(this.doc.body.innerHTML);
        document.querySelector("#title").value = this.title;
        base.ajaxInjector((obj,url) => {
            if (obj && obj.data && obj.data._id && url.includes("save-draft")) {
                ipcRenderer.send('articlePublishMain', {
                    siteId: this.siteId,
                    url: 'https://mp.dayu.com/dashboard/article/write?draft_id=' + obj.data._id
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
        cb();
    }, 380);
}

ipcRenderer.on('message', (event, article) => {
    base.removeBeforUnload();
    waitForReady(() => {
        imgProcessor.init(article);
    })
})