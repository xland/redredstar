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
        fd.append("name", file.name);
        let extname = path.extname(file.name).substr(1);
        fd.append("file_type", extname);
        fd.append("file_size", file.size);
        fd.append("auth_token", base.getCookieParam(document.cookie, "P00001"));
        fd.append("business_type", "image");
        fd.append("share_type", "external");
        fd.append("share_expire", 0);
        let userInfo = base.getCookieParam(document.cookie, "P00002")
        userInfo = decodeURIComponent(userInfo);
        userInfo = JSON.parse(userInfo);
        fd.append("uid", userInfo.uid);
        fd.append("role", "paopao_image");
        fd.append("bizid", "jy_opendep_prod");
        fd.append("file", file);
        let url = "https://upload.iqiyi.com/common_upload";
        base.post(url, fd, (r) => {
            var imgObj = JSON.parse(r);
            dom.src = imgObj.data.share_url;
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
        document.querySelector(".editorTitle input").value = this.title;
        base.ajaxInjector(obj => {
            if (obj && obj.data && obj.msg && obj.ajax_post_url.includes("save_draft")) {
                ipcRenderer.send('articlePublishMain', {
                    siteId: this.siteId,
                    url: 'https://mp.iqiyi.com/publish/content/article?id='+obj.data+'&status=4&source=ap'
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
        if (typeof UE == "undefined" || typeof UE.instants == "undefined") {
            waitForReady(cb);
            return;
        }
        cb();
    }, 380);
}
var waitForIndex = function (cb) {
    setTimeout(function () {
        let goBtn = document.querySelector(".home-go-write");
        if (!goBtn) {
            waitForIndex(cb);
            return;
        }
        cb(goBtn);
    }, 280);
}

ipcRenderer.on('message', (event, article) => {
    base.removeBeforUnload();
    if (window.location.href == "https://mp.iqiyi.com/") {
        if (document.querySelector(".login-btn")) {
            return;
        } else {
            window.location.href = article.url;
        }
        return;
    }
    waitForReady(() => {
        imgProcessor.init(article);
    })
})