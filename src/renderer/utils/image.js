const fs = require('fs');
const path = require('path');
const request = require('request');
const url = require("url");
export default {
    savePath: null,
    init(basePath, articleId) {
        this.savePath = path.join(basePath, articleId);
    },
    compress() {

    },
    saveBase64Obj(dom) {
        let id = "img" + new Date().getTime();
        let base64Data = dom.src.replace(/^data:([A-Za-z-+/]+);base64,/, '');
        let fullName = path.join(this.savePath, id + ".png");
        fs.writeFile(fullName, base64Data, 'base64', err => {
            document.getElementById("ueditor_0").contentWindow.document.getElementById(id).src = 'file://' + fullName;
        });
        dom.removeAttribute("_src");
        dom.src = 'file://' + fullName;
        dom.id = id;
    },
    saveFileObj(fileObj, cb) {
        let id = "img" + new Date().getTime();
        let fullName = path.join(this.savePath, id + path.extname(fileObj.name));
        let fr = new FileReader();
        fr.onload = () => {
            if (fr.readyState == 2) {
                var buffer = new Buffer(fr.result);
                fs.writeFile(fullName, buffer, err => {
                    var imgDom = '<img id="' + id + '" src="file://' + fullName + '" />';
                    window.UE.instants.ueditorInstant0.execCommand("inserthtml", imgDom);
                    cb(err);
                });
            }
        };
        fr.readAsArrayBuffer(fileObj);
    },
    saveInternetObj(dom) {
        let id = "img" + new Date().getTime();
        let parsedUrl = url.parse(dom.src);
        let ext = dom.src.includes("webp") ? ".webp" : path.extname(parsedUrl.pathname)
        let fullName = path.join(this.savePath, id + ext);
        request(dom.src)
            .pipe(fs.createWriteStream(fullName))
            .on('finish', (err) => {
                document.getElementById("ueditor_0").contentWindow.document.getElementById(id).src = 'file://' + fullName;
            });
        dom.removeAttribute("_src");
        dom.src = 'file://' + fullName;
        dom.id = id;
    }
}