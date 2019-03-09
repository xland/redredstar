<template>
    <div class="editor">
        <div v-if="editorType == 'html'" id="editorU"></div>
        <div v-if="editorType == 'markdown'" id="editorMD"></div>
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
                articleContent: null,
                articlePath: null,
                editorType: 'html',
                tick: null,
                needSave: false,
            }
        },
        methods: {
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
                    let mdStr = window.mdEditor.getValue();
                    this.articleContent = window.mdEditor.convertor.toHTMLWithCodeHightlight(mdStr)
                }
                fs.writeFileSync(path.join(this.articlePath, "a.data"), this.articleContent, this.$root.rwOption);
                this.needSave = false;
                this.db("articles").update({
                    updated_at: new Date()
                }).where("id", this.$parent.article.id).then(rows => {
                    if (cb) {
                        cb();
                    }
                });
            },
            hookContentRefresh() {
                ipcRenderer.on('contentRefreshRenderer', (e, message) => {
                    this.articleContent = message.content;
                    if (this.editorType == "html") {
                        window.UE.instants.ueditorInstant0.setContent(this.articleContent);
                    } else {
                        this.htmlToMd();
                    }
                    this.needSave = true;
                });
            },
            hookWinQuit() {
                var self = this;
                remote.getCurrentWindow().on('close', (event) => {
                    event.preventDefault();
                    self.saveContent();
                    remote.app.quit();
                })
            },
            initContent() {
                if (this.editorType == "html" && UE.instants.ueditorInstant0 && UE.instants.ueditorInstant0.isReady) {
                    window.UE.instants.ueditorInstant0.setContent(this.articleContent);
                    var editorDoc = document.getElementById("ueditor_0").contentWindow.document;
                    editorDoc.documentElement.scrollTop = editorDoc.scrollHeight;
                }
                if (this.editorType == "markdown") {
                    this.$nextTick(function () {
                        this.htmlToMd();
                    })
                }
                this.tick = setInterval(() => {
                    this.saveContent()
                }, this.$root.tickStep)
            },
            getContent(id) {
                this.articlePath = path.join(remote.app.getPath('userData'), "/xxm/" + this.$parent.article.id);
                this.articleContent = fs.readFileSync(path.join(this.articlePath, "a.data"), this.$root.rwOption);
                this.hide = false;
                this.initContent();
                this.removeUselessImg();
            }
        },
        mounted() {
            this.hookContentRefresh();
            this.hookArticleRefresh();
            this.hookWinQuit();
        }
    }
</script>
<style scoped lang="scss">
    .editor {
        flex: 1;
        background: #fff;
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