require('codemirror/lib/codemirror.css');
require('tui-editor/dist/tui-editor.css'); // editor ui
require('tui-editor/dist/tui-editor-contents.css'); // editor content
require('highlight.js/styles/github.css'); // code block highlight
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
            window.mdEditor.setValue(this.articleContent);
        },
        initEditorMD() {
            let self = this;
            new Editor({
                el: document.querySelector('#editorMD'),
                height: '100%',
                //language:'zh_CN', //todo:会显示台湾的语言
                hideModeSwitch: true,
                initialEditType: 'markdown',
                previewStyle: 'vertical',
                usageStatistics: false,
                events: {
                    load: function (editor) {
                        window.mdEditor = editor;
                        window.mdEditor.setValue(this.articleContent);
                    },
                    change: function () {
                        self.needSave = true;
                    }
                },
                hooks: {
                    addImageBlobHook: function (file, cb, source) {
                        //source of an event the item belongs to. 'paste', 'drop', 'ui'
                        self.imgSaveFileObj(file, (id, fullName, err) => {
                            let imgDom = '<img id="' + id + '" src="file://' + fullName + '" />';
                            window.mdEditor.insertText(imgDom);
                            self.needSave = true;
                        })
                    }
                }
            });
        }
    }
}