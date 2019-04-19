const fs = require('fs');
const path = require('path');
const {
    ipcRenderer,
    remote
} = require('electron');
export default {
    data() {
        return {
            jnaGard: 0,
            jnaUrl:'https://jiaonia.com',
            //jnaUrl:'http://localhost:52081',
        }
    },
    methods: {
        jnaGetFileObjByLocalUrl(url) {
            let pathIndex = remote.process.platform == "win32" ? 8 : 7
            let filePath = decodeURI(url).substr(pathIndex);
            let extname = path.extname(filePath).substr(1);
            let buffer = fs.readFileSync(filePath);
            let file = new window.File([Uint8Array.from(buffer)], path.basename(filePath), {
                type: this.fileTypeMap[extname]
            });
            return file;
        },
        jnaUploadImg(doc, cb) {
            let imgs = doc.querySelectorAll('img');
            imgs.forEach(v => {
                let file = this.jnaGetFileObjByLocalUrl(v.src);
                let imgFd = new FormData();
                imgFd.append("token", this.$root.jnaToken);
                imgFd.append("answerImg", file);
                window.xxmPost(this.jnaUrl+"/Xxm/PublishArticleImg", imgFd, rt => {
                    this.jnaGard += 1;
                    var imgObj = JSON.parse(rt);
                    v.src = imgObj.url;
                    ipcRenderer.send('imgUploadMain', {
                        id: v.id,
                        siteId: "jiaonia",
                        url: v.src
                    });
                    if (this.jnaGard == imgs.length) {
                        cb();
                    }
                });
            });
            if (imgs.length < 1) {
                cb();
            }
        },
        jnaPublish() {
            if (!this.$root.jnaSync || !this.$root.jnaToken) {
                return;
            }
            let articleId = this.$parent.article.id;
            let articleTitle = this.$parent.article.title;
            let content = "";
            if (this.$parent.article.editor_type == "html") {
                content = window.CKEDITOR.instances.editorCk.getData();
            } else {
                content = window.editorMd.getHtml();
            }
            let parser = new DOMParser();
            let doc = parser.parseFromString(content, "text/html");
            this.jnaUploadImg(doc, () => {
                let fd = new FormData();
                fd.append("token", this.$root.jnaToken);
                fd.append("title", articleTitle);
                fd.append("content", doc.body.innerHTML);
                this.db("article_site").where({
                    "article_id": articleId,
                    "site_id": "jiaonia"
                }).then(rows => {
                    if (rows.length > 0) {
                        let eurlObj = JSON.parse(rows[0].edit_url);
                        fd.append("qId", eurlObj.qId);
                        window.xxmPost(this.jnaUrl+"/Xxm/EditArticle", fd, rt => {
                            this.db("article_site").update({
                                edit_url: rt
                            }).where("id", rows[0].id).then();
                        });
                    } else {
                        window.xxmPost(this.jnaUrl+"/Xxm/PublishArticle", fd, rt => {
                            this.db("article_site").insert({
                                article_id: articleId,
                                site_id: 'jiaonia',
                                edit_url: rt
                            }).then();
                        });
                    };
                });
            });
        }
    }
}