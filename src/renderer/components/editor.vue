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
                articleId: null,
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
                }).where("id", this.articleId).then(rows => {
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
            hookArticleRefresh() {
                ipcRenderer.on('articleRefreshRenderer', (e, message) => {
                    this.db('article_site')
                        .where("article_id", this.articleId)
                        .andWhere("site_id", message.siteId)
                        .select("*").then(rows => {
                            let asObj = {
                                article_id: this.articleId,
                                site_id: message.siteId,
                                edit_url: message.url
                            }
                            if (rows.length < 1) {
                                this.db("article_site").insert(asObj).then();
                            } else {
                                this.db("article_site").update(asObj).where("id", rows[0].id).then();
                            }
                        });
                });
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
        },
        mounted() {
            this.hookContentRefresh();
            this.hookArticleRefresh();
            this.hookWinQuit();
            //todo:全局的setting
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
                    this.articleContent = fs.readFileSync(path.join(this.articlePath, "a.data"), this.$root.rwOption);
                    this.hide = false;
                    this.initContent();
                    this.removeUselessImg();
                }
            })
        }
    }
</script>
<style scoped lang="scss">
    .editor {
        flex: 1;background: #fff;
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