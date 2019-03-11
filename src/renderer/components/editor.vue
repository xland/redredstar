<template>
    <div class="editor">
        <div v-show="$parent.article.editor_type == 'markdown'" class="tip">
            请不要改变img标签的格式（会影响文章发布）
        </div>
        <div v-show="$parent.article.editor_type == 'markdown'" id="editorMd"></div>
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
                if (this.$parent.article.editor_type == "html") {
                    this.articleContent = window.editorU.getContent();
                } else {
                    this.articleContent = window.editorMd.getValue();
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
            focus() {
                if (this.$parent.article.editor_type == "html") {
                    this.editorDoc.focus();
                } else {
                    window.editorMd.focus();
                }
            },
            destroy() {
                clearInterval(this.tick);
            },
            hookImgUpload() {
                this.$root.imgUploadCb = (obj) => {
                    if (this.$parent.article.editor_type == "html") {
                        this.imageUploadU(obj);
                    } else {
                        this.imageUploadMd(obj);
                    }
                    this.needSave = true;
                };
            },
            hookWinQuit() {
                var self = this;
                remote.getCurrentWindow().on('close', (event) => {
                    event.preventDefault();
                    self.saveContent();
                    remote.app.quit();
                })
            },
            getContent() {
                this.articlePath = path.join(remote.app.getPath('userData'), "/xxm/" + this.$parent.article.id);
                this.articleContent = fs.readFileSync(path.join(this.articlePath, "a.data"), this.$root.rwOption);
                if (this.$parent.article.editor_type == "html") {
                    this.initEditorU();
                } else {
                    this.initEditorMd();
                }
                this.tick = setInterval(() => {
                    this.saveContent()
                }, this.$root.tickStep);
                this.removeUselessImg();
            }
        },
        mounted() {
            this.hookImgUpload();
            this.hookWinQuit();
        }
    }
</script>
<style scoped lang="scss">
    .editor {
        overflow: hidden;
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

    @keyframes flash {
        0% {
            color: red;
        }
        20%{
            color: #ccc;
        }
        40%{
            color: red;
        }
        60%{
            color:#ccc;
        }
        80% {
            color: red;
        }
        100% {
            color: #ccc;
        }
    }

    .tip {
        animation: flash 5.6s linear;
        position: absolute;
        z-index: 9;
        line-height: 32px;
        right: 12px;
        color: #ccc;
    }
</style>