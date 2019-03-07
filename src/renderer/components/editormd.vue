<template>
    <div id="editormd">
        <div id="editorContainer"></div>
    </div>
</template>
<script>
    //todo:图片放大缩小需要按比例
    require('codemirror/lib/codemirror.css'); // codemirror
    require('tui-editor/dist/tui-editor.css'); // editor ui
    require('tui-editor/dist/tui-editor-contents.css'); // editor content
    require('highlight.js/styles/github.css'); // code block highlight
    var Editor = require('tui-editor');
    const fs = require('fs');
    const path = require('path');
    import imageProcessor from "../utils/image";
    const {
        ipcRenderer,
        remote
    } = require('electron');
    export default {
        data() {
            return {
                hide: true,
                id: -1,
                tick: null,
                tickStep: 8,
                content: null,
                needSave: false,
                docPath: null,
                rwOption: {
                    encoding: 'utf8'
                }
            }
        },
        methods: {
            initContent() {
                this.docPath = path.join(remote.app.getPath('userData'), "/xxm/" + this.id + "/a.data");
                var content = fs.readFileSync(this.docPath, this.rwOption);
                this.content = content;
                if (UE.instants.ueditorInstant0 && UE.instants.ueditorInstant0.isReady) {
                    window.UE.instants.ueditorInstant0.setContent(content);
                    document.getElementById("ueditor_0").contentWindow.document.documentElement.scrollTop = 0;
                }
                imageProcessor.setArticleId(this.id);
                this.autoSave();
            },
            autoSave() {
                self = this;
                this.tick = setInterval(() => {
                    self.saveContent();
                }, this.tickStep)
            },
            saveContent(cb) {
                if (!this.needSave) {
                    if (cb) {
                        cb();
                    }
                    return;
                }
                this.bus.$emit('saveContent');
                this.content = window.UE.instants.ueditorInstant0.getContent();
                fs.writeFileSync(this.docPath, this.content, this.rwOption);
                this.needSave = false;

                this.db("articles").update({
                    updated_at: new Date()
                }).where("id", this.id).then(rows => {
                    if (cb) {
                        cb();
                    }
                });
            },
            initEditor() {
                this.hookWinQuit();
                this.hookPasteImg();
                this.hookImgDomChange();
                this.hookSaveKeyEvent();
                this.hookContentChange();
                this.hookContentRefresh();
                this.hookArticleRefresh();
            },
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
            }
        },
        mounted() {
            var editor = new Editor({
                el: document.querySelector('#editormd'),
                initialEditType: 'markdown',
                previewStyle: 'vertical',
                height: '300px'
            });
            // this.db("settings").select("*").then(rows => {
            //     this.tickStep = rows[0].autosave_interval * 1000;
            //     imageProcessor.setImageSize(rows[0].img_w, rows[0].img_h)
            // })
            // var editor = window.UE.getEditor('editorContainer');
            // var self = this;
            // editor.addListener("ready", () => {
            //     self.initEditor();
            //     if (self.content) {
            //         editor.setContent(self.content);
            //     }
            // })
            // this.bus.$on("changeView", obj => {
            //     if (obj.fromId) {
            //         this.saveContent(obj.done);
            //     }
            //     if (!obj.toId) {
            //         this.id = -1;
            //         clearInterval(this.tick);
            //         this.hide = true;
            //     } else {
            //         this.id = obj.toId;
            //         this.hide = false;
            //         this.initContent();
            //     }
            // })
        }
    }
</script>
<style scoped lang="scss">
    #editor {
        position: absolute;
        bottom: 72px;
        left: 8px;
        top: 76px;
        right: 8px;
        z-index: 9;
        border-top: 1px solid #e5e5e5;
        border-bottom: 1px solid #e5e5e5;
    }

    #editorContainer {
        height: 100% !important;
    }

    #ta {
        height: 100% !important;
        width: 100% !important;
    }
</style>