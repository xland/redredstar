<template>
    <div v-show="!hide" id="editor">
        <div v-if="editorType == 'html'" id="editorU"></div>
        <div v-if="editorType == 'markdown'" id="editorMD"></div>
        <div class="mdSwitchBtn">
            <div @click="mdSwitch('md')" :class="mdSwitchType=='md'?'mdSelected':''">Markdow</div>
            <div @click="mdSwitch('wysiwyg')" :class="mdSwitchType=='wysiwyg'?'mdSelected':''">WYSIWYG</div>
        </div>
    </div>
</template>
<script>
    //todo:图片放大缩小需要按比例
    const fs = require('fs');
    const path = require('path');
    import u from './editor_mixin/u';
    import img from './editor_mixin/img';
    import md from './editor_mixin/md';
    const {
        ipcRenderer,
        remote
    } = require('electron');
    export default {
        mixins: [u, img, md],
        data() {
            return {
                articleId: null,
                articleContent: null,
                articlePath: null,
                editorType: 'html',
                hide: true,
                tick: null,
                tickStep: 8000,
                rwOption: {
                    encoding: 'utf8'
                },
                needSave: false,
            }
        },
        methods: {
            mdSwitch(type) {
                this.mdSwitchType = type;
                if (type == "md") {
                    this.mdEditor.layout.switchToMarkdown();
                } else {
                    this.mdEditor.layout.switchToWYSIWYG();
                }
            },
            saveContent(cb) {
                if (!this.needSave) {
                    if (cb) {
                        cb();
                    }
                    return;
                }
                this.bus.$emit('saveContent');
                if (this.editorType == "html") {
                    this.articleContent = window.UE.instants.ueditorInstant0.getContent();
                } else {
                    this.articleContent = this.mdEditor.getValue();
                }
                fs.writeFileSync(path.join(this.articlePath, "a.data"), this.articleContent, this.rwOption);
                this.needSave = false;
                this.db("articles").update({
                    updated_at: new Date()
                }).where("id", this.articleId).then(rows => {
                    if (cb) {
                        cb();
                    }
                });
            },
            initContent() {
                if (this.editorType == "html" && UE.instants.ueditorInstant0 && UE.instants.ueditorInstant0.isReady) {
                    window.UE.instants.ueditorInstant0.setContent(this.articleContent);
                    var editorDoc = document.getElementById("ueditor_0").contentWindow.document;
                    editorDoc.documentElement.scrollTop = editorDoc.scrollHeight;
                }
                if (this.editorType == "markdown") {
                    let self = this;
                    this.$nextTick(function () {
                        self.mdEditor.setValue(this.articleContent);
                    })
                }
                this.tick = setInterval(() => {
                    this.saveContent()
                }, this.tickStep)
            },
        },
        mounted() {
            //todo:全局的setting
            this.db("settings").select("*").then(rows => {
                this.tickStep = rows[0].autosave_interval * 1000;
                this.imgHight = rows[0].img_h;
                this.imgWidth = rows[0].img_w;
                this.editorType = rows[0].editor_type;
                this.$nextTick(function () {
                    if (this.editorType == "html") {
                        this.initEditorU();
                    } else {
                        this.initEditorMD();
                    }
                })
            });
            this.bus.$on("changeView", obj => {
                if (obj.fromId) {
                    this.saveContent(obj.done);
                }
                if (!obj.toId) {
                    this.articleId = null;
                    clearInterval(this.tick);
                    this.hide = true;
                } else {
                    this.articleId = obj.toId;
                    this.articlePath = path.join(remote.app.getPath('userData'), "/xxm/" + this.articleId);
                    this.articleContent = fs.readFileSync(path.join(this.articlePath, "a.data"), this.rwOption);
                    this.hide = false;
                    this.initContent();
                }
            })
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

    .mdSwitchBtn {
        z-index: 10;
        position: absolute;
        top: 0px;
        right: 0px;
    }

    .mdSwitchBtn div {
        line-height: 31px;
        height: 31px;
        display: inline-block;
        width: 80px;
        text-align: center;
        background: #eee;
        cursor: pointer;
    }

    .mdSelected {
        background: #fff !important;
    }

    #editorU {
        height: 100% !important;
    }

    #ta {
        height: 100% !important;
        width: 100% !important;
    }
</style>