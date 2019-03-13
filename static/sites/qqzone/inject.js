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
    getUploadUrl(cb) {
        let urlParams = {
            action: 'upload_material',
            f: 'json',
            writetype: 'doublewrite',
            groupid: 3,
            ticket_id: '',
            ticket: wx.commonData.data.ticket,
            svr_time: wx.cgiData.svr_time
        }
        let postUrl = 'https://mp.weixin.qq.com/cgi-bin/filetransfer?'
        remote.session.defaultSession.cookies.get({}, (error, cookies) => {
            urlParams.ticket_id = cookies.find(v => v.name == "ticket_id").value;
            Object.keys(urlParams).forEach(key => {
                postUrl += key + '=' + urlParams[key] + "&";
            });
            postUrl += wx.commonData.data.param.substr(1); //"&token=1429231721&lang=zh_CN"
            cb(postUrl);
        })
    },
    uploadImg(dom, file) {
        let fd = new FormData();
        fd.append('id', 'WU_FILE_' + this.guard);
        fd.append("type", file.type);
        fd.append("lastModifiedDate", new Date());
        fd.append("size", file.length);
        fd.append("file", file);
        this.getUploadUrl((url) => {
            base.post(url, fd, (r) => {
                var imgObj = JSON.parse(r);
                dom.src = imgObj.cdn_url;
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
        setTimeout(()=>{
            window.onbeforeunload = null;
            UE.instants.ueditorInstant0.setContent(this.doc.body.innerHTML);
            document.getElementById("title").value = this.title;
        },600);
        base.ajaxInjector(obj => {
            if (obj && obj.appMsgId) {
                let url = 'https://mp.weixin.qq.com/?appmsgid=' + obj.appMsgId;
                ipcRenderer.send('articlePublishMain', {
                    siteId: 'weixin',
                    url
                });
            }
        })
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
        this.title = article.title;
        if (this.imgs.length > 0) {
            this.start();
        } else {
            this.end();
        }
    }
}

var waitForReady = function (cb) {
    setTimeout(function () {
        if (!document.getElementById("ueditor_0")) {
            console.log('wait');
            waitForReady(cb);
            return;
        }
        console.log('go');
        cb();
    }, 280);
}

ipcRenderer.on('message', (event, article) => {
    let url = window.location.href;
    if(article.type == "new"){
        if (url.startsWith('https://rc.qzone.qq.com/blog/add')) {

        }
    }
    else{
        if(!window.VEDITOR_SIDEALBUM){
            document.getElementById("tblog").contentWindow.document.getElementById("ModifyListAreaDesc")
        }else{
            //todo：上传图片居然是用webworker搞得
        }
    }
    //https://rc.qzone.qq.com/proxy/domain/qzs.qq.com/qzone/newblog/blogcanvas.html#g_iframedescend=1&uin=412588801&pfid=2&qz_ver=8&appcanvas=0&qz_style=35&params=edit&entertime=1551917151353&canvastype=&blogid=1478598367&cdn_use_https=1
    //https://rc.qzone.qq.com/proxy/domain/qzs.qq.com/qzone/newblog/blogcanvas.html#g_iframedescend=1&uin=412588801&pfid=2&qz_ver=8&appcanvas=0&qz_style=35&params=edit&entertime=1551917151353&canvastype=&blogid=1478598367&cdn_use_https=1
    //https://user.qzone.qq.com/proxy/domain/qzs.qq.com/qzone/newblog/blogcanvas.html#g_iframedescend=1&uin=412588801&pfid=2&qz_ver=8&appcanvas=0&qz_style=35&params=&entertime=1551916827322&canvastype=&cdn_use_https=1
    // if (token && !type && article.type == "edit") {
    //     window.location.href = article.url + wx.commonData.data.param;
    //     return;
    // }
    // if (token && type == "10") {
    //     waitForReady(function () {
    //         window.onbeforeunload = null;
    //         imgProcessor.init(article);
    //     });
    // }
    return;
})