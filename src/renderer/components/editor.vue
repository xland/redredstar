<template>
    <div v-show="$root.aIndex>=0" id="editor">
        <div id="editorContainer"></div>
    </div>
</template>
<script>
    //todo:图片放大缩小需要按比例
    //todo:公式插件base64 to file
    var fs = require('fs');
    var path = require('path');
    const {
        ipcRenderer
    } = require('electron');
    export default {
        data() {
            return {

            }
        },
        computed: {
            article() {
                return this.$root.a[this.$root.aIndex];
            }
        },
        methods: {
            initEditor() {
                var editor = window.UE.getEditor('editorContainer');
                var self = this;
                editor.addListener("ready", () => {
                    self.hookImgInsert();
                    self.hookImgRemove();
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
                    self.$root.a[self.$root.aIndex].cnblogs = {
                        url: message.url
                    }
                    console.log(self.$root.a[self.$root.aIndex]);
                    self.$root.needSave.a = true;
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
            hookImgRemove() {
                var self = this;
                var editorDocument = document.getElementById("ueditor_0").contentWindow.document;
                var observer = new MutationObserver(records => {
                    self.$root.needSave.c = true;
                    records.forEach((item, index) => {
                        if (item.removedNodes.length > 0 && item.removedNodes[0].tagName ==
                            "IMG") {
                            var path = decodeURI(item.removedNodes[0].src.substr(7));
                            fs.unlink(path, err => {
                                if (err) console.log(err);
                            });
                        }
                    });
                });
                observer.observe(editorDocument, {
                    childList: true,
                    subtree: true
                });
            },
            hookImgInsert() {
                var self = this;
                window.editorImgInsert = function (file) {
                    var basePath = path.join(self.$root.basePath, self.article.id.toString());
                    var id = "img" + new Date().getTime();
                    var name = path.join(basePath, id + '.' + file.name.split('.').last());
                    var fr = new FileReader();
                    fr.onload = () => {
                        if (fr.readyState == 2) {
                            var buffer = new Buffer(fr.result);
                            fs.writeFileSync(name, buffer);
                            var imgDom = '<img id="' + id + '" src="file://' + name + '" />';
                            window.UE.instants.ueditorInstant0.execCommand("inserthtml", imgDom);
                        }
                    };
                    fr.readAsArrayBuffer(file);
                }
            }
        },
        created() {
            this.hookContentReady();
        },
        mounted() {
            this.initEditor();
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