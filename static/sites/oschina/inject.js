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
    userId:'',
    uploadImg(dom, file) {
        var token = CKEDITOR.tools.getCsrfToken();
        var formData = new FormData();
        formData.append('upload', file);
        formData.append("ckCsrfToken", token);
        var xhr = new XMLHttpRequest();
        xhr.open("POST", 'https://my.oschina.net/u/'+this.userId+'/space/ckeditor_dialog_img_upload', true);
        xhr.withCredentials = true;
        xhr.onreadystatechange = () => {
            if (xhr.readyState == 4 && (xhr.status == 200 || xhr.status == 304)) {
                var imgObj = JSON.parse(xhr.responseText);
                dom.dataset[this.siteId] = imgObj.url;
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
                var pathIndex = remote.process.platform == "win32"?8:7
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
        this.userId = article.userId;
        if (this.imgs.length > 0) {
            this.start();
        } else {
            CKEDITOR.instances["body"].setData(this.doc.body.innerHTML);
        }
    }
}

ipcRenderer.on('message', (event, article) => {
    window.onbeforeunload = null;
    var url = window.location.href;
    var userId = $("#headerNavMenu").find(".osc-avatar").attr("data-user-id");
    if (!url.includes('/blog/write') && !userId && !url.includes("/home/login")) {
        window.location.href = 'https://www.oschina.net/home/login';
        return;
    }
    if(url.includes("/home/login")){
        return;
    }
    if ($("h2.header").text().trim().includes(article.title)) {
        var id = $(".article-like")[0].dataset.id;
        ipcRenderer.send('articleRefreshMain', {
            siteId: 'oschina',
            url: 'https://my.oschina.net/u/' + userId + '/blog/write/' + id
        });
        return;
    }
    if(!url.includes('/blog/write')){
        if(article.type == "new"){
            window.location.href = "https://my.oschina.net/u/" + userId + "/blog/write";
            return;
        }else{
            window.location.href = article.url;
            return
        }
    }
    if ($("input[name='title']")[0]) {
        article.userId = userId;
        imgProcessor.init(article);
        $("input[name='title']").val(article.title);
        return;
    }
})