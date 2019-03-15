export default {
    data() {
        return {
            ckEditorWin: null,
        }
    },
    methods: {
        pasteImgHook(){
            let self = this;
            this.ckEditorWin.addEventListener("paste", (event) => {
                var items = event.clipboardData.items;
                for (let i = 0; i < items.length; i++) {
                    if (items[i].kind === 'file') {
                        var file = items[i].getAsFile();
                        self.imgSaveFileObj(file, (id, fullName, err) => {
                            var imgDom = '<img id="' + id + '" src="file://' + fullName + '" />';
                            window.CKEDITOR.instances.ckEditor.insertHtml(imgDom);
                            //self.needSave = true;
                        })
                    }
                }
            });
        },
        dropImgHook(){
            let self = this;
            this.ckEditorWin.addEventListener("drop", (event) => {
                let items = event.dataTransfer.files;
                for (let i = 0; i < items.length; i++) {
                    self.imgSaveDropObj(items[i].path, (id, fullName) => {
                        var imgDom = '<img id="' + id + '" src="file://' + fullName + '" />';
                        window.CKEDITOR.instances.ckEditor.insertHtml(imgDom);
                        //self.needSave = true;
                    })
                };
            })
        },
        initEditorCk() {
            let self = this;
            window.CKEDITOR.replace('ckEditor', {
                on: {
                    change: function () {
                        self.needSave = true;
                        console.log(111);
                    },
                    loaded: function () {
                        // window.CKEDITOR.instances.ckEditor.filter.addElementCallback(function (el) {
                        //     //&& CKEDITOR.instances.editor.mode == 'wysiwyg'
                        //     if (el.name == 'img' && el.src && el.src.startWith("http")) {
                        //         console.log(el);
                        //     }
                        // });
                        self.ckEditorWin = document.querySelector('iframe.cke_wysiwyg_frame').contentWindow;
                        self.pasteImgHook();
                        self.dropImgHook();
                        window.CKEDITOR.instances.ckEditor.setData(self.articleContent);
                    }
                }
            });
        }
    }
}