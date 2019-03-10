//静态资源也用require的方式引入
const fs = require('fs');
const path = require('path');
const {
    ipcRenderer,
    remote
} = require('electron');
export default {
    data(){
        return {
            editorDoc:null,
        }
    },
    methods: {
        imageUploadU(obj) {
            let dom = this.editorDoc.getElementById(obj.id);
            dom.dataset[obj.siteId] = obj.url;
        },
        hookSaveKeyEventU() {
            var self = this;
            window.saveArticleKeyEvent = function () {
                self.saveContent();
            };
        },
        hookImgDomChangeU() {
            let self = this;
            let observer = new MutationObserver(records => {
                records.forEach((item, index) => {
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
            observer.observe(self.editorDoc, {
                childList: true,
                subtree: true
            });
        },
        hookPasteImgU() {
            var self = this;
            window.editorImgInsert = function (file) {
                self.imgSaveFileObj(file, (id, fullName, err) => {
                    var imgDom = '<img id="' + id + '" src="file://' + fullName + '" />';
                    window.editorU.execCommand("inserthtml", imgDom);
                    self.needSave = true;
                })
            }
        },
        initEditorU() {
            var editor = window.UE.getEditor('editorU');
            var self = this;
            editor.addListener("ready", () => {
                window.editorU = editor;
                self.editorDoc = document.querySelectorAll("#editorU iframe")[0].contentWindow.document;
                editor.setContent(self.articleContent);
                self.hookPasteImgU();
                self.hookImgDomChangeU();
                self.hookSaveKeyEventU();
                self.editorDoc.documentElement.scrollTop = self.editorDoc.scrollHeight;
                self.editorDoc.oninput = function (e) {
                    self.needSave = true;
                }
            })
        },
    }
}