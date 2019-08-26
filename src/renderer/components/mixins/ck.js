export default {
    data() {
        return {}
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
                exec: async function(editor, data) {
                    await self.saveContent();
                }
            });
            instans.addCommand('findAndReplace', {
                exec: function(editor, data) {
                    CKEDITOR.tools.callFunction(105, editor);
                }
            });
            // instans.addCommand('cancleCmdZ', {
            //     exec: function (editor, data) {
            //         return false;
            //     }
            // });
            // instans.keystrokeHandler.keystrokes[CKEDITOR.CTRL + 90] = 'cancleCmdZ';
            instans.keystrokeHandler.keystrokes[CKEDITOR.CTRL + 83] = 'saveContent';
            instans.keystrokeHandler.keystrokes[CKEDITOR.CTRL + 70] = 'findAndReplace';
        },
        hook() {
            let self = this;
            self.saveKeyEventHook();
            CKEDITOR.instances.editorCk.on("key", function() {
                self.needSave = true;
            });
            CKEDITOR.instances.editorCk.on("fileUploadRequest", function(evt) {
                evt.stop();
                self.saveImg(evt.data.fileLoader.file);
            });
        },
        initEditorCk() {
            this.$root.curArticleMd = false;
            let self = this;
            window.CKEDITOR.replace('editorCk', {
                on: {
                    instanceReady: function() {
                        self.hook();
                        self.needSave = false;
                        self.downloadInternetImg();
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