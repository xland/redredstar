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
        let token = CKEDITOR.tools.getCsrfToken();
        let fd = new FormData();
        fd.append('upload', file);
        fd.append("ckCsrfToken", token);
        let url = CKEDITOR.instances["body"].config.uploadUrl;
        base.post(url, fd, (r) => {
            var imgObj = JSON.parse(r);
            dom.src = imgObj.url;
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
        CKEDITOR.instances["body"].setData(this.doc.body.innerHTML);
        base.clearMask();
    },
    start() {
        base.maskPage();
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
    base.removeBeforUnload();
    let url = window.location.href;
    let userId = $(".go-inbox").find("a").attr("href");
    let baseUrl = "";
    if (userId) {
        baseUrl = userId.replace("/admin/inbox", '');
        userId = baseUrl.substr(baseUrl.lastIndexOf('/') + 1);
    }
    //如果没登录，那么就去登录
    if (!url.includes('/blog/write') && !userId && !url.includes("/home/login")) {
        window.location.href = 'https://www.oschina.net/home/login';
        return;
    }
    //如果现在在登录页面，就不用跳转了
    if (url.includes("/home/login")) {
        return;
    }
    if ($("h2.header").text().trim().includes(article.title)) {
        let id = $(".article-like")[0].dataset.id;
        let editUrl = baseUrl + '/blog/write/' + id;
        let showUrl = baseUrl + '/blog/' + id;
        ipcRenderer.send('articlePublishMain', {
            siteId: 'oschina',
            url: editUrl
        });
        remote.shell.openExternal(showUrl);
        remote.getCurrentWindow().close();
        return;
    }
    if (!url.includes('/blog/write')) {
        if (article.type == "new") {
            window.location.href = baseUrl + "/blog/write";
        } else {
            window.location.href = article.url;
        }
        return;
    }
    if ($("input[name='title']")[0]) {
        if (!CKEDITOR.instances["body"]) {
            alert("抱歉：目前暂不支持osc的markdown编辑器");
            return;
        }
        setTimeout(function() {
            imgProcessor.init(article);
            $("input[name='title']").val(article.title);
        }, 980);
        return;
    }
})