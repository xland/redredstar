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
    resultHtml:'',
    finished:false,
    uploadImg(dom, file) {
        var xhr = new XMLHttpRequest();
        xhr.open("GET", 'https://www.jianshu.com/upload_images/token.json?filename=' + file.name, true);
        xhr.onreadystatechange = () => {
            if (xhr.readyState == 4 && (xhr.status == 200 || xhr.status == 304)) {
                var r = JSON.parse(xhr.responseText);
                var formData = new FormData();
                formData.append("token", r.token);
                formData.append("key", r.key);
                formData.append("file", file);
                formData.append("x:protocol", 'https');
                var xhr2 = new XMLHttpRequest();
                xhr2.open("POST", 'https://upload.qiniup.com/', true);
                xhr2.setRequestHeader("Cache-Control", "no-cache");
                xhr2.onreadystatechange = () => {
                    if (xhr2.readyState == 4 && (xhr2.status == 200 || xhr2.status == 304)) {
                        var imgObj = JSON.parse(xhr2.responseText);
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
                xhr2.send(formData);
            }
        }
        xhr.send();
    },
    end() {
        this.imgs.forEach(v => {
            v.src = v.dataset[this.siteId]
            Object.keys(v.dataset).forEach(ds => {
                delete v.dataset[ds];
            })
        });
        clipboard.writeHTML(this.doc.body.innerHTML);
        var win = remote.BrowserWindow.fromId(this.winId);
        win.focus();
        document.getElementsByClassName("kalamu-area")[0].focus();
        win.webContents.paste();
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
    var win = remote.BrowserWindow.fromId(article.winId);
    win.webContents.on('dom-ready', () => {
        alert(1);
    })
    var url = window.location.href;
    if (url == 'https://www.jianshu.com/sign_in') {
        return;
    }
    if (!url.startsWith('https://www.jianshu.com/writer#/')) {
        window.location.href = article.url;
        return;
    }
    if (article.type == "new") {
        var open = window.XMLHttpRequest.prototype.open;
        window.XMLHttpRequest.prototype.open = function (method, url, async, user, pass) {
            this.addEventListener("readystatechange", function () {
                if (this.readyState === 4) {
                    var obj = JSON.parse(this.responseText);
                    //必须得确认这是一篇新建的文章才能往里写内容
                    if (obj && obj.autosave_control == 0 && obj.content_updated_at) {
                        var newTime = parseInt(obj.content_updated_at + "000")
                        var curTime = new Date().getTime();
                        if (curTime - newTime < 6000) {
                            document.getElementsByClassName("_24i7u")[0].addEventListener("change", function(){
                                
                            }
                            setTimeout(function(){
                                imgProcessor.init(article);
                                document.getElementsByClassName("_24i7u")[0].value = article.title;
                            },800);
                        }
                    }
                }
            }, false);
            open.apply(this, arguments);
        };
        setTimeout(function(){
            alert("请先选择文集并新建文章");
        },800);
    }
    // if (window.location.href.startsWith('https://i.cnblogs.com/PostDone.aspx')) {
    //     var url = document.getElementById("TipsPanel_LinkEdit").href
    //     ipcRenderer.send('articleRefreshMain', {
    //         url: url
    //     });
    //     alert("发布成功!");
    //     remote.shell.openExternal(document.getElementById("TipsPanel_LinkViewPost").href);
    //     remote.getCurrentWindow().close();
    // }
    //编辑文章的逻辑
    var titleTb = document.getElementById("Editor_Edit_txbTitle");
    if (!titleTb) {
        return; //没有标题和内容区域，就认定不是文章编辑页面
    }
    titleTb.value = article.title;
    
})