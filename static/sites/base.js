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
        window.XMLHttpRequest.prototype.open = function (method, url, async, user, pass) {
            this.addEventListener("readystatechange", function () {
                if (this.readyState === 4) {
                    try {
                        var obj = null;
                        if(this.responseType == "json") {
                            obj = this.response;
                        } else{
                            obj = JSON.parse(this.responseText);
                        }
                        obj.ajax_post_url = this.responseURL;
                        cb(obj);
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
        setInterval(() => {
            window.onbeforeunload = null;
        }, 560);
    }
}