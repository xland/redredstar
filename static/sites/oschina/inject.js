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
        let token = CKEDITOR.tools.getCsrfToken();
        let fd = new FormData();
        fd.append('upload', file);
        fd.append("ckCsrfToken", token);
        let url = CKEDITOR.instances["body"].config.uploadUrl;
        base.post(url, fd, (r) => {
            var imgObj = JSON.parse(r);
            dom.dataset[this.siteId] = imgObj.url;
            this.guard -= 1;
            if (this.guard < 1) {
                var html = this.doc.body.innerHTML;
                ipcRenderer.send('contentRefreshMain', {
                    content: html
                });
                this.end();
            }
        })
    },
    end() {
        this.imgs.forEach(v => {
            v.src = v.dataset[this.siteId]
            Object.keys(v.dataset).forEach(ds => {
                delete v.dataset[ds];
            })
        });
        CKEDITOR.instances["body"].setData(this.doc.body.innerHTML);
    },
    start() {
        this.imgs.forEach(v => {
            if (!v.dataset[this.siteId]) {
                this.guard += 1;
                var pathIndex = remote.process.platform == "win32" ? 8 : 7
                var filePath = decodeURI(v.src).substr(pathIndex);
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
    let url = window.location.href;
    let userId = $(".go-inbox").find("a").attr("href");
    let baseUrl = "";
    if(userId){
        baseUrl = userId.replace("/admin/inbox",'');
        userId = baseUrl.substr(baseUrl.lastIndexOf('/')+1);
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
        ipcRenderer.send('articleRefreshMain', {
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
        if(!CKEDITOR.instances["body"]){
            alert("抱歉：目前暂不支持osc的markdown编辑器");
            return;
        }
        setTimeout(function(){
            imgProcessor.init(article);
            $("input[name='title']").val(article.title);
        },980);
        return;
    }
})