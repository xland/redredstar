export default {
    data() {
        return {
            ckEditorWin: null,
        }
    },
    methods: {
        imageUploadCk(obj) {
            let dom = this.ckEditorWin.document.getElementById(obj.id);
            dom.dataset[obj.siteId] = obj.url;
        },
        pasteImgHook() {
            let self = this;
            this.ckEditorWin.addEventListener("paste", (event) => {
                var items = event.clipboardData.items;
                for (let i = 0; i < items.length; i++) {
                    if (items[i].kind === 'file') {
                        var file = items[i].getAsFile();
                        self.imgSaveFileObj(file, (id, fullName, err) => {
                            var imgDom = '<img id="' + id + '" src="file://' + fullName + '" />';
                            window.CKEDITOR.instances.editorCk.insertHtml(imgDom);
                        })
                    }
                }
            });
        },
        dropImgHook() {
            let self = this;
            this.ckEditorWin.addEventListener("drop", (event) => {
                let items = event.dataTransfer.files;
                for (let i = 0; i < items.length; i++) {
                    self.imgSaveDropObj(items[i].path, (id, fullName) => {
                        var imgDom = '<img id="' + id + '" src="file://' + fullName + '" />';
                        window.CKEDITOR.instances.editorCk.insertHtml(imgDom);
                    })
                };
            })
        },
        saveKeyEventHook() {
            let self = this;
            let instans = window.CKEDITOR.instances.editorCk
            instans.addCommand('saveContent', {
                exec: function (editor, data) {
                    self.saveContent();
                }
            });
            instans.keystrokeHandler.keystrokes[CKEDITOR.CTRL + 83] = 'saveContent';
        },
        initEditorCk() {
            this.$root.curArticleMd = false;
            let self = this;
            window.CKEDITOR.replace('editorCk', {
                on: {
                    change: function () {
                        self.needSave = true;
                    },
                    instanceReady: function () {
                        self.ckEditorWin = document.querySelector('iframe.cke_wysiwyg_frame').contentWindow;
                        self.pasteImgHook();
                        self.dropImgHook();
                        self.saveKeyEventHook();
                        window.CKEDITOR.instances.editorCk.setData(self.articleContent, {
                            callback: function () {
                                self.needSave = false;
                            }
                        });
                    }
                }
            });
        }
    }
}
// window.CKEDITOR.instances.ckEditor.filter.addElementCallback(function (el) {
//     //&& CKEDITOR.instances.editor.mode == 'wysiwyg'
//     if (el.name == 'img' && el.src && el.src.startWith("http")) {
//         console.log(el);
//     }
// });