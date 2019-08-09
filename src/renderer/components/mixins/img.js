const fs = require('fs');
const path = require('path');
import compressor from 'compressorjs';
const request = require('request');
const url = require("url");
export default {
    data() {
        return {
            fileTypeMap: {
                "jpeg": "image/jpeg",
                "jpg": "image/jpeg",
                "png": "image/png",
                "gif": "image/gif",
                "bmp": 'image/bmp',
                "webp": 'image/webp'
            }
        }
    },
    methods: {
        imgCompress(fullName, cb) {
            let data = fs.readFileSync(fullName);
            let file = new File([data], path.basename(fullName), { type: this.fileTypeMap[path.extname(fullName).substr(1)] });
            if (this.$root.imgWidth > 0 && this.$root.imgHight > 0) {
                new compressor(file, {
                    quality: 0.8,
                    success(result) {
                        fs.unlinkSync(fullName)
                        let fr = new FileReader();
                        fr.onload = () => {
                            if (fr.readyState == 2) {
                                var buffer = new Buffer.from(fr.result);
                                fs.writeFileSync(fullName, buffer);
                                cb();
                            }
                        };
                        fr.readAsArrayBuffer(result);
                    },
                    error(err) {
                        console.log(err.message);
                    },
                });
            }
        },
        // imgCompress(fullName) {
        //     let extIndex = fullName.lastIndexOf('.');
        //     let tempName = fullName.substring(0, extIndex) + "_temp" + fullName.substring(extIndex);
        //     if (this.$root.imgWidth > 0 && this.$root.imgHight > 0) {
        //         sharp(fullName)
        //             .resize(this.$root.imgWidth, this.$root.imgHight, {
        //                 withoutEnlargement: true,
        //                 fit: "inside"
        //             })
        //             .toFile(tempName)
        //             .then(() => {
        //                 fs.unlink(fullName, err => {
        //                     if (!err) {
        //                         fs.rename(tempName, fullName, err => {
        //                             err && console.log(err);
        //                         });
        //                     }
        //                 })
        //             });
        //     }
        // },
        imgSaveBase64Obj(dom) {
            let id = "img" + new Date().getTime();
            let base64Data = dom.src.replace(/^data:([A-Za-z-+/]+);base64,/, '');
            let fullName = path.join(this.articlePath, id + ".png");
            fs.writeFile(fullName, base64Data, 'base64', err => {
                this.imgCompress(fullName);
                document.getElementById("ueditor_0").contentWindow.document.getElementById(id).src = 'file://' + fullName;
            });
            dom.removeAttribute("_src");
            dom.src = 'file://' + fullName;
            dom.id = id;
        },
        imgSaveFileObj(fileObj, cb) {
            let id = "img" + new Date().getTime();
            let fullName = path.join(this.articlePath, id + path.extname(fileObj.name));
            let fr = new FileReader();
            fr.onload = () => {
                if (fr.readyState == 2) {
                    var buffer = new Buffer.from(fr.result);
                    fs.writeFile(fullName, buffer, err => {
                        this.imgCompress(fullName, _ => cb(id, fullName, ''));
                    });
                }
            };
            fr.readAsArrayBuffer(fileObj);
        },
        imgSaveDropObj(fullName, cb) {
            let id = "img" + new Date().getTime();
            let tarName = path.join(this.articlePath, id + path.extname(fullName));
            fs.copyFile(fullName, tarName, err => {
                cb(id, tarName, err);
            });
        },
        removeUselessImg() {
            fs.readdir(this.articlePath, (err, files) => {
                files.forEach(v => {
                    if (!v.startsWith("img")) {
                        return;
                    }
                    let tempStr = 'id="' + v.substring(0, v.lastIndexOf('.')) + '"';
                    if (this.articleContent.includes(tempStr)) {
                        return;
                    }
                    fs.unlink(path.join(this.articlePath, v), err => {
                        if (err) {
                            err && console.log(err);
                        }
                    });
                })
            });
        },
        downloadInternetImg() {
            var editor = CKEDITOR.instances.editorCk;
            editor.document.$.querySelectorAll("img").forEach((dom, index) => {
                if (dom.src.startsWith("http")) {
                    let id = "img" + new Date().getTime() + index;
                    let parsedUrl = url.parse(dom.src);
                    let ext = dom.src.includes("webp") ? ".webp" : path.extname(parsedUrl.pathname);
                    if (!ext) ext = ".png";
                    let fullName = path.join(this.articlePath, id + ext);
                    request(dom.src)
                        .pipe(fs.createWriteStream(fullName))
                        .on('finish', (err) => {
                            this.imgCompress(fullName);
                            dom.src = 'file://' + fullName;
                            dom.dataset["ckeSavedSrc"] = dom.src;
                            dom.id = id;
                            editor.updateElement();
                            this.needSave = true;
                        });
                }
            });
        }
    }
}