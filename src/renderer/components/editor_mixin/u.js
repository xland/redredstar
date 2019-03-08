const fs = require('fs');
const path = require('path');
const {
    ipcRenderer,
    remote
} = require('electron');
import imageProcessor from "../../utils/image";
export default {
    methods:{

        hookWinQuit() {
            var self = this;
            remote.getCurrentWindow().on('close', (event) => {
                event.preventDefault();
                self.saveContent();
                remote.app.quit();
            })
        },
        hookContentChange() {
            var self = this;
            var subContent = document.getElementById("ueditor_0").contentWindow.document;
            subContent.oninput = function (e) {
                self.needSave = true;
            }
        },
        hookContentRefresh() {
            ipcRenderer.on('contentRefreshRenderer', (e, message) => {
                window.UE.instants.ueditorInstant0.setContent(message.content);
                this.needSave = true;
            });
        },
        hookArticleRefresh() {
            ipcRenderer.on('articleRefreshRenderer', (e, message) => {
                this.db('article_site')
                    .where("article_id", this.id)
                    .andWhere("site_id", message.siteId)
                    .select("*").then(rows => {
                        let asObj = {
                            article_id: this.id,
                            site_id: message.siteId,
                            edit_url: message.url
                        }
                        if (rows.length < 1) {
                            this.db("article_site").insert(asObj).then();
                        } else {
                            this.db("article_site").update(asObj).where("id", rows[0].id).then();
                        }
                    });
            });
        },
        hookSaveKeyEvent() {
            var self = this;
            window.saveArticleKeyEvent = function () {
                self.saveContent();
            };
        },
        hookImgDomChange() {
            var editorDocument = document.getElementById("ueditor_0").contentWindow.document;
            var observer = new MutationObserver(records => {
                records.forEach((item, index) => {
                    if (item.removedNodes.length > 0 && item.removedNodes[0].tagName ==
                        "IMG") {
                        let pathIndex = remote.process.platform == "win32" ? 8 : 7
                        let filePath = decodeURI(item.removedNodes[0].src).substr(pathIndex);
                        fs.unlink(filePath, err => {
                            if (err) {
                                err && console.log(err);
                            }
                        });
                        this.needSave = true;
                    }
                    if (item.addedNodes.length > 0 && item.addedNodes[0].tagName ==
                        "IMG" && !item.addedNodes[0].src.startsWith("file")) {
                        if (item.addedNodes[0].src.startsWith("data:")) {
                            imageProcessor.saveBase64Obj(item.addedNodes[0]);
                        } else {
                            imageProcessor.saveInternetObj(item.addedNodes[0]);
                        }
                        this.needSave = true;
                    }
                });
            });
            observer.observe(editorDocument, {
                childList: true,
                subtree: true
            });
        },
        hookPasteImg() {
            var self = this;
            window.editorImgInsert = function (file) {
                imageProcessor.saveFileObj(file, (err) => {
                    self.needSave = true;
                })
            }
        },
        initEditorU() {
            var editor = window.UE.getEditor('editorU');
            var self = this;
            editor.addListener("ready", () => {
                self.hookWinQuit();
                self.hookPasteImg();
                self.hookImgDomChange();
                self.hookSaveKeyEvent();
                self.hookContentChange();
                self.hookContentRefresh();
                self.hookArticleRefresh();
                if (self.articleContent) {
                    editor.setContent(self.articleContent);
                }
            })
        },
    }
}