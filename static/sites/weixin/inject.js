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
            dom.dataset[this.siteId] = imgObj.message;
            this.guard -= 1;
            if (this.guard < 1) {
                var html = this.doc.body.innerHTML;
                ipcRenderer.send('contentRefreshMain', {
                    content: html
                });
                this.end();
            }
        });
    },
    end() {
        this.imgs.forEach(v => {
            v.src = v.dataset[this.siteId]
            Object.keys(v.dataset).forEach(ds => {
                delete v.dataset[ds];
            })
        });
        UE.instants.ueditorInstant0.setContent(this.doc.body.innerHTML);
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
            blogEditor.setContent(this.doc.body.innerHTML);
        }
    }
}

ipcRenderer.on('message', (event, article) => {
    window.onbeforeunload = null;
    let url = window.location.href;
    let token = base.getUrlParam(url, "token");
    let type = base.getUrlParam(url, "type");
    if (token && !type) {
        //todo isnew
        window.location.href = "https://mp.weixin.qq.com/cgi-bin/appmsg?t=media/appmsg_edit_v2&action=edit&isNew=1&type=10&lang=zh_CN&token=" + token;
        return;
    }
    if (token && type == "10") {
        setTimeout(function () {
            document.getElementById("title").value = article.title;
            imgProcessor.init(article);
        }, 860);
    }
    return; 
    ///https://mp.weixin.qq.com/cgi-bin/filetransfer?action=upload_material&f=json&writetype=doublewrite&groupid=3&ticket_id=gh_60653c52cd64&ticket=2d6e13e4acdb3bfe4099d7ae504a61c976686774&svr_time=1551272627&token=1429231721&lang=zh_CN
// query string
    // action: upload_material
    // f: json
    // writetype: doublewrite
    // groupid: 3   文章配图
    // ticket_id: gh_60653c52cd64  cookie
    // ticket: 2d6e13e4acdb3bfe4099d7ae504a61c976686774  //wx.commonData.data.ticket
    // svr_time: 1551268997  //wx.cgiData.svr_time
    // token: 1429231721  //wx.commonData.data.param: "&token=1429231721&lang=zh_CN"
    // lang: zh_CN     
    // seq: 1
//form data
    // id: WU_FILE_0  //0,1,2,3,4...
    // name: 1550411785681.jpg
    // type: image/jpeg
    // lastModifiedDate: Sun Feb 17 2019 21:56:29 GMT 0800 (中国标准时间)
    // size: 277022
    // file: (binary)


    //https://mp.weixin.qq.com/cgi-bin/filetransfer?action=upload_material&f=json&writetype=doublewrite&groupid=3&ticket_id=gh_60653c52cd64&ticket=2d6e13e4acdb3bfe4099d7ae504a61c976686774&svr_time=1551268997&token=1429231721&lang=zh_CN&seq=1
    // if (!token && url != 'https://mp.weixin.qq.com/') {
    //     var url = document.getElementById("TipsPanel_LinkEdit").href
    //     ipcRenderer.send('articleRefreshMain', {
    //         siteId: 'cnblogs',
    //         url: url
    //     });
    //     alert("发布成功!");
    //     remote.shell.openExternal(document.getElementById("TipsPanel_LinkViewPost").href);
    //     remote.getCurrentWindow().close();
    // }
    // //编辑文章的逻辑
    // var titleTb = document.getElementById("Editor_Edit_txbTitle");
    // if (!titleTb) {
    //     return; //没有标题和内容区域，就认定不是文章编辑页面
    // }
    // titleTb.value = article.title;
    // imgProcessor.init(article);
})