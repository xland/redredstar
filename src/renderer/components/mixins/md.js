export default {
    data() {
        return {}
    },
    methods: {
        imageUploadMd(obj) {
            let parser = new DOMParser();
            let doc = parser.parseFromString(this.articleContent, "text/html");
            let img = doc.getElementById(obj.id);
            img.dataset[obj.siteId] = obj.url;
            this.articleContent = doc.body.innerHTML;
            window.editorMd.setValue(this.articleContent);
            this.needSave = true;
        },
        initEditorMd() {
            this.$root.curArticleMd = true;
            let self = this;
            new tui.Editor({
                el: document.querySelector('#editorMd'),
                height: '100%',
                //language:'zh_CN', //todo:会显示台湾的语言
                hideModeSwitch: true,
                initialEditType: 'markdown',
                previewStyle: 'vertical',
                usageStatistics: false,
                events: {
                    load: function (editor) {
                        window.editorMd = editor;
                        window.editorMd.setValue(self.articleContent);
                        setTimeout(() => {
                            self.needSave = false;
                        }, 86);
                    },
                    change: function () {
                        self.needSave = true;
                    }
                },
                hooks: {
                    addImageBlobHook: function (file, cb, source) {
                        self.imgSaveFileObj(file, (id, fullName, err) => {
                            let imgDom = '<img id="' + id + '" src="file://' + fullName + '" />';
                            window.editorMd.insertText(imgDom);
                            self.needSave = true;
                        })
                    }
                }
            });
        }
    }
}