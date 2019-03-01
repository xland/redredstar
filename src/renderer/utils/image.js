const fs = require('fs');
const path = require('path');
const request = require('request');
const url = require("url");
export default {
    savePath: null,
    init(articleId, basePath) {
        this.savePath = path.join(basePath, articleId);
    },
    compress() {

    },
    saveBase64Obj(urlData, cb) {
        let id = "img" + new Date().getTime();
        let base64Data = urlData.replace(/^data:([A-Za-z-+/]+);base64,/, '');
        let fullName = path.join(this.savePath, id + ".png");
        fs.writeFile(fullName, base64Data, 'base64', err => {
            cb(err);
        });
    },
    saveFileObj(fileObj, cb) {
        let id = "img" + new Date().getTime();
        let fullName = path.join(this.savePath, id + path.extname(fileObj.name));
        let fr = new FileReader();
        fr.onload = () => {
            if (fr.readyState == 2) {
                var buffer = new Buffer(fr.result);
                fs.writeFile(fullName, buffer,err=>{
                    cb(err);
                });
            }
        };
        fr.readAsArrayBuffer(fileObj);
    },
    saveInternetObj(httpUrl, cb) {
        let id = "img" + new Date().getTime();
        let parsedUrl = url.parse(httpUrl);
        let ext = httpUrl.includes("webp") ? ".webp" : path.extname(parsedUrl.pathname)
        let fullName = path.join(this.savePath, id + ext);
        request(httpUrl)
            .pipe(fs.createWriteStream(fullName))
            .on('finish', (err)=>{
                cb(err);
            });
    }
}