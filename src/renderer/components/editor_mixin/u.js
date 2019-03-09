//静态资源也用require的方式引入
const fs = require('fs');
const path = require('path');
const {
    ipcRenderer,
    remote
} = require('electron');
export default {
    methods: {
        hookContentChangeU() {
            var self = this;
            var subContent = document.getElementById("ueditor_0").contentWindow.document;
            subContent.oninput = function (e) {
                self.needSave = true;
            }
        },
        hookSaveKeyEventU() {
            var self = this;
            window.saveArticleKeyEvent = function () {
                self.saveContent();
            };
        },
        hookImgDomChangeU() {
            let self = this;
            let editorDocument = document.getElementById("ueditor_0").contentWindow.document;
            let observer = new MutationObserver(records => {
                records.forEach((item, index) => {
                    if (item.removedNodes.length > 0 && item.removedNodes[0].tagName ==
                        "IMG") {
                        this.delImgWhenDomChange(item.removedNodes[0]);
                        this.needSave = true;
                    }
                    if (item.addedNodes.length > 0 && item.addedNodes[0].tagName ==
                        "IMG" && !item.addedNodes[0].src.startsWith("file")) {
                        if (item.addedNodes[0].src.startsWith("data:")) {
                            self.imgSaveBase64Obj(item.addedNodes[0]);
                        } else {
                            self.imgSaveInternetObj(item.addedNodes[0]);
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
        hookPasteImgU() {
            var self = this;
            window.editorImgInsert = function (file) {
                self.imgSaveFileObj(file, (id, fullName, err) => {
                    var imgDom = '<img id="' + id + '" src="file://' + fullName + '" />';
                    window.UE.instants.ueditorInstant0.execCommand("inserthtml", imgDom);
                    self.needSave = true;
                })
            }
        },
        initEditorU() {
            var editor = window.UE.getEditor('editorU');
            var self = this;
            editor.addListener("ready", () => {
                self.hookPasteImgU();
                self.hookImgDomChangeU();
                self.hookSaveKeyEventU();
                self.hookContentChangeU();
                if (self.articleContent) {
                    editor.setContent(self.articleContent);
                }
            })
        },
    }
}