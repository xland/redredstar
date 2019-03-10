require('codemirror/lib/codemirror.css');
require('tui-editor/dist/tui-editor.css');
require('tui-editor/dist/tui-editor-contents.css');
require('highlight.js/styles/github.css');
var Editor = require('tui-editor');

export default {
    data() {
        return {

        }
    },
    methods: {
        imageUploadMd(obj) {
            let preStr = '<img id="' + obj.id + '"';
            let str = '<img id="' + obj.id + '" data-' + obj.siteId + '="' + obj.url + '"';
            this.articleContent = this.articleContent.replace(preStr, str);
            window.editorMd.setValue(this.articleContent);
            this.needSave = true;
        },
        initEditorMd() {
            let self = this;
            new Editor({
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
                            console.log("2")
                        }, 86);
                    },
                    change: function () {
                        self.needSave = true;
                        console.log("1")
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