import sharp from 'sharp';
const fs = require('fs');
const path = require('path');
const request = require('request');
const url = require("url");
const electron = require('electron');
export default {
    savePath: null,
    w: 1300,
    h: 800,
    articleId:-1,
    setArticleId(id){
        this.articleId = id;
        this.savePath = path.join(electron.remote.app.getPath('userData'), "/xxm/" + id);
    },
    setImageSize(w,h){
        this.w = w;
        this.h = h;
    },
    compress(fullName) {
        let w = parseInt(this.w);
        let h = parseInt(this.h);
        let extIndex = fullName.lastIndexOf('.');
        let tempName = fullName.substring(0, extIndex) + "_temp" + fullName.substring(extIndex);
        if (w > 0 && h > 0) {
            sharp(fullName)
                .resize(w, h, {
                    withoutEnlargement: true,
                    fit: "inside"
                })
                .toFile(tempName)
                .then(() => {
                    fs.unlink(fullName, err => {
                        if (!err) {
                            fs.rename(tempName, fullName, err => {
                                err && console.log(err);
                            });
                        }
                    })
                });;
        }
    },
    saveBase64Obj(dom) {
        let id = "img" + new Date().getTime();
        let base64Data = dom.src.replace(/^data:([A-Za-z-+/]+);base64,/, '');
        let fullName = path.join(this.savePath, id + ".png");
        fs.writeFile(fullName, base64Data, 'base64', err => {
            this.compress(fullName);
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
                    this.compress(fullName);
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
                this.compress(fullName);
                document.getElementById("ueditor_0").contentWindow.document.getElementById(id).src = 'file://' + fullName;
            });
        dom.removeAttribute("_src");
        dom.src = 'file://' + fullName;
        dom.id = id;
    },
    delImgWhenDomChange(dom){
        let pathIndex = electron.remote.process.platform == "win32" ? 8 : 7;
        let filePath = decodeURI(dom.src).substr(pathIndex);
        fs.unlink(filePath, err => {
            if (err) {
                err && console.log(err);
            }
        });
    }
}