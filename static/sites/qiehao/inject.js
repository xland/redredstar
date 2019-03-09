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
    title: '',
    uploadImg(dom, file) {
        let token = CKEDITOR.tools.getCsrfToken();
        let fd = new FormData();
        fd.append('upload', file);
        fd.append("ckCsrfToken", token);
        let url = "https://mp.csdn.net/" + CKEDITOR.instances["editor"].config.imageUploadUrl;
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
            Object.keys(v.dataset).forEach(ds => {
                delete v.dataset[ds];
            })
        });
        window.onbeforeunload = null;
        CKEDITOR.instances["editor"].setData(this.doc.body.innerHTML)
        document.getElementById("txtTitle").value = this.title;
        base.ajaxInjector(obj => {
            if (obj && obj.data) {
                let id = obj.data.substr(obj.data.lastIndexOf('/') + 1);
                if (!id) {
                    return;
                }
                ipcRenderer.send('articlePublishMain', {
                    siteId: 'csdn',
                    url: 'https://mp.csdn.net/postedit/' + id
                });
            }
        })
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
        this.title = article.title;
        if (this.imgs.length > 0) {
            this.start();
        } else {
            this.end();
        }
    }
}

//UE.instants.ueditorInstant0.setContent("allen");
//https://om.qq.com/image/archscaleupload?isRetImgAttr=1&relogin=1
//img up response {"response":{"code":0,"msg":"success!","cost":"290ms"},"data":{"url":{"url":"http:\/\/inews.gtimg.com\/newsapp_bt\/0\/80214
ipcRenderer.on('message', (event, article) => {
    window.onbeforeunload = null;
    let url = window.location.href;
    if (url.startsWith('https://mp.csdn.net/postedit')) {
        setTimeout(()=>{
            if(!document.getElementById("cke_editor")){
                alert("抱歉：目前暂不支持csdn的markdown编辑器")
            }
            imgProcessor.init(article);
        },960)
    }
})