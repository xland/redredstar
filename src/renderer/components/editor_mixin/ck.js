export default {
    data() {
        return {
        }
    },
    methods: {
        imageUploadCk(obj) {
            let win = document.querySelector('iframe.cke_wysiwyg_frame').contentWindow
            let dom = win.document.getElementById(obj.id);
            dom.dataset[obj.siteId] = obj.url;
        },
        saveImg(f) {
            let cb = (id, fullName, err) => {
                var imgDom = '<img id="' + id + '" src="file://' + fullName + '" />';
                window.CKEDITOR.instances.editorCk.insertHtml(imgDom);
            }
            if (f.path) {
                this.imgSaveDropObj(f.path, cb)
            } else {
                this.imgSaveFileObj(f, cb)
            }
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
                    fileUploadRequest: function (evt) {
                        evt.stop();
                        self.saveImg(evt.data.fileLoader.file);
                    },
                    instanceReady: function () {
                        self.saveKeyEventHook();
                        window.CKEDITOR.instances.editorCk.setData(self.articleContent, {
                            callback: function () {
                                self.needSave = false;
                            }
                        });
                    },
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