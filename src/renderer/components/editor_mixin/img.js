const fs = require('fs');
const path = require('path');
export default{
    data(){
        return {
            imgWidth:1300,
            imgHight:800,
        }
    },
    methods:{
        imgCompress(fullName) {
            let extIndex = fullName.lastIndexOf('.');
            let tempName = fullName.substring(0, extIndex) + "_temp" + fullName.substring(extIndex);
            if (this.imgWidth > 0 && this.imgHight > 0) {
                sharp(fullName)
                    .resize(this.imgWidth, this.imgHight, {
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
        imgSaveBase64Obj(dom) {
            let id = "img" + new Date().getTime();
            let base64Data = dom.src.replace(/^data:([A-Za-z-+/]+);base64,/, '');
            let fullName = path.join(this.articlePath, id + ".png");
            fs.writeFile(fullName, base64Data, 'base64', err => {
                this.compress(fullName);
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
        imgSaveInternetObj(dom) {
            let id = "img" + new Date().getTime();
            let parsedUrl = url.parse(dom.src);
            let ext = dom.src.includes("webp") ? ".webp" : path.extname(parsedUrl.pathname)
            let fullName = path.join(this.articlePath, id + ext);
            request(dom.src)
                .pipe(fs.createWriteStream(fullName))
                .on('finish', (err) => {
                    this.compress(fullName);
                    document.getElementById("ueditor_0").contentWindow.document.getElementById(id).src = 'file://' + fullName;
                });
            dom.removeAttribute("_src");
            dom.src = 'file://' + fullName;
            dom.id = id;
        }
    }
}