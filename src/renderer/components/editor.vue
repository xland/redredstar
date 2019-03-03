<template>
    <div v-show="!hide" id="editor">
        <div id="editorContainer"></div>
    </div>
</template>
<script>
    //todo:图片放大缩小需要按比例
    var fs = require('fs');
    var path = require('path');
    const electron = require('electron');
    const {
        ipcRenderer,
        remote
    } = require('electron');
    export default {
        data() {
            return {
                hide: true,
                content:null,
            }
        },
        watch: {
            "$route.params.id": function (val, oldVal) {
                if (!val) {
                    this.hide = true;
                    return;
                    // this.$root.imageProcessor.init(this.$root.basePath,val.toString())
                    // imageProcessor.init(this.$root.basePath, val.toString(), this.$root.u.imgSize);
                }
                this.hide = false;
                let aPath = path.join(remote.app.getPath('userData'), "/xxm/"+val+"/a.data");
                var content = fs.readFileSync(aPath, {
                    encoding: 'utf8'
                });
                this.content = content;
                if (UE.instants.ueditorInstant0 && UE.instants.ueditorInstant0.isReady) {
                    window.UE.instants.ueditorInstant0.setContent(content);
                }else{
                    UE.instants.ueditorInstant0.addListener("ready",()=>{
                        window.UE.instants.ueditorInstant0.setContent(content);
                    })
                }
            }
        },
        methods: {
            initEditor() {
                var editor = window.UE.getEditor('editorContainer');
                var self = this;
                editor.addListener("ready", () => {
                    self.hookPasteImg();
                    self.hookImgDomChange();
                    self.hookSaveKeyEvent();
                    self.hookContentChange();
                    self.hookContentRefresh();
                    self.hookArticleRefresh();
                    if (self.article) {
                        window.editorContentReady(self.article.id);
                    }
                });
            },
            hookContentChange() {
                var self = this;
                var subContent = document.getElementById("ueditor_0").contentWindow.document;
                subContent.oninput = function (e) {
                    self.$root.needSave.c = true;
                }
            },
            hookContentRefresh() {
                var self = this;
                ipcRenderer.on('contentRefreshRenderer', (e, message) => {
                    window.UE.instants.ueditorInstant0.setContent(message.content);
                    self.$root.needSave.c = true;
                });
            },
            hookArticleRefresh() {
                var self = this;
                ipcRenderer.on('articleRefreshRenderer', (e, message) => {
                    self.$root.a[self.$root.aIndex][message.siteId] = {
                        url: message.url
                    }
                });
            },
            hookSaveKeyEvent() {
                var self = this;
                window.saveArticleKeyEvent = function () {
                    self.$root.saveContent();
                };
            },
            hookContentReady() {
                var self = this;
                window.editorContentReady = function (articleId) {
                    var aPath = path.join(self.$root.basePath, articleId + "/a.data");
                    var content = fs.readFileSync(aPath, {
                        encoding: 'utf8'
                    });
                    if (UE.instants.ueditorInstant0 && UE.instants.ueditorInstant0.isReady) {
                        window.UE.instants.ueditorInstant0.setContent(content);
                    }
                }
            },
            hookImgDomChange() {
                var self = this;
                var editorDocument = document.getElementById("ueditor_0").contentWindow.document;
                var observer = new MutationObserver(records => {
                    self.$root.needSave.c = true;
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
                        }
                        if (item.addedNodes.length > 0 && item.addedNodes[0].tagName ==
                            "IMG" && !item.addedNodes[0].src.startsWith("file")) {
                            if (item.addedNodes[0].src.startsWith("data:")) {
                                imageProcessor.saveBase64Obj(item.addedNodes[0]);
                            } else {
                                imageProcessor.saveInternetObj(item.addedNodes[0]);
                            }
                        }
                    });
                });
                observer.observe(editorDocument, {
                    childList: true,
                    subtree: true
                });
            },
            setImgDom(dom, id, src) {
                dom.removeAttribute("_src");
                dom.src = src;
                dom.id = id;
            },
            hookPasteImg() {
                var self = this;
                window.editorImgInsert = function (file) {
                    imageProcessor.saveFileObj(file, (err) => {
                        self.$root.needSave.c = true;
                    })
                }
            }
        },
        mounted() {
            var editor = window.UE.getEditor('editorContainer');
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