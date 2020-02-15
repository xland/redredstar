const fs = require('fs');
const path = require('path');
const {
    remote
} = require('electron');
module.exports = {
    mime: {
        "jpeg": "image/jpeg",
        "jpg": "image/jpeg",
        "png": "image/png",
        "gif": "image/gif",
        "bmp": 'image/bmp',
        "webp": 'image/webp'
    },
    ajaxInjector(cb) {
        var open = window.XMLHttpRequest.prototype.open;
        window.XMLHttpRequest.prototype.open = function(method, url, async, user, pass) {
            this.addEventListener("readystatechange", function() {
                if (this.readyState === 4) {
                    try {
                        var obj = null;
                        if (this.responseType == "json") {
                            obj = this.response;
                        } else {
                            obj = JSON.parse(this.responseText);
                        }
                        cb(obj, this.responseURL);
                    } catch (error) {
                        cb(null);
                    }
                }
            }, false);
            open.apply(this, arguments);
        }
    },
    post(url, form, cb, headers = {}, withCredentials = true) {
        var xhr = new XMLHttpRequest();
        xhr.open("POST", url, true);
        xhr.withCredentials = withCredentials;
        Object.keys(headers).forEach(key => {
            xhr.setRequestHeader(key, headers[key]);
        });
        xhr.onreadystatechange = () => {
            if (xhr.readyState == 4) {
                if (xhr.status == 200 || xhr.status == 304) {
                    cb(xhr.responseText)
                } else {
                    alert("错误：可能的原因是目标网站不支持您上传的图片");
                }
            } else {
                //alert("错误：可能的原因是目标网站不支持您上传的图片");
            }
        }
        xhr.send(form);
    },
    get(url, cb) {
        var xhr = new XMLHttpRequest();
        xhr.open("GET", url, true);
        xhr.onreadystatechange = () => {
            if (xhr.readyState == 4) {
                if (xhr.status == 200 || xhr.status == 304) {
                    cb(xhr.responseText)
                } else {
                    alert("错误：可能的原因是目标网站不支持您上传的图片");
                }
            } else {
                //alert("错误：可能的原因是目标网站不支持您上传的图片");
            }
        }
        xhr.send();
    },
    getUrlParam(url, name) {
        let paramStr = url.split('?')[1];
        if (!paramStr) {
            return null;
        }
        var reg = new RegExp("(^|&)" + name + "=([^&]*)(&|$)", "i");
        var r = paramStr.match(reg);
        if (r != null) return unescape(r[2]);
        return null;
    },
    getCookieParam(cookie, name) {
        let filter = new RegExp(name + "=([^;]*)(;|$)");
        let matches = cookie.match(filter);
        return matches ? matches[1] : null;
    },
    getFileObjByLocalUrl(url) {
        let pathIndex = remote.process.platform == "win32" ? 8 : 7
        let filePath = decodeURI(url).substr(pathIndex);
        let extname = path.extname(filePath).substr(1);
        let buffer = fs.readFileSync(filePath);
        let file = new window.File([Uint8Array.from(buffer)], path.basename(filePath), {
            type: this.mime[extname]
        });
        return file;
    },
    removeBeforUnload() {
        var wc = remote.getCurrentWebContents();
        wc.on('will-prevent-unload', event => {
            var win = remote.BrowserWindow.fromWebContents(wc);
            win.destroy();
        })
    },
    maskPage() {
        let temp = document.createElement("div");　　
        temp.innerHTML = `<div id="xxm__post__mask"
        style="position:absolute;left:0px;right:0px;bottom:0px;top:0px;z-index:99999999;background:rgba(0,0,0,0.8);text-align:center;color:#fff;"
      >
        <div style="font-size:46px;padding-top:160px;font-weight:600;">正在提交...</div>
        <div style="font-size:22px;padding-top:20px;">目标平台编辑器需设置为HTML编辑器<br />确保文章内图片无差错</div>
      </div>`;
        document.body.appendChild(temp.children[0]);
    },
    clearMask() {
        let dom = document.querySelector("#xxm__post__mask");
        if (dom) document.body.removeChild(dom);
    }
}